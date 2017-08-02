#ifndef UTIL_MATH_HPP
#define UTIL_MATH_HPP

namespace Util {
    constexpr inline double pixelToGLCoord(int screenSize, int pixel) {
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

} /* Util */

#endif //UTIL_MATH_HPP
