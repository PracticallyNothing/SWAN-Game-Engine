#ifndef MENU_STATE_HPP
#define MENU_STATE_HPP

#include "SWAN/GUI/GUIManager.hpp"
#include "State.hpp"

#include "Button.hpp"

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

	void AddButton(SWAN::String name, std::function<void(void)> action);

  private:
	void PositionButtons();

	SWAN::Vector<Button*> buttons;
	SWAN::GUIManager gui;

	int btnW = 300;
	int btnH = 70;
	int btnPad = 10;
};

#endif
