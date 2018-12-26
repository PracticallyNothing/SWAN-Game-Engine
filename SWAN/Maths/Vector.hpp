#ifndef SWAN_VEC_HPP
#define SWAN_VEC_HPP

#include <cmath>
#include <type_traits> // For std::is_arithmetic<T>, std::common_type<T, U, ...>

#define IS_ARITH(T) static_assert(std::is_arithmetic<T>::value, "Type " #T "is not an arithmetic type.")
#define ENABLE_IF_ARITH(Type, RetType) typename std::enable_if<std::is_arithmetic<Type>::value, RetType>::type

namespace SWAN {
    template<typename T>
    struct tvec2 {
	using Type = T;
	
	constexpr tvec2() : x(T()), y(T()) { IS_ARITH(T); }
	constexpr tvec2(T x, T y) : x(x), y(y) { IS_ARITH(T); }
	explicit constexpr tvec2(T v) : x(v), y(v) { IS_ARITH(T); }
	template<typename U = T> constexpr tvec2(const tvec2<U>& v) : x(v.x), y(v.y) { IS_ARITH(T); }

	template<typename U = T> constexpr void operator+=(const tvec2<U>& v) { IS_ARITH(U); x += v.x; y += v.y;}
	template<typename U = T> constexpr void operator-=(const tvec2<U>& v) { IS_ARITH(U); x -= v.x; y -= v.y;}
	template<typename U = T> constexpr void operator*=(const tvec2<U>& v) { IS_ARITH(U); x *= v.x; y *= v.y;}
	template<typename U = T> constexpr void operator/=(const tvec2<U>& v) { IS_ARITH(U); x /= v.x; y /= v.y;}

	template<typename U = T> constexpr void operator*=(U d) { IS_ARITH(U); x *= d; y *= d;}
	template<typename U = T> constexpr void operator/=(U d) { IS_ARITH(U); x /= d; y /= d;}

	T x, y;
    };

    template<typename T>
    struct tvec3 {
	using Type = T;

	constexpr tvec3() : x(T()), y(T()), z(T()) { IS_ARITH(T); }
	explicit constexpr tvec3(T v) : x(v), y(v), z(v) { IS_ARITH(T); }
	constexpr tvec3(T x, T y, T z) : x(x), y(y), z(z) { IS_ARITH(T); }
	template<typename U = T> constexpr tvec3(tvec2<U> v, T z) : x(v.x), y(v.y), z(z) { IS_ARITH(T); }
	template<typename U = T> constexpr tvec3(tvec3<U> v)      : x(v.x), y(v.y), z(v.z) { IS_ARITH(T); }

	template<typename U = T> constexpr operator tvec2<U>() { IS_ARITH(U); return tvec2<U>(x, y); }

	template<typename U = T> constexpr void operator+=(const tvec3<U>& v) { IS_ARITH(U); x += v.x; y += v.y; z += v.z;}
	template<typename U = T> constexpr void operator-=(const tvec3<U>& v) { IS_ARITH(U); x -= v.x; y -= v.y; z -= v.z;}
	template<typename U = T> constexpr void operator*=(const tvec3<U>& v) { IS_ARITH(U); x *= v.x; y *= v.y; z *= v.z;}
	template<typename U = T> constexpr void operator/=(const tvec3<U>& v) { IS_ARITH(U); x /= v.x; y /= v.y; z /= v.z;}

	template<typename U = T> constexpr void operator*=(U d) { IS_ARITH(U); x *= d; y *= d; z *= d;}
	template<typename U = T> constexpr void operator/=(U d) { IS_ARITH(U); x /= d; y /= d; z /= d;}

	T x, y, z;
    };

    template<typename T>
    struct tvec4 {
	using Type = T;

	constexpr tvec4() : x(T()), y(T()), z(T()), w(T()) { IS_ARITH(T); }
	constexpr tvec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { IS_ARITH(T); }
	explicit constexpr tvec4(T v) : x(v), y(v), z(v), w(v) { IS_ARITH(T); }
	template<typename U = T> constexpr tvec4(tvec2<U> v, T z, T w) : x(v.x), y(v.y), z(z), w(w) { IS_ARITH(T); }
	template<typename U = T> constexpr tvec4(tvec3<U> v, T w) : x(v.x), y(v.y), z(v.z), w(w) { IS_ARITH(T); }
	template<typename U = T> constexpr tvec4(tvec4<U> v) : x(v.x), y(v.y), z(v.z), w(v.w) { IS_ARITH(T); }

	template<typename U = T> constexpr void operator+=(const tvec4<U>& v) { IS_ARITH(U); x += v.x; y += v.y; z += v.z; w += v.w;}
	template<typename U = T> constexpr void operator-=(const tvec4<U>& v) { IS_ARITH(U); x -= v.x; y -= v.y; z -= v.z; w -= v.w;}
	template<typename U = T> constexpr void operator*=(const tvec4<U>& v) { IS_ARITH(U); x *= v.x; y *= v.y; z *= v.z; w *= v.w;}
	template<typename U = T> constexpr void operator/=(const tvec4<U>& v) { IS_ARITH(U); x /= v.x; y /= v.y; z /= v.z; w /= v.w;}

	template<typename U = T> constexpr void operator*=(U d) { IS_ARITH(U); x *= d; y *= d; z *= d; w *= d;}
	template<typename U = T> constexpr void operator/=(U d) { IS_ARITH(U); x /= d; y /= d; z /= d; w /= d;}

	template<typename U = T> constexpr operator tvec2<U>() { IS_ARITH(U); return tvec2<U>(x, y); }
	template<typename U = T> constexpr operator tvec3<U>() { IS_ARITH(U); return tvec3<U>(x, y, z); }

	float x, y, z, w;
    };

    template<typename T> constexpr double Length2(tvec2<T> v) { return v.x * v.x + v.y * v.y; }
    template<typename T> constexpr double Length2(tvec3<T> v) { return v.x * v.x + v.y * v.y + v.z * v.z; }
    template<typename T> constexpr double Length2(tvec4<T> v) { return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w; }

    template<typename T> double Length(const tvec2<T>& v) { return std::sqrt(Length2(v)); }
    template<typename T> double Length(const tvec3<T>& v) { return std::sqrt(Length2(v)); }
    template<typename T> double Length(const tvec4<T>& v) { return std::sqrt(Length2(v)); }

    template<typename T> constexpr tvec2<T> operator+(tvec2<T> a) { return a; }
    template<typename T> constexpr tvec3<T> operator+(tvec3<T> a) { return a; }
    template<typename T> constexpr tvec4<T> operator+(tvec4<T> a) { return a; }

    template<typename T> constexpr tvec2<T> operator-(tvec2<T> a) { return { -a.x, -a.y }; }
    template<typename T> constexpr tvec3<T> operator-(tvec3<T> a) { return { -a.x, -a.y, -a.z }; }
    template<typename T> constexpr tvec4<T> operator-(tvec4<T> a) { return { -a.x, -a.y, -a.z, -a.w }; }

    template<typename T, typename U> constexpr tvec2<std::common_type_t<T,U>> operator+(tvec2<T> a, tvec2<U> b) { return { a.x + b.x, a.y + b.y }; }
    template<typename T, typename U> constexpr tvec3<std::common_type_t<T,U>> operator+(tvec3<T> a, tvec3<U> b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
    template<typename T, typename U> constexpr tvec4<std::common_type_t<T,U>> operator+(tvec4<T> a, tvec4<U> b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }

    template<typename T, typename U> constexpr tvec2<std::common_type_t<T,U>> operator-(tvec2<T> a, tvec2<U> b) { return { a.x - b.x, a.y - b.y }; }
    template<typename T, typename U> constexpr tvec3<std::common_type_t<T,U>> operator-(tvec3<T> a, tvec3<U> b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
    template<typename T, typename U> constexpr tvec4<std::common_type_t<T,U>> operator-(tvec4<T> a, tvec4<U> b) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }

    template<typename T, typename U = T> constexpr ENABLE_IF_ARITH(U, tvec2<T>) operator*(tvec2<T> v, U d) { return { v.x * d, v.y * d }; }
    template<typename T, typename U = T> constexpr ENABLE_IF_ARITH(U, tvec3<T>) operator*(tvec3<T> v, U d) { return { v.x * d, v.y * d, v.z * d }; }
    template<typename T, typename U = T> constexpr ENABLE_IF_ARITH(U, tvec4<T>) operator*(tvec4<T> v, U d) { return { v.x * d, v.y * d, v.z * d, v.w * d }; }

    template<typename T, typename U = T> constexpr ENABLE_IF_ARITH(U, tvec2<T>) operator/(tvec2<T> v, U d) { return { v.x / d, v.y / d }; }
    template<typename T, typename U = T> constexpr ENABLE_IF_ARITH(U, tvec3<T>) operator/(tvec3<T> v, U d) { return { v.x / d, v.y / d, v.z / d }; }
    template<typename T, typename U = T> constexpr ENABLE_IF_ARITH(U, tvec4<T>) operator/(tvec4<T> v, U d) { return { v.x / d, v.y / d, v.z / d, v.w / d }; }

    template<typename T, typename U = T> constexpr tvec2<std::common_type_t<T,U>> operator*(tvec2<T> a, tvec2<U> b) { return { a.x * b.x, a.y * b.y }; }
    template<typename T, typename U = T> constexpr tvec3<std::common_type_t<T,U>> operator*(tvec3<T> a, tvec3<U> b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }
    template<typename T, typename U = T> constexpr tvec4<std::common_type_t<T,U>> operator*(tvec4<T> a, tvec4<U> b) { return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w }; }

    template<typename T, typename U = T> constexpr tvec2<std::common_type_t<T,U>> operator/(tvec2<T> a, tvec2<U> b) { return { a.x / b.x, a.y / b.y }; }
    template<typename T, typename U = T> constexpr tvec3<std::common_type_t<T,U>> operator/(tvec3<T> a, tvec3<U> b) { return { a.x / b.x, a.y / b.y, a.z / b.z }; }
    template<typename T, typename U = T> constexpr tvec4<std::common_type_t<T,U>> operator/(tvec4<T> a, tvec4<U> b) { return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }; }

    template<typename T> tvec2<T> Normalized(const tvec2<T>& v) { return v / Length(v); }
    template<typename T> tvec3<T> Normalized(const tvec3<T>& v) { return v / Length(v); }
    template<typename T> tvec4<T> Normalized(const tvec4<T>& v) { return v / Length(v); }

    template<typename T, typename U = T> double Dot(tvec2<T> a, tvec2<U> b) { return a.x * b.x + a.y * b.y; }
    template<typename T, typename U = T> double Dot(tvec3<T> a, tvec3<U> b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    template<typename T, typename U = T> double Dot(tvec4<T> a, tvec4<U> b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

    template<typename T, typename U = T> constexpr double Cross(tvec2<T> a, tvec2<U> b) { return a.x * b.y - a.y * b.x; }

    template<typename T, typename U = T>
    constexpr tvec3<T> Cross(tvec3<T> a, tvec3<U> b) {
	return { a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x };
    }

    template<typename T, typename U = T> constexpr bool operator==(tvec2<T> a, tvec2<U> b) { return a.x == b.x && a.y == b.y; }
    template<typename T, typename U = T> constexpr bool operator==(tvec3<T> a, tvec3<U> b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
    template<typename T, typename U = T> constexpr bool operator==(tvec4<T> a, tvec4<U> b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }

    template<typename T, typename U = T> constexpr bool operator!=(const tvec2<T>& a, const tvec2<U>& b) { return !(a == b); }
    template<typename T, typename U = T> constexpr bool operator!=(const tvec3<T>& a, const tvec3<U>& b) { return !(a == b); }
    template<typename T, typename U = T> constexpr bool operator!=(const tvec4<T>& a, const tvec4<U>& b) { return !(a == b); }

    using ivec2 = tvec2<int>;
    using ivec3 = tvec3<int>;
    using ivec4 = tvec4<int>;

    using uvec2 = tvec2<unsigned int>;
    using uvec3 = tvec3<unsigned int>;
    using uvec4 = tvec4<unsigned int>;

    using fvec2 = tvec2<float>;
    using fvec3 = tvec3<float>;
    using fvec4 = tvec4<float>;

    using dvec2 = tvec2<double>;
    using dvec3 = tvec3<double>;
    using dvec4 = tvec4<double>;

    using vec2 = dvec2;
    using vec3 = dvec3;
    using vec4 = dvec4;

} // namespace SWAN

#undef IS_ARITH

#endif
