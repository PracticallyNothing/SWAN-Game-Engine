#define SDL_MAIN_HANDLED
#include "Core/Display.hpp"
#include "Core/EventListener.hpp"
#include "Core/Input.hpp"
#include "Core/Resources.hpp"

#include "Rendering/Text.hpp"

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

	guiRenderer._exp_shad = SWAN::Res::GetShader("_exp_GUIShader");

	bool running = true;

	guiRenderer._exp_add(SWAN::CreateButton(10, 10, 100, 100, BUTTON_RES, [&running] { running = false; }));
	guiRenderer._exp_add(SWAN::CreateButton(10, 150, 100, 100, BUTTON_RES, [] {}));
	guiRenderer._exp_add(SWAN::CreateButton(150, 150, 100, 100, BUTTON_RES, [] {}));

	SWAN::Color sliderBGColor{ 60, 60, 60, 0 };
	SWAN::Color sliderHandleColor{ 183, 186, 0, 0 };
	SWAN::Color sliderActiveColor{ 221, 111, 0, 0 };

	guiRenderer._exp_add(
	    SWAN::CreateVerticalSlider(
	        1000, 250,
	        50, 300,
	        sliderBGColor,
	        sliderHandleColor,
	        sliderActiveColor,
	        [](double v) {
		        SWAN::Display::SetClearColor(
		            v,
		            SWAN::Display::detail::green,
		            SWAN::Display::detail::blue,
		            1);
		    }));

	guiRenderer._exp_add(
	    SWAN::CreateSlider(
	        500, 425,
	        400, 25,
	        sliderBGColor,
	        sliderHandleColor,
	        sliderActiveColor,
	        [](double v) {
		        SWAN::Display::SetClearColor(
		            v,
		            SWAN::Display::detail::green,
		            SWAN::Display::detail::blue,
		            1);
		    }));

	guiRenderer._exp_add(
	    SWAN::CreateSlider(
	        500, 525,
	        400, 25,
	        sliderBGColor,
	        sliderHandleColor,
	        sliderActiveColor,
	        [](double v) {
		        SWAN::Display::SetClearColor(
		            SWAN::Display::detail::red,
		            v,
		            SWAN::Display::detail::blue,
		            1);
		    }));

	guiRenderer._exp_add(
	    SWAN::CreateSlider(
	        500, 625,
	        400, 25,
	        sliderBGColor,
	        sliderHandleColor,
	        sliderActiveColor,
	        [](double v) {
		        SWAN::Display::SetClearColor(
		            SWAN::Display::detail::red,
		            SWAN::Display::detail::green,
		            v,
		            1);
		    }));

	SWAN::Display::SetClearColor(0.0f, 0.3f, 0.25f, 0.0f);

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

		    SWAN::RenderText(
		        10, 10,
		        " !\"#$%&'()*+,-./"
		        "\n0123456789\n:;<=>?@\nABCDEFGHIJKLMNOPQRSTUVWXYZ\n["
		        "\\]^_`\nabcdefghijklmnopqrstuvwxyz{|}~",
		        SWAN::Res::GetShader("Text"),
		        SWAN::Res::GetBitmapFont("Monospace 16"));

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
