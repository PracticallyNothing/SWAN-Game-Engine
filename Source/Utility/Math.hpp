#ifndef UTIL_MATH_HPP
#define UTIL_MATH_HPP

namespace Util {
    constexpr inline double pixelToGLCoord(int screenSize, int pixel) {
        return 2.0 / (double)screenSize * (double)pixel - 1.0;
    }

    constexpr inline int GLCoordToPixel(int screenSize, double GLCoord) {
        return screenSize / 2 * (GLCoord + 1);
    }
} /* Util */

#endif //UTIL_MATH_HPP
