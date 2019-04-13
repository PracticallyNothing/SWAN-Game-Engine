#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "SWAN/GUI/GUIManager.hpp"
#include "State.hpp"
#include "MenuState.hpp"

#include "Button.hpp"

class SettingsState : public State
{
  public:
	SettingsState(MenuState*);
	~SettingsState();
	void Update() override;
	void Render() override;

	void OnWindowExit() override;

	void OnWindowResize(SWAN::WindowResize wr) override
	{
		PositionButtons();
	}

	void AddIntSlider(SWAN::String label, int minValue, int maxValue, std::function<void(void)> action);
	void AddFloatSlider(SWAN::String label, float minValue, float maxValue, std::function<void(void)> action);
	void AddButton(SWAN::String name, std::function<void(void)> action);
	void AddCheckbox()

  private:
	void PositionButtons();

	SWAN::Vector<Button*> buttons;

	Button *playButton,
	    *settingsButton,
	    *quitButton;

	SWAN::GUIManager gui;

	int btnW = 300;
	int btnH = 70;
	int btnPad = 10;
};

#endif
