#define SDL_MAIN_HANDLED

#include "Core/Display.hpp"
#include "Core/EventListener.hpp"
#include "Core/Input.hpp"
#include "Core/Resources.hpp"

#include "GUI/GUIPrim.hpp"
#include "GUI/GUIRenderer.hpp"

using namespace std::chrono;
using Clock = std::chrono::steady_clock;

void ParseArgv(int argc, const char** argv);
void Usage();
void Version();

#define BUTTON_RES                               \
	SWAN::Res::GetTexture("Normal Button"),      \
	    SWAN::Res::GetTexture("Hovered Button"), \
	    SWAN::Res::GetTexture("Pressed Button")

int main(int argc, const char** argv) {
	SWAN::Display::Init(1280, 720, "SWAN GUI Demo");
	SWAN_Input_Init();
	SWAN::Display::SetClearColor(0.1f, 0.3f, 0.3f, 0.0f);

	SWAN::Res::LoadFromFile("Resources/res.xml");
	SWAN::GUIRenderer guiRenderer;

	bool running = true;

	guiRenderer.add(SWAN::CreateButton(10, 10, 100, 100, BUTTON_RES, [&running] { running = false }));
	guiRenderer.add(SWAN::CreateButton(10, 150, 100, 100, BUTTON_RES, [] {}));
	guiRenderer.add(SWAN::CreateButton(150, 150, 100, 100, BUTTON_RES, [] {}));

	guiRenderer.add(
	    SWAN::CreateSlider(
	        500, 500,
	        300, 75,
	        SWAN::Color{ 100, 25, 68, 0 },
	        SWAN::Color{ 150, 75, 118, 0 },
	        SWAN::Color{ 30, 120, 120, 0 },
	        [](double v) {}));

	SWAN::Display::SetClearColor(0.0f, 0.3f, 0.25f, 0.0f);

	SWAN::EventListener exitEvent(
	    []() -> bool {
		    return SWAN_Input.Keyboard.escapeKey ||
		           SWAN_Input.Window.exitRequest;
		},
	    [&running]() { running = false; }, false);

	SWAN::EventListener renderEvent =
	    SWAN::CreateRepeatingTimer(16ms, [&guiRenderer] {
		    guiRenderer.update();
		    guiRenderer.render();

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
