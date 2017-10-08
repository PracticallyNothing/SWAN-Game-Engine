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

struct GUISkin {
	const SWAN::Texture
		*sliderBGTex = nullptr,
		*sliderHandleTex = nullptr;
	// What percentage of the width should the slider handle take up?
	float sliderHandleWidthPart;
	// What percentage of the height should the background image take up?
	float sliderBGHeightPart;
};

class Slider : public SWAN::GUIP::IElement {
	public:
		Slider(GUISkin skin) {
			bg     = new SWAN::GUIP::Image     (skin.sliderBGTex);
			handle = new SWAN::GUIP::Draggable (skin.sliderHandleTex, false, true);
			this->skin = skin;
		}

		IElement* resizeTo(int w, int h){
			this->w = w;
			this->h = h;

			bg->resizeTo(w, h * skin.sliderBGHeightPart);
			handle->resizeTo(w * skin.sliderHandleWidthPart, h);

			return this;
		}

	private:
		GUISkin skin;
		SWAN::GUIP::Image* bg;
		SWAN::GUIP::Draggable* handle;
		float min, max;
};

int main (int argc, const char** argv) {
	SWAN::Display::Init(1280, 720, "SWAN GUI Demo");
	SWAN_Input_Init();
	SWAN::Display::SetClearColor(0.1f, 0.3f, 0.3f, 0.0f);

	SWAN::Res::LoadFromFile("Resources/res.xml");
	SWAN::GUIRenderer guiRenderer;

	guiRenderer.add(new SWAN::GUIP::Button(
		SWAN::Res::GetTexture("Flat Blue"),
		SWAN::Res::GetTexture("Flat Red"),
		SWAN::Res::GetTexture("Flat Green")
	)) -> moveTo(500, 500) -> resizeTo(200, 125);

	guiRenderer.add(new SWAN::GUIP::Button(
		SWAN::Res::GetTexture("Flat Blue"),
		SWAN::Res::GetTexture("Flat Red"),
		SWAN::Res::GetTexture("Flat Green")
	)) -> moveTo(250, 500) -> resizeTo(200, 125);

	guiRenderer.add(new SWAN::GUIP::Button(
		SWAN::Res::GetTexture("Flat Blue"),
		SWAN::Res::GetTexture("Flat Red"),
		SWAN::Res::GetTexture("Flat Green")
	)) -> moveTo(750, 500) -> resizeTo(200, 125);

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
