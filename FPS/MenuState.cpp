#include "MenuState.hpp"
#include "ExitState.hpp"
#include "SWAN/Core/Display.hpp"
#include "SWAN/Core/Logging.hpp"
#include "SWAN/Core/Resources.hpp"

MenuState::MenuState()
{
	int scrW = SWAN::Display::GetWidth();
	int scrH = SWAN::Display::GetHeight();

	int totalBtnH = btnH * 3 + btnPad * 2;

	playButton = new Button(
	    scrW / 2 - btnW / 2,
	    scrH / 2 - totalBtnH / 2,
	    btnW, btnH);
	playButton->SetAction([] { SWAN::Log("Hello"); });
	playButton->SetText(SWAN::Res::GetBitmapFont("button"), "Play");

	settingsButton = new Button(
	    scrW / 2 - btnW / 2,
	    scrH / 2 - totalBtnH / 2 + btnH + btnPad,
	    btnW, btnH);
	settingsButton->SetText(SWAN::Res::GetBitmapFont("button"), "Settings");
	settingsButton->SetAction([] { SWAN::Log("Yes"); });

	quitButton = new Button(
	    scrW / 2 - btnW / 2,
	    scrH / 2 - totalBtnH / 2 + 2 * (btnH + btnPad),
	    btnW, btnH);
	quitButton->SetText(SWAN::Res::GetBitmapFont("button"), "Quit");
	quitButton->SetAction([] { g_CurrentState = new ExitState(); });

	gui.AddElement(playButton);
	gui.AddElement(settingsButton);
	gui.AddElement(quitButton);

	gui.SetExtraInputFrame(this);
	SWAN::SetCurrentInputFrame(&gui);
}

MenuState::~MenuState()
{
	delete playButton;
	delete settingsButton;
	delete quitButton;
}

void MenuState::OnWindowExit()
{
	g_CurrentState = new ExitState();
}

void MenuState::Update()
{
}

void MenuState::PositionButtons()
{
	int scrW = SWAN::Display::GetWidth();
	int scrH = SWAN::Display::GetHeight();

	int totalBtnH = btnH * 3 + btnPad * 2;

	playButton->SetPosition(scrW / 2 - btnW / 2, scrH / 2 - totalBtnH / 2);
	settingsButton->SetPosition(scrW / 2 - btnW / 2, scrH / 2 - totalBtnH / 2 + btnH + btnPad);
	quitButton->SetPosition(scrW / 2 - btnW / 2, scrH / 2 - totalBtnH / 2 + 2 * (btnH + btnPad));
}

void MenuState::Render()
{
	int scrW = SWAN::Display::GetWidth();
	int scrH = SWAN::Display::GetHeight();

	int totalBtnH = btnH * 3 + btnPad * 2;

	gui.RenderRect2D(
	    SWAN::Rect2D(
	        scrW / 2 - btnW / 2 - 10,
	        scrH / 2 - totalBtnH / 2 - 10,
	        btnW + 20, totalBtnH + 20, { 0.7, 0.3, 0.3, 1 }));
	gui.RenderElements();
}
