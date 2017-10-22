#include "Core/Display.hpp"
#include "Core/Resources.hpp"
#include "Core/Input.hpp"

#include "GUI/GUIPrim.hpp"
#include "GUI/GUIRenderer.hpp"

int main(){
	SWAN::Display::Init(1280, 720, "SWAN Pong Demo");

	SWAN_Input_Init();

	bool running = true;

	SWAN::Res::LoadFromFile("Pong/res.xml");
	SWAN::GUIRenderer ren;

	while (running) {
		ren.update();
		ren.render();

		SWAN::Display::Clear();
	}
}
