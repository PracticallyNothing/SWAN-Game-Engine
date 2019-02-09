#ifndef SWAN_RENDERER_HPP
#define SWAN_RENDERER_HPP

#include <chrono>

namespace SWAN
{
	using Clock = std::chrono::steady_clock;
	using TimePoint = Clock::time_point;
	using Duration = Clock::duration;

	struct Renderer {
		Duration MinFrameTime;
		TimePoint LastRender;
	};
} // namespace SWAN

#endif
