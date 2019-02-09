#ifndef UTIL_ON_EXIT_HPP
#define UTIL_ON_EXIT_HPP

#include <functional>
#include <vector>

namespace SWAN
{
	namespace Util
	{
		class OnExit
		{
		  public:
			OnExit(std::function<void()> func, bool isFinalizer = false)
			{
				if(!isFinalizer)
					vec.push_back(func);
				else
					finalizerFunc = func;
			}

			~OnExit() { finalizerFunc(); }

		  private:
			static std::vector<std::function<void()>> vec;
			static OnExit finalizer;
			std::function<void()> finalizerFunc = []() {};
		};

		std::vector<std::function<void()>> OnExit::vec;
		OnExit OnExit::finalizer = OnExit(
		    []() {
			    for(auto& f : OnExit::vec)
				    f();
		    },
		    true);
	} // namespace Util
} // namespace SWAN

#endif // HEADER_HPP
