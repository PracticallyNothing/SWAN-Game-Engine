#ifndef EDITOR_STATE_HPP
#define EDITOR_STATE_HPP

#include "State.hpp"

class EditorState : public State
{
  public:
	EditorState();
	~EditorState();

	void Update() override;
	void Render() override;

  private:
	bool showGrid = false;
};

#endif