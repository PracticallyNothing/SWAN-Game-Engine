#define SDL_MAIN_HANDLED
#include "Core/Display.hpp"       // For SWAN::Display::*
#include "Core/EventListener.hpp" // For SWAN::EventListener, SWAN::CreateTimer()
#include "Core/Input.hpp"         // For SWAN_Input
#include "Core/Resources.hpp"     // For SWAN::Res::Get*

//#include "Rendering/Text.hpp" // For SWAN::RenderText()

#include "GUI/Element.hpp"  // For SWAN::GUI::Element
#include "GUI/Renderer.hpp" // For SWAN::GUI::Renderer

#include "Utility/Math.hpp"

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
  SWAN::
	SWAN::Display::Init(1280, 720, "SWAN GUI Demo");
	SWAN_Input_Init();
	SWAN::Display::SetClearColor(0.1f, 0.3f, 0.3f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	SWAN::Res::LoadFromFile("Resources/minires.xml");

	SWAN::GUI::Renderer guiRenderer(SWAN::Res::GetShader("GUI"));

	bool running = true;

	guiRenderer.add(SWAN::GUI::CreateButton(10, 10, 100, 100, BUTTON_RES, [&running] { running = false; }));
	guiRenderer.add(SWAN::GUI::CreateButton(10, 150, 100, 100, BUTTON_RES, [] {}))->setVisible(false);
	guiRenderer.add(SWAN::GUI::CreateButton(150, 150, 100, 100, BUTTON_RES, [] {}));

	SWA

	SWAN::Color sliderBGColor{ 60, 60, 60, 255 };
	SWAN::Color sliderHandleColor{ 183, 186, 0, 255 };
	SWAN::Color sliderActiveColor{ 221, 111, 0, 255 };

	int textX = 0, textY = 0;

	guiRenderer.add(
	    SWAN::GUI::CreateSlider(
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

	guiRenderer.add(
	    SWAN::GUI::CreateSlider(
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

	guiRenderer.add(
	    SWAN::GUI::CreateSlider(
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
	    SWAN::CreateRepeatingTimer(16ms, [=, &textX, &textY, &guiRenderer] {
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
