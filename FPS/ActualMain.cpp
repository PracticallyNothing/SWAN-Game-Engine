#define SDL_MAIN_HANDLED

#include <chrono>
#include <cmath>
#include <random>
#include <set>
#include <unordered_set>

#include "SWAN/Core/Defs.hpp"
#include "SWAN/Core/Format.hpp"

// ----- Resource Loading ----- //
#include "SWAN/Core/Resources.hpp"

// ----- Video and Audio ----- //
#include "SWAN/Core/Display.hpp"       // For SWAN::Display::*
#include "SWAN/OpenAL/SoundSystem.hpp" // For SWAN::SoundSystem

// ----- Rendering stuff ----- //
#include "SWAN/Rendering/Camera.hpp"      // For SWAN::Camera
#include "SWAN/Rendering/DebugRender.hpp" // For SWAN::Render()

// ----- GUI ----- //
#include "SWAN/GUI/GUIManager.hpp"
#include "SWAN/GUI/IGUIElement.hpp"

// ----- OpenGL ----- //
#include "SWAN/OpenGL/OnGLInit.hpp" // For SWAN::OnGLInit()

// ----- Input ----- //
#include "SWAN/Input/Event.hpp"      // For SWAN::UpdateInputEvents();
#include "SWAN/Input/InputFrame.hpp" // For SWAN::InputFrame

#include "MenuState.hpp"
#include "State.hpp"

using fms = std::chrono::duration<double, std::milli>;
using FloatSeconds = std::chrono::duration<double>;

bool g_GameRunning = false;
void Stop() { g_GameRunning = false; }

void Init()
{
	// ----- Video initilazitaion ----- //
	SWAN::Display::Init(1280, 720, "Hello, world!");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Cleanup()
{
	SWAN::Display::Close();
}

State* g_CurrentState = nullptr;

int main()
{
	Init();
	SWAN::Res::ReportLoad(SWAN::Res::LoadBitmapFont("Resources/Fonts/Font.toml", "button"));

	MenuState ms;
	g_CurrentState = &ms;

	g_GameRunning = true;
	while(g_GameRunning) {
		SWAN::UpdateInputEvents();
		g_CurrentState->Update();
		g_CurrentState->Render();

		glClear(GL_DEPTH_BUFFER_BIT);
		SWAN::Display::Clear();
	}

	Cleanup();
}
