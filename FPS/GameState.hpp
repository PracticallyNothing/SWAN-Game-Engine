#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include "State.hpp"
#include "Scene.hpp"

class GameState : public State
{
  public:
	void Update() override;
	void Render() override;

	void OnKeyPress(SWAN::Key key) override;
	void OnKeyHold(SWAN::Key key) override;

	void OnWindowExit() override;

  private:
	SWAN::GUIManager gui;
};

#endif
