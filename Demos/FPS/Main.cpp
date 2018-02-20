#define SDL_main_h_ // SDL_Main might be very useful, but I find it to be pesky.

#include "Core/Display.hpp"       // For SWAN::Display
#include "Core/EventListener.hpp" // For SWAN::EventListener
#include "Core/Input.hpp"         // For SWAN_Input
#include "Core/Resources.hpp"     // For SWAN::Res::LoadFromFile(), SWAN::Res::Get*()

#include "Physics/Transform.hpp" // For SWAN::Transform
#include "Rendering/Camera.hpp"  // For SWAN::Camera
#include "Rendering/Mesh.hpp"    // For SWAN::Mesh
#include "Rendering/Shader.hpp"  // For SWAN::Shader
#include "Rendering/Text.hpp"    // For SWAN::Text
#include "Rendering/Texture.hpp" // For SWAN::Texture

#include "Utility/AngleUnits.hpp" // For SWAN::Radians, SWAN::Degrees
#include "Utility/Debug.hpp"      // For SWAN_DEBUG_DO(), SWAN_DEBUG_VAR
#include "Utility/Math.hpp"       // For SWAN::Util::PixelToGLCoord()

#include <algorithm> // For std::generate()
#include <chrono>    // For std::chrono::*
#include <iostream>  // For std::cout
#include <map>       // For std::map<K, V>
#include <random>    // For std::random_device
#include <sstream>   // For std::stringstream
#include <string>    // For std::string
#include <vector>    // For std::vector<T>

using namespace std::chrono;
using std::cout;
using std::map;
using std::vector;

using SWAN::Util::Radians;

typedef duration<float, milliseconds::period> fms;

std::random_device rd;
SWAN::Transform getRandTransform() {
	std::uniform_real_distribution<double> d(-75, 75);
	std::uniform_real_distribution<double> d2(1, 7.5);

	return SWAN::Transform(
	    glm::vec3(d(rd), d(rd), d(rd)),
	    glm::vec3(d(rd), d(rd), d(rd)));
}

void _CheckGLError(std::string funcCall) {
	cout << "    Checking GL errors...\n";
	map<GLenum, std::string> errors = {
		{ 0x0500, "GL_INVALID_ENUM" },
		{ 0x0501, "GL_INVALID_VALUE" },
		{ 0x0502, "GL_INVALID_OPERATION" },
		{ 0x0503, "GL_STACK_OVERFLOW" },
		{ 0x0504, "GL_STACK_UNDERFLOW" },
		{ 0x0505, "GL_OUT_OF_MEMORY" },
		{ 0x0506, "GL_INVALID_FRAMEBUFFER_OPERATION" },
		{ 0x0507, "GL_CONTEXT_LOST" }
	};

	GLenum err;
	while((err = glGetError()) != GL_NO_ERROR) {
		cout << "OpenGL error:\n"
		     << "    type: " << errors[err] << '\n'
		     << "    func: " << funcCall << '\n';
	}
}

void Init() {
	SWAN::Display::Init(1280, 720, "SWAN-FPS");
	SWAN::Display::SetClearColor(0.5f, 0.3f, 0.25f, 0.0f);
	SWAN_Input_Init();

	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SWAN::Res::LoadFromFile("Resources/res.xml");
}

void CamControl(SWAN::Camera& cam, double speed = 0.001, double sensitivity = 2.0) {
	if(SWAN_Input.Keyboard.letterKeys['w' - 'a'])
		cam.moveForw(speed);
	else if(SWAN_Input.Keyboard.letterKeys['s' - 'a'])
		cam.moveForw(-speed);

	if(SWAN_Input.Keyboard.letterKeys['a' - 'a'])
		cam.moveRight(-speed);
	else if(SWAN_Input.Keyboard.letterKeys['d' - 'a'])
		cam.moveRight(speed);

	if(SWAN_Input.Keyboard.spaceKey)
		cam.moveUp(speed);
	else if(SWAN_Input.Keyboard.LShiftKey)
		cam.moveUp(-speed);

	auto mouseX     = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetCurrMouseState().x);
	auto prevMouseX = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetPrevMouseState().x);
	cam.rotateByY(SWAN::Util::Radians((mouseX - prevMouseX) * sensitivity));

	auto mouseY     = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetCurrMouseState().y);
	auto prevMouseY = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetPrevMouseState().y);
	cam.rotateByX(-SWAN::Util::Radians((mouseY - prevMouseY) * sensitivity));
}

int main() {
	Init();

	SWAN::Shader* unlitShader = SWAN::Res::GetShader("Unlit");
	SWAN::Shader* fogShader   = SWAN::Res::GetShader("Fog");
	SWAN::Shader* shader      = fogShader;

	const SWAN::Mesh* cubeMesh    = SWAN::Res::GetMesh("1x1 Cube");
	const SWAN::Texture* placeTex = SWAN::Res::GetTexture("Place");

	SWAN::Camera cam(SWAN::Display::GetAspectRatio(), glm::vec3(0.0f, 0.0f, 0.0f));
	SWAN::

	    bool running = true;
	SWAN::EventListener exitEvent(
	    []() -> bool {
		    return SWAN_Input.Keyboard.escapeKey ||
		           SWAN_Input.Window.exitRequest;
	    },
	    [&running]() { running = false; }, false);

	auto prevTime = steady_clock::now();

	// Hacky way to set type to duration
	auto frameTime = prevTime - prevTime;

	int nCubes = 1000;
	std::vector<SWAN::Transform> transforms(nCubes);
	std::generate(transforms.begin(), transforms.end(), getRandTransform);

	while(running) {
		exitEvent();
		SWAN_Input.handleEvents();

		CamControl(cam);

		auto now = steady_clock::now();

		SWAN::Text text("", SWAN::Res::GetBitmapFont("Monospace 12"));

		if(now - prevTime >= 16ms) {
			std::vector<SWAN::ShaderUniform> unis = {
				{ "view", cam.getView() },
				{ "perspective", cam.getPerspective() },
				{ "fogColor", glm::vec3(SWAN::Display::GetClearColor()) },
				{ "fogZ", 0.85f * cam.zFar }
			};
			shader->setUniforms(unis);

			placeTex->bind();

			int nRenderedCubes = 0;

			for(int i = 0; i < nCubes; i++) {
				if(glm::length(transforms[i].pos - cam.transform.pos) <= cam.zFar) {
					shader->setUniform({ "transform", transforms[i] });
					shader->renderMesh(*cubeMesh);
					nRenderedCubes++;
				}
			}

			std::stringstream info;
			info
			    << "Frame time: " << duration_cast<fms>(frameTime).count() << " ms\n"
			    << "FPS: " << 1000 / duration_cast<fms>(frameTime).count() << '\n'
			    << "# rendered cubes: " << nRenderedCubes << " out of " << nCubes << '\n';

			text.text = info.str();
			text.updateVAO();

			glClear(GL_DEPTH_BUFFER_BIT);
			text.render(SWAN::Res::GetShader("Text"), 0, 0, glm::vec4(1, 1, 1, 1));

			glClear(GL_DEPTH_BUFFER_BIT);
			SWAN::Display::Clear();

			now       = steady_clock::now();
			frameTime = now - prevTime;
			prevTime  = now;
			SWAN::SetMousePos(SWAN::Display::detail::width / 2, SWAN::Display::detail::height / 2);
		}
	}

	SWAN::Display::Close();

	return 0;
}
