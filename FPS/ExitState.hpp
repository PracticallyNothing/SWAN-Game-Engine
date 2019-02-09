#ifndef EXIT_STATE_HPP
#define EXIT_STATE_HPP

#include "State.hpp"

extern bool g_GameRunning;
extern void Stop();

class ExitState : public State
{
  public:
	void Update() override { Stop(); }
	void Render() override {}
};

#endif
