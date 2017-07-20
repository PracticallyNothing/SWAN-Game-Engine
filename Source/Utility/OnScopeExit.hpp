#ifndef UTIL_ON_SCOPE_EXIT_HPP
#define UTIL_ON_SCOPE_EXIT_HPP

#include <functional> // For std::function<Ret, Args...>

namespace Util {
	class OnScopeExit {
		public:
			OnScopeExit(const std::function<void(void)>func) : f(func){}
			~OnScopeExit(){ f(); }
		private:
			const std::function<void(void)> f;
	};
}

#endif //UTIL_ON_SCOPE_EXIT_HPP
