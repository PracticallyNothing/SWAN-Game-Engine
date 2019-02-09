#ifndef STATE_HPP
#define STATE_HPP

#include "SWAN/GUI/GUIManager.hpp"
#include "SWAN/Input/InputFrame.hpp"

class State : public SWAN::InputFrame
{
  public:
	virtual ~State() {}

	virtual void Update() = 0;
	virtual void Render() = 0;
};

extern State* g_CurrentState;

#endif
