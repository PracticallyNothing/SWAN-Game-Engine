#ifndef SWAN_EVENT_LISTENER_HPP
#define SWAN_EVENT_LISTENER_HPP

#include <chrono>     // For std::chrono::duration
#include <functional> // For std::function<Ret, Args...>

namespace SWAN {
class EventListener {
  public:
	using CheckFuncT  = std::function<bool(void)>;
	using ActionFuncT = std::function<void(void)>;

	explicit EventListener (bool runOnce = false)
	  : runOnce (runOnce) {}
	EventListener (CheckFuncT checkFunc, ActionFuncT actionFunc, bool runOnce = false)
	  : check (checkFunc)
	  , action (actionFunc)
	  , runOnce (runOnce) {}

	void update () {
		if (check () && !expired ())
			action ();

		alreadyRan = true;
	}

	void setCheckFunc (CheckFuncT cf) { check = cf; }
	void setActionFunc (ActionFuncT af) { action = af; }

	void setRunOnce (bool ro) { runOnce = ro; }

	bool runsOnce () { return runOnce; }
	bool expired () { return runOnce && alreadyRan; }
	bool active () { return check (); }

	std::string description = "an event listener",
	            type        = "generic";

  private:
	CheckFuncT  check;
	ActionFuncT action;

	bool runOnce;
	bool alreadyRan = false;
};

using Clock = std::chrono::steady_clock;

inline EventListener
CreateTimer (typename Clock::duration time, EventListener::ActionFuncT action, bool runOnce = true) {
	auto          start = Clock::now ();
	EventListener res (
	  [time, start]() -> bool {
		  return time <= Clock::now () - start;
	  },
	  action,
	  runOnce);

	return res;
}
inline EventListener
CreateRepeatingTimer (typename Clock::duration time, EventListener::ActionFuncT action) {
	return CreateTimer (time, action, false);
}
}

#endif
