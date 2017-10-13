#define SDL_MAIN_HANDLED
#include <chrono>

#include "Core/Display.hpp"
#include "Core/Input.hpp"
#include "Core/Resources.hpp"

#include "GUI/GUIPrim.hpp"
#include "GUI/GUIRenderer.hpp"

using namespace std::chrono;
using Clock = std::chrono::steady_clock;

void ParseArgv (int argc, const char** argv);
void Usage     ();
void Version   ();

#define BUTTON_RES \
	SWAN::Res::GetTexture("Normal Button"), SWAN::Res::GetTexture("Hovered Button"), SWAN::Res::GetTexture("Pressed Button")

int main (int argc, const char** argv) {
	SWAN::Display::Init(1280, 720, "SWAN GUI Demo");
	SWAN_Input_Init();
	SWAN::Display::SetClearColor(0.1f, 0.3f, 0.3f, 0.0f);

	SWAN::Res::LoadFromFile("Resources/res.xml");
	SWAN::GUIRenderer guiRenderer;

	guiRenderer.add(new SWAN::GUIP::Button(BUTTON_RES))->moveTo(500, 500)->resizeTo(150, 150);
	guiRenderer.add(new SWAN::GUIP::Button(BUTTON_RES))->moveTo(250, 500)->resizeTo(150, 150);
	guiRenderer.add(new SWAN::GUIP::Button(BUTTON_RES))->moveTo(750, 500)->resizeTo(150, 150);

	SWAN::GUIP::Draggable* dr =
		(SWAN::GUIP::Draggable*) guiRenderer.add(
				new SWAN::GUIP::Draggable(
					SWAN::Res::GetTexture("Flat Cyan"),
					false,
					true
				)
		) -> resizeTo(50, 20) -> setMinX(100) -> setMaxX(1000);

	Clock::time_point prevTime = Clock::now();
	Clock::time_point now;

	SWAN::Display::SetClearColor(0.0f, 0.3f, 0.25f, 0.0f);

	while(!(SWAN_Input.Keyboard.escapeKey || SWAN_Input.Window.exitRequest)){
		now = Clock::now();

		SWAN_Input.handleEvents();

		if (now - prevTime > 16ms) {
			guiRenderer.update();
			guiRenderer.render();

			SWAN::Display::Clear();

			prevTime = Clock::now();
		}
	}

	SWAN::Display::Close();

	return 0;
}
