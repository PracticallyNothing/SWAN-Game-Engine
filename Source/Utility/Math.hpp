#ifndef SWAN_UTIL_MATH_HPP
#define SWAN_UTIL_MATH_HPP

namespace SWAN { namespace Util {
	template<typename T>
	constexpr inline T Clamp(T val, T min, T max){
		if (val < min)
			return min;
		else if (val > max)
			return max;
		else
			return val;
	}

	template<typename T>
	constexpr inline bool InRange(T val, T min, T max){
		return
			val > min &&
			val < max;
	}

	template<typename T>
	constexpr inline bool RangesOverlap(T min1, T max1, T min2, T max2){
		return
			InRange(min2, min1, max1) ||
			InRange(max2, min1, max1) ||
			InRange(min1, min2, max2) ||
			InRange(max1, min2, max2);
	}

	constexpr inline double PixelToGLCoord(int screenSize, int pixel) {
        return 2.0 / (double)screenSize * (double)pixel - 1.0;
    }

    constexpr inline double GLCoordToPixel(int screenSize, double GLCoord) {
        return screenSize / 2 * (GLCoord + 1);
    }

    constexpr inline double Normalize(double v, double min, double max){
        return (v - min) / (max - min);
    }

	constexpr inline double UnNormalize(double x, double min, double max){
		return x*max + (x + 1)*min;
	}

} /* Util */ }

#endif //UTIL_MATH_HPP
