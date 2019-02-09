#ifndef SWAN_UTIL_MATH_HPP
#define SWAN_UTIL_MATH_HPP

#include <algorithm>

namespace SWAN
{
	namespace Util
	{
		template <typename T>
		constexpr T Clamp(T val, T min, T max)
		{
			if(val < std::min<T>(min, max))
				return min;
			else if(val > std::max<T>(max, min))
				return max;
			else
				return val;
		}

		template <typename T>
		constexpr bool InRange(T val, T min, T max)
		{
			return min <= val && val <= max;
		}

		template <typename T>
		constexpr bool RangesOverlap(T min1, T max1, T min2, T max2)
		{
			return InRange(min2, min1, max1) || InRange(max2, min1, max1) || InRange(min1, min2, max2) || InRange(max1, min2, max2);
		}

		constexpr double PixelToGLCoord(int screenSize, int pixel)
		{
			return 2.0 / (double) screenSize * (double) pixel - 1.0;
		}

		constexpr double GLCoordToPixel(int screenSize, double GLCoord)
		{
			return screenSize / 2 * (GLCoord + 1);
		}

		constexpr double Normalize(double v, double min, double max)
		{
			return (v - min) / (max - min);
		}

		constexpr double UnNormalize(double x, double min, double max)
		{
			return x * max + (x + 1) * min;
		}

		constexpr double LinearInterpolate(double clampAmt, double min, double max)
		{
			return min + clampAmt * (max - min);
		}
		constexpr double CurveInterpolate(double clampAmt, double min, double max)
		{
			return min + clampAmt * clampAmt * (max - min);
		}

	} // namespace Util
} // namespace SWAN

#endif // UTIL_MATH_HPP
