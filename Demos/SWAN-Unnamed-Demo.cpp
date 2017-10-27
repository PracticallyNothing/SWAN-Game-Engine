#define SDL_main_h_ // SDL_Main might be very useful, but I find it to be pesky.

#include "CMakeConfig.h"

#include "Core/Display.hpp"   // For SWAN::Display
#include "Core/Input.hpp"     // For SWAN_Input
#include "Core/Resources.hpp" // For SWAN::Res::LoadFromFile(), SWAN::Res::Get*()

#include "GUI/GUIPrim.hpp"     // For SWAN::GUIP::*
#include "GUI/GUIRenderer.hpp" // For SWAN::GUIRenderer

#include "Utility/Debug.hpp"      // For SWAN_DEBUG_OUT()
#include "Utility/Math.hpp"       // For SWAN::Util::pixelToGLCoord()
#include "Utility/Profile.hpp"    // For SWAN::Util::CurrentProfile, UTIL_PROFILE()
#include "Utility/StreamOps.hpp"  // For SWAN::Util::StreamOps::*
#include "Utility/StringUtil.hpp" // For SWAN::Util::GetDirectory(), SWAN::Util::IsAbsolutePath()
#include "Utility/XML.hpp"        // For SWAN::Util::ReadXML(), SWAN::Util::XML

#include "Rendering/BitmapFont.hpp" // For SWAN::BitmapFont
#include "Rendering/Camera.hpp"     // For SWAN::Camera
#include "Rendering/Light.hpp"      // For SWAN::DirectionalLight, SWAN::PointLight
#include "Rendering/Mesh.hpp"       // For SWAN::Mesh
#include "Rendering/OBJ-Import.hpp" // For SWAN::Import::OBJ()
#include "Rendering/Shader.hpp"     // For SWAN::Shader
#include "Rendering/Text.hpp"       // For SWAN::Text
#include "Rendering/Texture.hpp"    // For SWAN::Texture

#include "Physics/AABB.hpp"           // For SWAN::AABB
#include "Physics/CheckCollision.hpp" // For SWAN::CheckCollision()

#define STBI_IMPLEMENTATION
#include "External/stb_image.h" // For stbi_set_flip_vertically_on_load()
#undef STBI_IMPLEMENTATION

#include "Logic/Entity.hpp" // For SWAN::Entity

#include <chrono>   // For std::chrono::*
#include <cstring>  // For std::strcmp(), std::strncmp(), strcpy()
#include <iostream> // For std::cout
#include <memory>   // For std::unique_ptr<T>, std::make_unique<T>()

using namespace SWAN::Util::StreamOps;
using std::unique_ptr;
using std::make_unique;

using namespace std::chrono;
using Clock = std::chrono::steady_clock;

// TODO: Remove this class, it's a disgusting god object
class Game {
  public:
	explicit Game(std::string resourcesFile) {
		SWAN_DEBUG_PRINT("    [Game::Game()] Display initialized.");

		SWAN::Display::SetClearColor(0.0f, 0.3f, 0.25f, 0.0f);
		SWAN_Input_Init();

		SWAN_DEBUG_PRINT("    [Game::Game()] Loading resources...");
		SWAN::Res::LoadFromFile(resourcesFile);
		SWAN_DEBUG_PRINT("    [Game::Game()] Resources loaded...");

		tex         = SWAN::Res::GetTexture("Monkey Head Texture");
		planeMesh   = SWAN::Res::GetMesh("Plane");
		shotgunMesh = SWAN::Res::GetMesh("SPAS 12");
		torusMesh   = SWAN::Res::GetMesh("Torus");
		font        = SWAN::Res::GetBitmapFont("Monospace 16");

		shader     = SWAN::Res::GetShader("Proper");
		textShader = SWAN::Res::GetShader("Text");

		cam = make_unique<SWAN::Camera>(SWAN::Display::GetAspectRatio(),
		                                glm::vec3(0.0f, 0.0f, -1.0f));

		SWAN::DirectionalLight dl0;
		dl0.direction = glm::normalize(glm::vec3(1, 1, 1));
		dl0.ambient   = glm::vec3(0.003, 0.0, 0.003);
		dl0.diffuse   = glm::vec3(0.0, 0.0, 0.3);
		dl0.specular  = glm::vec3(1, 1, 1);

		SWAN::PointLight pl1;
		pl1.position     = glm::vec3(1, 7, 15);
		pl1.ambient      = glm::vec3(0.05, 0.0005, 0.0);
		pl1.diffuse      = glm::vec3(0.5, 0.005, 0.0);
		pl1.specular     = glm::vec3(1, 1, 1);
		pl1.linearAtt    = 0.045;
		pl1.quadraticAtt = 0.0075;

		std::vector<SWAN::ShaderUniform> uniforms = {
			{ "matSpecular", glm::vec3(0.5, 0.5, 0.5) },
			{ "matShininess", 2.0f },
			{ "lights[0]", dl0 },
			{ "lights[1]", pl1 },
			{ "activeLights", 2 },
		};

		shader->setUniforms(uniforms);

		guiRenderer = make_unique<SWAN::GUIRenderer>();
		guiRenderer->add(new SWAN::GUIP::Draggable(
		    SWAN::Res::GetTexture("Flat Red"), 100, 100));
		guiRenderer->add(new SWAN::GUIP::Draggable(
		    SWAN::Res::GetTexture("Flat Blue"), 100, 100));

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

		run = true;
	}

	~Game() {}

	void update() {
		if(SWAN_Input.Window.exitRequest || SWAN_Input.Keyboard.escapeKey)
			run = false;

		float moveSpeed = 0.1;
		SWAN::Util::Radians rotSpeed(.05);

		if(SWAN_Input.Keyboard.letterKeys['a' - 'a'])
			cam->moveRight(-moveSpeed);
		else if(SWAN_Input.Keyboard.letterKeys['d' - 'a'])
			cam->moveRight(moveSpeed);

		if(SWAN_Input.Keyboard.letterKeys['w' - 'a'])
			cam->moveForw(moveSpeed);
		else if(SWAN_Input.Keyboard.letterKeys['s' - 'a'])
			cam->moveForw(-moveSpeed);

		if(SWAN_Input.Keyboard.letterKeys['q' - 'a'])
			cam->rotateByZ(rotSpeed);
		else if(SWAN_Input.Keyboard.letterKeys['e' - 'a'])
			cam->rotateByZ(-rotSpeed);

		if(SWAN_Input.Keyboard.spaceKey)
			cam->moveUp(moveSpeed);
		else if(SWAN_Input.Keyboard.LShiftKey)
			cam->moveUp(-moveSpeed);

		if(SWAN_Input.Keyboard.letterKeys['z' - 'a'])
			cam->rotateByX(SWAN::Util::Radians::FromDegrees(-45.0f));
		else if(SWAN_Input.Keyboard.letterKeys['c' - 'a'])
			cam->rotateByX(SWAN::Util::Radians::FromDegrees(45.0f));

		if(SWAN_Input.Keyboard.letterKeys['y' - 'a'])
			std::cout << "Camera FOV: " << (cam->fov -= glm::radians(1.0))
			          << '\n';
		if(SWAN_Input.Keyboard.letterKeys['h' - 'a'])
			std::cout << "Camera FOV: " << (cam->fov += glm::radians(1.0))
			          << '\n';

		auto mouseX = SWAN::Util::PixelToGLCoord(SWAN::Display::GetWidth(),
		                                         SWAN_Input.Mouse.x);
		auto prevMouseX = SWAN::Util::PixelToGLCoord(SWAN::Display::GetWidth(),
		                                             PrevInput.Mouse.x);

		cam->rotateByY(SWAN::Util::Radians((mouseX - prevMouseX) * 2.0));

		auto mouseY = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(),
		                                         SWAN_Input.Mouse.y);
		auto prevMouseY = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(),
		                                             PrevInput.Mouse.y);

		cam->rotateByX(SWAN::Util::Radians((mouseY - prevMouseY) * 2.0));

		plane.getTransform_ref().pos.x = std::sin(time) * 10;
		plane.getTransform_ref().pos.z = std::sin(time) * 10;

		time += 0.01f;

		PrevInput = SWAN_Input;

		SWAN::TextConfig tc;
		// tc.bold = true;
		tc.italics = true;
		tc.text    = std::string("time: ") + std::to_string(time) + "\n ";
		tc.font    = font;
		txt.set(tc, false);

		guiRenderer->update();
	}

	void render() {
		shader->setUniform({ "matShininess", shininess });

		shader->use();
		{
			const float shininessD = 0.5;
			if(SWAN_Input.Keyboard.letterKeys['p' - 'a']) {
				shininess += shininessD;
			} else if(SWAN_Input.Keyboard.letterKeys['o' - 'a']) {
				shininess -= shininessD;
			}

			if(shininess < 0)
				shininess = 0;

			shader->setUniformData("camPos", cam->getPos());
			shader->setUniformData("view", cam->getView());
			shader->setUniformData("perspective", cam->getPerspective());

			shotgun.render(shader);
			plane.render(shader);
		}
		shader->unuse();

		auto col = SWAN::CheckCollision(
		    SWAN::ApplyTransform(shotgunMesh->getColWrapper().aabb,
		                         shotgun.getTransform()),
		    SWAN::ApplyTransform(planeMesh->getColWrapper().aabb,
		                         plane.getTransform()));

		SWAN::Render(SWAN::ApplyTransform(shotgunMesh->getColWrapper().aabb,
		                                  shotgun.getTransform()),
		             cam.get(),
		             col.happened);

		SWAN::Render(SWAN::ApplyTransform(planeMesh->getColWrapper().aabb,
		                                  plane.getTransform()),
		             cam.get(),
		             col.happened);

		/*
		           TextConfig tc;
		           tc.text = std::string("shotgun center: (") +
		   std::to_string(SMSC.x) + ", " + std::to_string(SMSC.y) + ", " +
		   std::to_string(SMSC.z) + ")";
		           tc.font = font;
		           tc.color = glm::vec3(0.3, 0.0, 0.7);
		           txt.set(tc, true);
		           */

		glClear(GL_DEPTH_BUFFER_BIT);
		// txt.render(textShader);
		guiRenderer->render();

		glClear(GL_DEPTH_BUFFER_BIT);
		SWAN::Display::Clear();
	}

	bool running() { return run; }

  private:
	bool run;

	_input PrevInput;

	SWAN::Shader* shader;
	SWAN::Entity plane, shotgun, xTorus, yTorus, zTorus;

	const SWAN::Mesh *planeMesh, *shotgunMesh, *torusMesh;
	const SWAN::Texture* tex;
	unique_ptr<SWAN::Camera> cam;

	SWAN::Text txt;
	SWAN::Shader* textShader;
	const SWAN::BitmapFont* font;

	unique_ptr<SWAN::GUIRenderer> guiRenderer;

	float shininess = 10.0f;
	float time      = 0;
};

void Version() {
	std::cout << "SWAN-Unnamed-Demo with SWAN version " << SWAN_VERSION_STRING
	          << " (SWAN Build type: " << SWAN_BUILD_TYPE << ")\n";
}

void Usage() {
	std::cout
	    << "Usage: SWAN-Unnamed-Demo [options]\n"
	    << '\n'
	    << "Available options:\n"
	    << "    -h, --help         Show this help\n"
	    << "    -v, --version      Display version\n"
	    << "    --config=<config>  Use custom configuration file, default "
	       "is ./Config.xml\n";
}

char ConfigFile[256] = { 0 };

void ProcessArgs(int argc, char** argv) {
	for(int i = 1; i < argc; i++) {
		if(!std::strcmp(argv[i], "-h") || !std::strcmp(argv[i], "--help")) {
			Usage();
			exit(EXIT_SUCCESS);
		} else if(!std::strcmp(argv[i], "-v") ||
		          !std::strcmp(argv[i], "--version")) {
			Version();
			exit(EXIT_SUCCESS);
		} else if(!std::strncmp(argv[i], "--config=", 9)) {
			if(strlen(argv[i]) <= 9) {
				std::cout
				    << "ERROR: \"--config=\" flag was passed, but no file was "
				       "set, the flag will be ignored.\n";
				continue;
			}
			strcpy(ConfigFile, argv[i] + 9);
		} else {
			std::cout
			    << "ERROR: Unknown or unsupported flag \"" << argv[i]
			    << "\" was passed.\n"
			    << "-----------------------------------------------------------"
			       "--------------\n";
			Usage();
			exit(EXIT_FAILURE);
		}
	}
}

int main(int argc, char** argv) {
	// stbi_set_flip_vertically_on_load(true);

	if(argc > 1) {
		ProcessArgs(argc, argv);
	}

	if(!ConfigFile[0]) {
		strcpy(ConfigFile, "./Config.xml");
	}

	auto xml = SWAN::Util::ReadXML(SWAN::Util::Unquote(ConfigFile));
	auto v   = xml.findTagsWithName("Resolution");
	auto resourcesFile =
	    xml.findTagsWithName("Resources").at(0)->getAttrib("file");

	if(SWAN::Util::IsRelativePath(resourcesFile))
		resourcesFile =
		    SWAN::Util::GetDirectory(std::string(ConfigFile), true) +
		    resourcesFile;

	SWAN::Display::Init(std::stoi(v.front()->getAttrib("width")),
	                    std::stoi(v.front()->getAttrib("height")),
	                    "OGL-Engine");

	Game game(resourcesFile);

	Clock::time_point prevTime = Clock::now();
	Clock::time_point now;

	while(game.running()) {
		now = Clock::now();

		SWAN_Input.handleEvents();

		if(now - prevTime > 16ms) {
			game.update();
			game.render();

			prevTime = Clock::now();
		}
	}

	SWAN_DEBUG_PRINT("Exiting...");

	return 0;
}
