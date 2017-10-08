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
	const Texture
		*sliderBGTex = nullptr,
		*sliderHandleTex = nullptr;
	// What percentage of the width should the slider handle take up?
	float sliderHandleWidthPart;
	// What percentage of the height should the background image take up?
	float sliderBGHeightPart;
};

class Slider : public GUIPrim::IElement {
	public:
		Slider(GUISkin skin) {
			bg     = new GUIPrim::Image     (skin.sliderBGTex);
			handle = new GUIPrim::Draggable (skin.sliderHandleTex, false, true);
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
		GUIPrim::Image* bg;
		GUIPrim::Draggable* handle;
		float min, max;
};

int main (int argc, const char** argv) {
	Display::Init(1280, 720, "SWAN GUI Demo");
	Input_init();
	Display::SetClearColor(0.1f, 0.3f, 0.3f, 0.0f);

	Resources::LoadFromFile("Resources/res.xml");
	GUIRenderer guiRenderer;

	guiRenderer.add(new GUIPrim::Button(
		Resources::GetTexture("Flat Blue"),
		Resources::GetTexture("Flat Red"),
		Resources::GetTexture("Flat Green")
	)) -> moveTo(500, 500) -> resizeTo(200, 125);

	guiRenderer.add(new GUIPrim::Button(
		Resources::GetTexture("Flat Blue"),
		Resources::GetTexture("Flat Red"),
		Resources::GetTexture("Flat Green")
	)) -> moveTo(250, 500) -> resizeTo(200, 125);

	guiRenderer.add(new GUIPrim::Button(
		Resources::GetTexture("Flat Blue"),
		Resources::GetTexture("Flat Red"),
		Resources::GetTexture("Flat Green")
	)) -> moveTo(750, 500) -> resizeTo(200, 125);

	GUIPrim::Draggable* dr =
		(GUIPrim::Draggable*) guiRenderer.add(
				new GUIPrim::Draggable(
					Resources::GetTexture("Flat Cyan"),
					false,
					true
				)
		) -> resizeTo(50, 20) -> setMinX(100) -> setMaxX(1000);

	Clock::time_point prevTime = Clock::now();
	Clock::time_point now;

	while(!(Input.Keyboard.escapeKey || Input.Window.exitRequest)){
		now = Clock::now();

		Input.handleEvents();

		if (now - prevTime > 16ms) {
			guiRenderer.update();
			guiRenderer.render();

			Display::Clear();

			prevTime = Clock::now();
		}
	}

	Display::Close();

	return 0;
}
