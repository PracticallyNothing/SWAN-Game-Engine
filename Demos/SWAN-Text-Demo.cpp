#define SDL_MAIN_HANDLED
#include "Core/Display.hpp"       // For SWAN::Display::*
#include "Core/EventListener.hpp" // For SWAN::EventListener, SWAN::CreateTimer()
#include "Core/Input.hpp"         // For SWAN_Input
#include "Core/Resources.hpp"     // For SWAN::Res::Get*

#include "Rendering/Text.hpp" // For SWAN::RenderText()

#include "Utility/Math.hpp"

using namespace std::chrono;
using Clock = std::chrono::steady_clock;

void ParseArgv(int argc, const char** argv);
void Usage();
void Version();

int main(int argc, const char** argv) {
	SWAN::Display::Init(1280, 720, "SWAN GUI Demo");
	SWAN_Input_Init();
	SWAN::Display::SetClearColor(0.1f, 0.3f, 0.3f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SWAN::Res::LoadFromFile("Resources/minires.xml");
	bool running = true;
	SWAN::Display::SetClearColor(0.0f, 0.3f, 0.25f, 0.0f);

	SWAN::EventListener exitEvent(
	    []() -> bool {
		    return SWAN_Input.Keyboard.escapeKey ||
		           SWAN_Input.Window.exitRequest;
		},
	    [&running]() { running = false; }, false);

	SWAN::EventListener renderEvent =
	    SWAN::CreateRepeatingTimer(16ms, [=] {
		    SWAN::RenderText(
		        0, 0,
		        "Hello, memes!",
		        SWAN::Res::GetShader("Text"),
		        SWAN::Res::GetBitmapFont("Terminus 10"),
		        SWAN::Color{ 255, 255, 255, 255 },
		        SWAN::Color{ 100, 100, 100, 255 });

		    SWAN::Display::Clear();
		});

	while(running) {
		exitEvent();
		SWAN_Input.handleEvents();
		renderEvent();
	}

	SWAN::Display::Close();

	return 0;
}
