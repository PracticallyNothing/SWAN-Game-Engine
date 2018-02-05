#define SDL_main_h_ // SDL_Main might be very useful, but I find it to be pesky.

#include "Core/Display.hpp"       // For SWAN::Display
#include "Core/EventListener.hpp" // For SWAN::EventListener
#include "Core/Input.hpp"         // For SWAN_Input
#include "Core/Resources.hpp"     // For SWAN::Res::LoadFromFile(), SWAN::Res::Get*()

#include "Physics/Transform.hpp" // For SWAN::Transform
#include "Rendering/Camera.hpp"  // For SWAN::Camera
#include "Rendering/Mesh.hpp"    // For SWAN::Mesh
#include "Rendering/Shader.hpp"  // For SWAN::Shader
#include "Rendering/Text.hpp"    // For SWAN::RenderText()
#include "Rendering/Texture.hpp" // For SWAN::Texture

#include "Utility/AngleUnits.hpp" // For SWAN::Radians, SWAN::Degrees
#include "Utility/Math.hpp"       // For SWAN::Util::PixelToGLCoord()

#include <chrono>   // For std::chrono::*
#include <iostream> // For std::cout
#include <sstream>  // For std::stringstream
#include <string>   // For std::string
#include <vector>   // For std::vector<T>

using namespace std::chrono;
using std::cout;
using std::vector;

using SWAN::Util::Radians;

typedef duration<float, milliseconds::period> fms;

int main() {
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

	SWAN::Shader* unlitShader = SWAN::Res::GetShader("Unlit");

	const SWAN::Mesh* placeMesh   = SWAN::Res::GetMesh("Place");
	const SWAN::Texture* placeTex = SWAN::Res::GetTexture("Place");

	const SWAN::Mesh* planeMesh = SWAN::Res::GetMesh("Plane");

	const SWAN::Mesh* shotgunMesh   = SWAN::Res::GetMesh("SPAS 12");
	const SWAN::Texture* shotgunTex = SWAN::Res::GetTexture("SPAS 12");

	SWAN::Camera cam(SWAN::Display::GetAspectRatio(), glm::vec3(0.0f, 0.0f, 0.0f));

	SWAN::Transform transf;
	transf.pos.x = -5.0;
	transf.pos.y = -5.0;
	transf.pos.z = 5.0;
	transf.rot.y -= M_PI / 2;
	transf.setParent(&cam.transform);

	SWAN::Transform transf2;
	transf.setParent(&transf2);

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

	while(running) {
		exitEvent();
		SWAN_Input.handleEvents();

		float speed = 0.0001f;

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

		float sensitivity = 2.0;

		auto mouseX     = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetCurrMouseState().x);
		auto prevMouseX = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetPrevMouseState().x);
		cam.rotateByY(SWAN::Util::Radians((mouseX - prevMouseX) * sensitivity));

		auto mouseY     = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetCurrMouseState().y);
		auto prevMouseY = SWAN::Util::PixelToGLCoord(SWAN::Display::GetHeight(), SWAN::GetPrevMouseState().y);
		cam.rotateByX(-SWAN::Util::Radians((mouseY - prevMouseY) * sensitivity));

		auto now = steady_clock::now();

		if(now - prevTime >= 16ms) {
			transf2.rot.x += 0.1;
			std::vector<SWAN::ShaderUniform> unis = {
				{ "transform", transf },
				{ "view", cam.getView() },
				{ "perspective", cam.getPerspective() }
			};
			unlitShader->setUniforms(unis);

			shotgunTex->bind();
			unlitShader->renderMesh(*shotgunMesh);

			unlitShader->setUniform({ "transform", transf2 });
			placeTex->bind();
			unlitShader->renderMesh(*placeMesh);

			std::stringstream info;
			info
			    << "Frame time: " << duration_cast<fms>(frameTime).count() << " ms\n"
			    << "FPS: " << 1000 / duration_cast<fms>(frameTime).count() << '\n'
			    << "Camera:  " << cam.transform << '\n'
			    << "Transf:  " << transf << '\n'
			    << "Transf2: " << transf2 << '\n';

			glClear(GL_DEPTH_BUFFER_BIT);
			SWAN::RenderText(
			    0, 0,
			    info.str(),
			    SWAN::Res::GetShader("Text"),
			    SWAN::Res::GetBitmapFont("Terminus 10"),
			    SWAN::Color{ 255, 255, 255, 255 },
			    SWAN::Color{ 0, 0, 0, 255 });

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
