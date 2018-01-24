#include "Core/Display.hpp"   // For SWAN::Display
#include "Core/Input.hpp"     // For SWAN_Input
#include "Core/Resources.hpp" // For SWAN::Res::LoadFromFile(), SWAN::Res::Get*()
#include "Core/EventListener.hpp" // For SWAN::EventListener

#include "Rendering/Text.hpp" // For SWAN::RenderText()

#include <iostream> // For std::cout
#include <chrono>   // For std::chrono::*

using namespace std::chrono;
using std::cout;

typedef duration<float, milliseconds::period> fms;

int main() {
	SWAN::Display::Init(640, 480, "SWAN-FPS");
	SWAN::Display::SetClearColor(0.5f, 0.3f, 0.25f, 0.0f);
	SWAN_Input_Init();

	SWAN::Res::LoadFromFile("../Resources/minires.xml");

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

		auto now = steady_clock::now();

		if(now - prevTime >= 16ms) {
		    SWAN::RenderText(
		        0, 0,
		        std::string("Frame time: ") +
					std::to_string(duration_cast<fms>(frameTime).count()) + " ms\nFPS: " +
					std::to_string((1000 / duration_cast<fms>(frameTime).count())),
		        SWAN::Res::GetShader("Text"),
		        SWAN::Res::GetBitmapFont("Terminus 10"),
		        SWAN::Color{ 255, 255, 255, 255 },
		        SWAN::Color{ 0, 0, 0, 255 });

			SWAN::Display::Clear();

			now = steady_clock::now();
			frameTime = now - prevTime;
			prevTime = now;
		}
	}

	SWAN::Display::Close();

	return 0;
}
