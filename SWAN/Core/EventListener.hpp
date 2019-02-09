#ifndef SWAN_EVENT_LISTENER_HPP
#define SWAN_EVENT_LISTENER_HPP

#include <chrono>     // For std::chrono::duration
#include <functional> // For std::function<Ret, Args...>

namespace SWAN
{
	/** 
     * @brief An object that listens for events and, when they occur, runs an action function.
     *
     *
     */
	class EventListener
	{
	  public:
		/// Type for the condition checking function
		using CheckFuncT = std::function<bool(void)>;
		/// Type for the action function
		using ActionFuncT = std::function<void(void)>;

		/// Construct an empty EventListener with no checker and no action.
		explicit EventListener(bool runOnce = false) : runOnce(runOnce) {}

		/// Construct an EventListener with a checker and an action.
		EventListener(CheckFuncT checkFunc, ActionFuncT actionFunc, bool runOnce = false)
		    : check(checkFunc), action(actionFunc), runOnce(runOnce) {}

		/// Check if the condition checking function returns true and execute the action if it is.
		void update()
		{
			if(check() && !expired())
				action();

			alreadyRan = true;
		}

		/// Check if the condition checking function returns true and execute the action if it is (runs update()).
		inline void operator()() { update(); }

		/// Check if the listener can listen for other events.
		bool expired() { return runOnce && alreadyRan; }

		/// Information about the listener
		std::string description = "an event listener", type = "generic";

	  private:
		CheckFuncT check;
		ActionFuncT action;

		bool runOnce;
		bool alreadyRan = false;
	};

	/// Typedef for std::chrono::steady_clock, which is used to track the time.
	using Clock = std::chrono::steady_clock;

	/** 
     * @brief Create an event listener, which waits for a certain time, then executes its action.
     *
     * The listener uses std::chrono::steady_clock to track the time.
     *
     * NOTE: The action will not run on its own, you need to either call update() or operator()
     *       before it can execute.
     *
     * @param time The std::chrono::duration, for which to wait.
     * @param action Action function to execute once the time runs out.
     * @param runOnce Whether the timer can repeat or not.
     *
     * @return An EventListener instance.
     */
	inline EventListener CreateTimer(typename Clock::duration time,
	                                 EventListener::ActionFuncT action,
	                                 bool runOnce = true)
	{
		auto start = Clock::now();
		EventListener res([time, start]() -> bool { return time <= Clock::now() - start; },
		                  action, runOnce);

		return res;
	}

	inline EventListener CreateRepeatingTimer(typename Clock::duration time,
	                                          EventListener::ActionFuncT action)
	{
		auto start = Clock::now();
		EventListener res([lastMoment = start, time]() mutable -> bool {
			if(time <= Clock::now() - lastMoment) {
				lastMoment = Clock::now();
				return true;
			}
			return false;
		},
		                  action, false);
		return res;
	}
} // namespace SWAN

#endif
