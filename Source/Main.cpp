#include "Core/Display.hpp" // For Display
#include "Core/Input.hpp"   // For Input

#include "Utility/Debug.hpp"   // For DEBUG_OUT()
#include "Utility/Math.hpp"    // For Util::pixelToGLCoord()
#include "Utility/Profile.hpp" // For Util::CurrentProfile, UTIL_PROFILE()

#include "Rendering/Camera.hpp"		// For Camera
#include "Rendering/Mesh.hpp"		// For Mesh
#include "Rendering/Shader.hpp"		// For Shader
#include "Rendering/Texture.hpp"	// For Texture
#include "Rendering/OBJ-Import.hpp"	// For Import::OBJ()
#include "Rendering/Light.hpp"		// For DirectionalLight, PointLight
#include "Rendering/BitmapFont.hpp" // For BitmapFont
#include "Rendering/Text.hpp"       // For Text

#define STBI_IMPLEMENTATION
#include "External/stb_image.h" // For stbi_set_flip_vertically_on_load()
#undef STBI_IMPLEMENTATION

#include "Logic/Entity.hpp" // For Entity

#include <iostream>	// For std::cout
#include <memory>	// For std::unique_ptr<T>, std::make_unique<T>()
#include <chrono>	// For std::chrono::milliseconds, std::chrono::steady_clock, std::chrono::operator""ms

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
		explicit Game(DisplayConfig conf = {1280, 720, "OpenGL-CPP-Engine"})
			: display(conf.w, conf.h, conf.title), 
			  run(true),
			  font("./Resources/Font.toml")
		{	
			UTIL_PROFILE();
			Display::setClearColor(0.0f, 0.3f, 0.25f, 0.0f);
			// SDL_SetRelativeMouseMode(SDL_TRUE);
			Input_init();

			tex = make_unique<Texture>("./Resources/Textures/Diffuse_Color.png");
			planeMesh 	  = Import::OBJ("./Resources/Meshes/SimplePlane.obj", Import::Settings{false});
			shotgunMesh = Import::OBJ("./Resources/Meshes/SPAS-12.obj", Import::Settings{true});
			cam = make_unique<Camera>(display.getAspect(), glm::vec3(0.0f, 0.0f, -1.0f));
			torusMesh = Import::OBJ("./Resources/Meshes/Torus.obj", Import::Settings{true});
			// ent.setCurrAngVel(glm::vec3(0.0, 0.0, 10.0));
			
			DirectionalLight dl0;
			dl0.direction = glm::normalize(glm::vec3(1, 1, 1));
			dl0.ambient   = glm::vec3(0.003, 0.0, 0.003);
			dl0.diffuse   = glm::vec3(0.0, 0.0, 0.3);
			dl0.specular  = glm::vec3(1, 1, 1);

			PointLight pl1;
			pl1.position     = glm::vec3(1, 7, 15);
			pl1.ambient      = glm::vec3(0.05, 0.0005, 0.0);
			pl1.diffuse      = glm::vec3(0.5, 0.005, 0.0);
			pl1.specular     = glm::vec3(1, 1, 1);
			pl1.linearAtt    = 0.045;
			pl1.quadraticAtt = 0.0075;

			plane.getTransform_ref().pos = pl1.position;

			shader.compileShaders("./Resources/Shaders/proper.vert",
								  "./Resources/Shaders/proper.frag");
			{
				shader.addAttrib("pos");  // GL Error: 1281
				shader.addAttrib("UV");   // GL Error: 1281
				shader.addAttrib("norm"); // GL Error: 1281
			}
			shader.linkShaders();
			
			shader.use();
			{
				shader.addUniform("transform");
				shader.addUniform("perspective");
				shader.addUniform("view");
				
				shader.addUniform("activeLights");

				shader.addUniform("camPos");

				shader.addUniform("matSpecular");
				shader.addUniform("matShininess");

				shader.setUniformData("matSpecular", glm::vec3(0.5, 0.5, 0.5));
				shader.setUniformData("matShininess", 2.0f);
				
				shader.addUniform("lights[0].type");
				shader.addUniform("lights[0].ambient");
				shader.addUniform("lights[0].diffuse");
				shader.addUniform("lights[0].specular");
				shader.addUniform("lights[0].pos");
				shader.addUniform("lights[0].dir");
				shader.addUniform("lights[0].cutoff");
				shader.addUniform("lights[0].outerCutoff");
				shader.addUniform("lights[0].constAtt");
				shader.addUniform("lights[0].linAtt");
				shader.addUniform("lights[0].quadAtt");
				
				shader.addUniform("lights[1].type");
				shader.addUniform("lights[1].ambient");
				shader.addUniform("lights[1].diffuse");
				shader.addUniform("lights[1].specular");
				shader.addUniform("lights[1].pos");
				shader.addUniform("lights[1].dir");
				shader.addUniform("lights[1].cutoff");
				shader.addUniform("lights[1].outerCutoff");
				shader.addUniform("lights[1].constAtt");
				shader.addUniform("lights[1].linAtt");
				shader.addUniform("lights[1].quadAtt");
				
				shader.setUniformData("lights[0]", dl0);
				shader.setUniformData("lights[1]", pl1);
				shader.setUniformData("activeLights", 2);
			}
			shader.unuse();
			
			textShader.compileShaders("./Resources/Shaders/text.vert",
								  	  "./Resources/Shaders/text.frag");
			{
				textShader.addAttrib("pos");  // GL Error: 1281
				textShader.addAttrib("UV");
			}
			textShader.linkShaders();

			textShader.use();
			{
				textShader.addUniform("model");
				textShader.addUniform("color");
			}
			textShader.unuse();
			
			TextConfig errorText;
			errorText.text = std::string("ERROR: ");
			errorText.font = &font;
			errorText.color = glm::vec3(1, 0.1, 0.1);
			errorText.bold = true;
			errorText.italics = false;
			
			TextConfig tc;
			tc.text = std::string("Hello World\nWoot! Multiline text!\nSweet!");
			tc.font = &font;
			tc.color = glm::vec3(1, 1, 1);
			tc.bold = true;
			tc.italics = true;

			txt.set(display, errorText, false);
			txt.set(display, tc, true);

			plane.setMesh(planeMesh.get());
			plane.setTexture(tex.get());

			shotgun.setMesh(shotgunMesh.get());
			shotgun.setTexture(tex.get());
		
			// ----- GL enable stuff -----
			glEnable(GL_PROGRAM_POINT_SIZE);
			glEnable(GL_DEPTH_TEST);
			
			//glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		~Game() {}

		void update() {
			auto start = Clock::now();
	
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

			if (Input.Keyboard.letterKeys['y' - 'a'])
				std::cout << "Camera FOV: " << (cam->fov -= glm::radians(1.0))
					<< '\n';
			if (Input.Keyboard.letterKeys['h' - 'a'])
				std::cout << "Camera FOV: " << (cam->fov += glm::radians(1.0))
					<< '\n';

			auto mouseX 	= Util::pixelToGLCoord(display.getW(), Input.Mouse.x);
			auto prevMouseX = Util::pixelToGLCoord(display.getW(), PrevInput.Mouse.x);

			cam->rotateByY(Util::Radians((mouseX - prevMouseX) * 2.0));
			
			auto mouseY 	= Util::pixelToGLCoord(display.getH(), Input.Mouse.y);
			auto prevMouseY = Util::pixelToGLCoord(display.getH(), PrevInput.Mouse.y);
			
			cam->rotateByX(Util::Radians((mouseY - prevMouseY) * 2.0));
			
			//DEBUG_OUT(Util::Degrees::fromRadians(cam->getTransform_ref().rot.x));
			//DEBUG_OUT(Util::Degrees::fromRadians(cam->getTransform_ref().rot.y));
			//DEBUG_OUT(Util::Degrees::fromRadians(cam->getTransform_ref().rot.z));
			//plane.getTransform_ref().rot.x += 0.01f;
			
			plane.getTransform_ref().pos.x = std::sin(time) * 10;
			//plane.getTransform_ref().pos.y = std::cos(time) * 10;
			//plane.getTransform_ref().pos.z = std::sin(time) * std::cos(time) * 10;
			
			//time += 0.01f;

			PrevInput = Input;

			updateTime = duration_cast<Ms>(Clock::now() - start);
		}

		void render() {
			shader.use();
			{
				if(Input.Keyboard.letterKeys['p' - 'a']){
					shininess += 2;
					DEBUG_OUT(shininess);
				} else if (Input.Keyboard.letterKeys['o' - 'a']) {
					shininess -= 2;
					DEBUG_OUT(shininess);
				}
				
				if(shininess < 0)
					shininess = 0;

				shader.setUniformData("matShininess", shininess);
				
				shader.setUniformData("camPos", cam->getPos());
				shader.setUniformData("view", cam->getView());
				shader.setUniformData("perspective", cam->getPerspective());

				shotgun.useTransform(&shader);
				shotgun.render();
					
				plane.useTransform(&shader);
				plane.render();
				plane.getMesh()->renderWireframe();
				plane.getMesh()->renderVerts();
			}
			shader.unuse();

			txt.render(&textShader);
			
			glClear(GL_DEPTH_BUFFER_BIT);
			display.clear();
		}

		bool running() { return run; }

	private:
		Display display;
		bool run;

		_input PrevInput;

		Shader shader;
		Entity plane, shotgun,
			   xTorus, yTorus, zTorus;
		
		unique_ptr<Mesh> planeMesh, shotgunMesh, torusMesh;
		unique_ptr<Texture> tex;
		unique_ptr<Camera> cam;
		
		Text txt;
		Shader textShader;
		BitmapFont font;

		float shininess = 10.0f;
		float time = 0;
		Ms renderTime = 0ms, updateTime = 0ms;
};

int main(/*int argc, char** argv*/) {
	//stbi_set_flip_vertically_on_load(true);
	Game game;

	while (game.running()) {
		game.update();
		game.render();
	}

	return 0;
}