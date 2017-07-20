#ifndef UTIL_RATIO_HPP
#define UTIL_RATIO_HPP

namespace Util {
    template <unsigned _first, unsigned _second>
    struct Ratio {
        enum { First = _first, Second = _second };

        template <typename T>
        constexpr T calcUnitFromFirst(T val) {
            return val / First;
        }

        template <typename T>
        constexpr T calcUnitFromSecond(T val) {
            return val / Second;
        }

        template <typename T>
        constexpr T calcValFromFirst(T val) {
            return calcUnitFromFirst(val) * Second;
        }

        template <typename T>
        constexpr T calcValFromSecond(T val) {
            return calcUnitFromSecond(val) * First;
        }
    };
}

#endif
