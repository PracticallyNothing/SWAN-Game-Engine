#include "CMakeConfig.h"

#include "Core/Display.hpp"	// For Display
#include "Core/Input.hpp"	  // For Input
#include "Core/Resources.hpp"  // For Resources::LoadFromFile(), Resources::Get*()

#include "GUI/GUI.hpp" // For GUI::*

#include "Utility/StringUtil.hpp" // For Util::GetDirectory(), Util::IsAbsolutePath()
#include "Utility/Debug.hpp"	  // For DEBUG_OUT()
#include "Utility/Math.hpp"	   // For Util::pixelToGLCoord()
#include "Utility/Profile.hpp"	// For Util::CurrentProfile, UTIL_PROFILE()
#include "Utility/XML.hpp"		// For Util::ReadXML(), Util::XML

#include "Rendering/Camera.hpp"		// For Camera
#include "Rendering/Mesh.hpp"		// For Mesh
#include "Rendering/Shader.hpp"		// For Shader
#include "Rendering/Texture.hpp"	// For Texture
#include "Rendering/OBJ-Import.hpp"	// For Import::OBJ()
#include "Rendering/Light.hpp"		// For DirectionalLight, PointLight
#include "Rendering/BitmapFont.hpp" // For BitmapFont
#include "Rendering/Text.hpp"	   // For Text

#define STBI_IMPLEMENTATION
#include "External/stb_image.h" // For stbi_set_flip_vertically_on_load()
#undef STBI_IMPLEMENTATION

#include "Logic/Entity.hpp" // For Entity

#include <iostream>	// For std::cout
#include <memory>	// For std::unique_ptr<T>, std::make_unique<T>()
#include <chrono>	// For std::chrono::*
#include <cstring>  // For std::strcmp(), std::strncmp(), strcpy()

using namespace Util::StreamOps;
using std::unique_ptr;
using std::make_unique;

using Ms 	= std::chrono::milliseconds;
using Clock = std::chrono::steady_clock;
using std::chrono::operator""ms;
using std::chrono::duration_cast;

struct DisplayConfig {
	int w, h;
	std::string title;
};

// TODO: Remove this class, it's a disgusting god object
class Game {
	public:
		explicit Game(std::string resourcesFile, DisplayConfig conf = {640, 480, "OpenGL-CPP-Engine"})
			: display(conf.w, conf.h, conf.title),
			run(true)
			{
				DEBUG_PRINT("    [Game::Game()] Display created.");

				//UTIL_PROFILE();
				Display::setClearColor(0.0f, 0.3f, 0.25f, 0.0f);
				// SDL_SetRelativeMouseMode(SDL_TRUE);
				Input_init();

				DEBUG_PRINT("    [Game::Game()] Loading resources...");
				Resources::LoadFromFile(resourcesFile);
				DEBUG_PRINT("    [Game::Game()] Resources loaded...");

				tex         = Resources::GetTexture("Monkey Head Texture");
				planeMesh   = Resources::GetMesh("Plane");
				shotgunMesh = Resources::GetMesh("SPAS 12");
				torusMesh   = Resources::GetMesh("Torus");
				font        = Resources::GetBitmapFont("Monospace 16");

				shader = Resources::GetShader("Proper");
				textShader = Resources::GetShader("Text");

				cam = make_unique<Camera>(display.getAspect(), glm::vec3(0.0f, 0.0f, -1.0f));

				DirectionalLight dl0;
				dl0.direction = glm::normalize(glm::vec3(1, 1, 1));
				dl0.ambient   = glm::vec3(0.003, 0.0, 0.003);
				dl0.diffuse   = glm::vec3(0.0, 0.0, 0.3);
				dl0.specular  = glm::vec3(1, 1, 1);

				PointLight pl1;
				pl1.position	 = glm::vec3(1, 7, 15);
				pl1.ambient	     = glm::vec3(0.05, 0.0005, 0.0);
				pl1.diffuse	     = glm::vec3(0.5, 0.005, 0.0);
				pl1.specular     = glm::vec3(1, 1, 1);
				pl1.linearAtt	 = 0.045;
				pl1.quadraticAtt = 0.0075;

				shader->use();
				{
					shader->setUniformData("matSpecular", glm::vec3(0.5, 0.5, 0.5));
					shader->setUniformData("matShininess", 2.0f);
					shader->setUniformData("lights[0]", dl0);
					shader->setUniformData("lights[1]", pl1);
					shader->setUniformData("activeLights", 2);
				}
				shader->unuse();

				guiRenderer = make_unique<GUI::Renderer>();
				guiRenderer->add(new GUI::Draggable(Resources::GetTexture("Flat Blue"),
												//Resources::GetTexture("Flat Red"),
												//Resources::GetTexture("Flat Cyan"),
												100, 100));

				plane.getTransform_ref().pos = pl1.position;

				plane.setMesh(planeMesh);
				plane.setTexture(tex);

				shotgun.setMesh(shotgunMesh);
				shotgun.setTexture(tex);

				// ----- GL enable stuff -----
				glEnable(GL_PROGRAM_POINT_SIZE);
				glEnable(GL_DEPTH_TEST);

				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

		~Game() {}

		void update() {
			Input.handleEvents();
			if (Input.Window.exitRequest || Input.Keyboard.escapeKey) run = false;

			float moveSpeed = 0.1;
			Util::Radians rotSpeed(.05);

			if (Input.Keyboard.letterKeys['a' - 'a'])
				cam->moveRight(-moveSpeed);
			else if (Input.Keyboard.letterKeys['d' - 'a'])
				cam->moveRight(moveSpeed);

			if (Input.Keyboard.letterKeys['w' - 'a'])
				cam->moveForw(moveSpeed);
			else if (Input.Keyboard.letterKeys['s' - 'a'])
				cam->moveForw(-moveSpeed);

			if (Input.Keyboard.letterKeys['q' - 'a'])
				cam->rotateByZ(rotSpeed);
			else if (Input.Keyboard.letterKeys['e' - 'a'])
				cam->rotateByZ(-rotSpeed);

			if (Input.Keyboard.spaceKey)
				cam->moveUp(moveSpeed);
			else if (Input.Keyboard.LShiftKey)
				cam->moveUp(-moveSpeed);

			if(Input.Keyboard.letterKeys['z' - 'a'])
				cam->rotateByX(Util::Radians::FromDegrees(-45.0f));
			else if(Input.Keyboard.letterKeys['c' - 'a'])
				cam->rotateByX(Util::Radians::FromDegrees(45.0f));

			if (Input.Keyboard.letterKeys['y' - 'a'])
				std::cout << "Camera FOV: " << (cam->fov -= glm::radians(1.0)) << '\n';
			if (Input.Keyboard.letterKeys['h' - 'a'])
				std::cout << "Camera FOV: " << (cam->fov += glm::radians(1.0)) << '\n';

			auto mouseX 	= Util::pixelToGLCoord(display.getW(), Input.Mouse.x);
			auto prevMouseX = Util::pixelToGLCoord(display.getW(), PrevInput.Mouse.x);

			cam->rotateByY(Util::Radians((mouseX - prevMouseX) * 2.0));

			auto mouseY 	= Util::pixelToGLCoord(display.getH(), Input.Mouse.y);
			auto prevMouseY = Util::pixelToGLCoord(display.getH(), PrevInput.Mouse.y);

			cam->rotateByX(Util::Radians((mouseY - prevMouseY) * 2.0));

			//DEBUG_OUT(Util::Degrees::FromRadians(cam->getTransform_ref().rot.x));
			//DEBUG_OUT(Util::Degrees::FromRadians(cam->getTransform_ref().rot.y));
			//DEBUG_OUT(Util::Degrees::FromRadians(cam->getTransform_ref().rot.z));
			//plane.getTransform_ref().rot.x += 0.01f;

			plane.getTransform_ref().pos.x = std::sin(time) * 10;
			//plane.getTransform_ref().pos.y = std::cos(time) * 10;
			//plane.getTransform_ref().pos.z = std::sin(time) * std::cos(time) * 10;

			time += 0.01f;

			PrevInput = Input;

			TextConfig tc;
			//tc.bold = true;
			tc.italics = true;
			tc.text = std::string("time: ") + std::to_string(time) + "\n ";
			tc.font = font;
			txt.set(display, tc, false);

			guiRenderer->update();
		}

		void render() {
			shader->use();
			{
				const float shininessD = 0.5;
				if(Input.Keyboard.letterKeys['p' - 'a']){
					shininess += shininessD;
				} else if (Input.Keyboard.letterKeys['o' - 'a']) {
					shininess -= shininessD;
				}

				if(shininess < 0)
					shininess = 0;

				shader->setUniformData("matShininess", shininess);

				shader->setUniformData("camPos", cam->getPos());
				shader->setUniformData("view", cam->getView());
				shader->setUniformData("perspective", cam->getPerspective());

				shotgun.useTransform(shader);
				shotgun.render();

				plane.useTransform(shader);
				plane.render();
				plane.getMesh()->renderWireframe();
				plane.getMesh()->renderVerts();
			}
			shader->unuse();

			TextConfig tc;
			tc.text = std::string("shininess: ") + std::to_string(shininess);
			tc.font = font;
			tc.color = glm::vec3(0.3, 0.0, 0.7);
			txt.set(display, tc, true);


			glClear(GL_DEPTH_BUFFER_BIT);
			//txt.render(textShader);
			guiRenderer->render(display);

			glClear(GL_DEPTH_BUFFER_BIT);
			display.clear();
		}

		bool running() { return run; }

	private:
		Display display;
		bool run;

		_input PrevInput;

		Shader* shader;
		Entity plane, shotgun,
			   xTorus, yTorus, zTorus;

		const Mesh *planeMesh, *shotgunMesh, *torusMesh;
		const Texture* tex;
		unique_ptr<Camera> cam;

		Text txt;
		Shader* textShader;
		const BitmapFont* font;

		unique_ptr<GUI::Renderer> guiRenderer;

		float shininess = 10.0f;
		float time = 0;
};

void Version(){
	std::cout << PROJECT_NAME << " version " << VERSION_STRING << " (Build type: " << BUILD_TYPE << ")\n";
}

void Usage(){
	std::cout
		<< "Usage: " << PROJECT_NAME << " [options]\n"
		<< '\n'
		<< "Available options:\n"
		<< "	-h, --help		 Show this help\n"
		<< "	-v, --version	  Display version\n"
		<< "	--config=<config>  Use custom configuration file, default is ./Config.xml\n";
}

char ConfigFile[256] = {0};

void ProcessArgs(int argc, char** argv){
	for(int i = 1; i < argc; i++){
		if(!std::strcmp(argv[i], "-h") || !std::strcmp(argv[i], "--help")){
			Usage();
			exit(EXIT_SUCCESS);
		} else if(!std::strcmp(argv[i], "-v") || !std::strcmp(argv[i], "--version")){
			Version();
			exit(EXIT_SUCCESS);
		} else if(!std::strncmp(argv[i], "--config=", 9)){
			if(strlen(argv[i]) <= 9){
				std::cout << "ERROR: \"--config=\" flag was passed, but no file was set, the flag will be ignored.\n";
				continue;
			}
			strcpy(ConfigFile, argv[i] + 9);
		} else {
			std::cout << "ERROR: Unknown or unsupported flag \"" << argv[i] << "\" was passed.\n"
					  << "-------------------------------------------------------------------------\n";
			Usage();
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char** argv) {
	//stbi_set_flip_vertically_on_load(true);

	if(argc > 1){
		ProcessArgs(argc, argv);
	}

	if(!ConfigFile[0]){
		strcpy(ConfigFile, "./Config.xml");
	}

	auto xml = Util::ReadXML(Util::Unquote(std::string(ConfigFile)));
	auto v = xml.findTagsWithName("Resolution");
	auto resourcesFile = xml.findTagsWithName("Resources").at(0)->getAttrib("file");

	if(Util::IsRelativePath(resourcesFile))
		resourcesFile = Util::GetDirectory(std::string(ConfigFile), true) + resourcesFile;

	DEBUG_OUT(resourcesFile);

	DEBUG_PRINT("Game::Game(): ");
	Game game(resourcesFile,
			{
				std::stoi(v.front()->getAttrib("width")),
				std::stoi(v.front()->getAttrib("height")),
				"OGL-Engine"
			});

	while (game.running()) {
		game.update();
		game.render();
	}

    return 0;
}
