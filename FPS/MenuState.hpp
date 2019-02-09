#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "SWAN/GUI/GUIManager.hpp"
#include "State.hpp"

#include "Button.hpp"

struct Panel {
	SWAN::String Title = "";
	SWAN::vec4 BGColor = { 0.75, 0.75, 0.75, 1 };
	SWAN::Vector<Button> Buttons;
};

class MenuState : public State
{
  public:
	MenuState();
	~MenuState();
	void Update() override;
	void Render() override;

	void OnWindowExit() override;

	void OnWindowResize(SWAN::WindowResize wr) override
	{
		PositionButtons();
	}

  private:
	void PositionButtons();

	Button *playButton,
	    *settingsButton,
	    *quitButton;

	SWAN::GUIManager gui;

	int btnW = 300;
	int btnH = 70;
	int btnPad = 10;
};

#endif
