#include "Core/Display.hpp"
#include "Core/Input.hpp"
#include "Core/Resources.hpp"

#include "GUI/GUIPrim.hpp"
#include "GUI/GUIRenderer.hpp"

int main() {
	SWAN::Display::Init(1280, 720, "SWAN Pong Demo");

	SWAN_Input_Init();

	SWAN::Res::LoadFromFile("Pong/res.xml");
	SWAN::GUIRenderer ren;

	ren._exp_shad = SWAN::Res::GetShader("GUI");

	bool running = true;

	SWAN::EventListener exitEvent(
	    []() -> bool {
		    return SWAN_Input.Keyboard.escapeKey ||
		           SWAN_Input.Window.exitRequest;
		},
	    [&running]() { running = false; }, false);

	SWAN::EventListener renderEvent =
	    SWAN::CreateRepeatingTimer(16ms, [&guiRenderer] {
		    guiRenderer._exp_update();
		    guiRenderer._exp_render();

		    SWAN::Display::Clear();
		});

	while(running) {
		exitEvent();
		SWAN_Input.handleEvents();
		renderEvent();
	}
}
