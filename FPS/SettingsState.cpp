#include "SettingsState.hpp"
#include "ExitState.hpp"
#include "SWAN/Core/Display.hpp"
#include "SWAN/Core/Logging.hpp"
#include "SWAN/Core/Resources.hpp"

SettingsState::SettingsState(MenuState* state)
{
	int scrW = SWAN::Display::GetWidth();
	int scrH = SWAN::Display::GetHeight();

	SWAN::Log("Adding buttons...");
	AddButton("Play", [] { SWAN::Log("TIME TO PLAY"); });
	AddButton("Settings", [] { SWAN::Log("Set me up!"); });
	AddButton("Editor", [] { SWAN::Log("EDIT ME, BITCH"); });
	AddButton("Quit", [] { SWAN::Log("Bye bye."); g_CurrentState = new ExitState(); });

	gui.SetExtraInputFrame(this);
	SWAN::SetCurrentInputFrame(&gui);
}

SettingsState::~SettingsState()
{
	for(Button* b : buttons)
		delete b;
}

void SettingsState::AddButton(SWAN::String name,
                          std::function<void(void)> action)
{
	int scrW = SWAN::Display::GetWidth();
	int scrH = SWAN::Display::GetHeight();

	int totalBtnH = btnH * (buttons.size() + 1) + btnPad * buttons.size();

	Button* b = new Button(scrW / 2 - btnW / 2,
	                       scrH / 2 - totalBtnH / 2,
	                       btnW, btnH);
	b->SetText(SWAN::Res::GetBitmapFont("button"), name);
	b->SetAction(action);

	buttons.push_back(b);
	gui.AddElement(b);

	PositionButtons();
}

void SettingsState::OnWindowExit()
{
	g_CurrentState = new ExitState();
}

void SettingsState::Update()
{
}

void SettingsState::PositionButtons()
{
	int scrW = SWAN::Display::GetWidth();
	int scrH = SWAN::Display::GetHeight();

	int totalBtnH = btnH * buttons.size() + btnPad * (buttons.size()-1);

	for(int i = 0; i < buttons.size(); i++) {
		buttons[i]->SetPosition(scrW / 2 - btnW / 2,
		                        scrH / 2 - totalBtnH / 2 + i * (btnH + btnPad));
	}
}

void SettingsState::Render()
{
	int scrW = SWAN::Display::GetWidth();
	int scrH = SWAN::Display::GetHeight();

	int totalBtnH = btnH * buttons.size() + btnPad * (buttons.size()-1);

	gui.RenderRect2D(
	    SWAN::Rect2D(
	        scrW / 2 - btnW / 2 - 10,
	        scrH / 2 - totalBtnH / 2 - 10,
	        btnW + 20, totalBtnH + 20, { 0.7, 0.3, 0.3, 1 }));
	gui.RenderElements();
}
