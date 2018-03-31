#ifndef SWAN_VEC_HPP
#define SWAN_VEC_HPP

#include <cmath>

namespace SWAN {
	struct vec2 {
		constexpr vec2() : x(0), y(0) {}
		explicit constexpr vec2(double v) : x(v), y(v) {}
		constexpr vec2(double x, double y) : x(x), y(y) {}

		inline constexpr void operator+=(const vec2& v) {x += v.x; y += v.y;}
		inline constexpr void operator-=(const vec2& v) {x -= v.x; y -= v.y;}
		inline constexpr void operator*=(const vec2& v) {x *= v.x; y *= v.y;}
		inline constexpr void operator/=(const vec2& v) {x /= v.x; y /= v.y;}
		inline constexpr void operator*=(double d) { x *= d; y *= d; }
		inline constexpr void operator/=(double d) { x /= d; y /= d; }

		double x, y;
	};

	struct ivec2 {
		constexpr ivec2() : x(0), y(0) {}
		explicit constexpr ivec2(int v) : x(v), y(v) {}
		constexpr ivec2(int x, int y) : x(x), y(y) {}
		explicit constexpr ivec2(vec2 v) : x(v.x), y(v.y) {}

		inline constexpr void operator+=(const ivec2& v) {x += v.x; y += v.y;}
		inline constexpr void operator-=(const ivec2& v) {x -= v.x; y -= v.y;}
		inline constexpr void operator*=(const ivec2& v) {x *= v.x; y *= v.y;}
		inline constexpr void operator/=(const ivec2& v) {x /= v.x; y /= v.y;}
		inline constexpr void operator*=(double d) { x *= d; y *= d; }
		inline constexpr void operator/=(double d) { x /= d; y /= d; }

		int x, y;
	};

	struct vec3 {
		constexpr vec3() : x(0), y(0), z(0) {}
		explicit constexpr vec3(double v) : x(v), y(v), z(v) {}
		constexpr vec3(double x, double y, double z) : x(x), y(y), z(z) {}
		constexpr vec3(vec2 v, double z) : x(v.x), y(v.y), z(z) {}
		constexpr vec3(ivec2 v, double z) : x(v.x), y(v.y), z(z) {}

		constexpr operator vec2() { return vec2(x, y); }
		constexpr operator ivec2() { return ivec2(x, y); }

		inline constexpr void operator+=(const vec3& v) { x += v.x; y += v.y; z += v.z; }
		inline constexpr void operator-=(const vec3& v) { x -= v.x; y -= v.y; z -= v.z; }
		inline constexpr void operator*=(const vec3& v) { x *= v.x; y *= v.y; z *= v.z; }
		inline constexpr void operator/=(const vec3& v) { x /= v.x; y /= v.y; z /= v.z; }
		inline constexpr void operator*=(double d) { x *= d; y *= d; z *= d; }
		inline constexpr void operator/=(double d) { x /= d; y /= d; z /= d; }

		double x, y, z;
	};
	struct ivec3 {
		constexpr ivec3() : x(0), y(0), z(0) {}
		explicit constexpr ivec3(int v) : x(v), y(v), z(v) {}
		constexpr ivec3(int x, int y, int z) : x(x), y(y), z(z) {}
		constexpr ivec3(vec2 v, double z) : x(v.x), y(v.y), z(z) {}
		constexpr ivec3(ivec2 v, double z) : x(v.x), y(v.y), z(z) {}

		explicit constexpr ivec3(vec2 v) : x(v.x), y(v.y), z(0) {}
		explicit constexpr ivec3(const vec3& v) : x(v.x), y(v.y), z(v.z) {}

		constexpr ivec3(ivec2 v) : x(v.x), y(v.y), z(0) {}

		explicit constexpr operator ivec2() { return ivec2(x, y); }
		explicit constexpr operator vec2() { return vec2(x, y); }
		explicit constexpr operator vec3() { return vec3(x, y, z); }

		inline constexpr void operator+=(const ivec3& v) { x += v.x; y += v.y; z += v.z; }
		inline constexpr void operator-=(const ivec3& v) { x -= v.x; y -= v.y; z -= v.z; }
		inline constexpr void operator*=(const ivec3& v) { x *= v.x; y *= v.y; z *= v.z; }
		inline constexpr void operator/=(const ivec3& v) { x /= v.x; y /= v.y; z /= v.z; }
		inline constexpr void operator*=(double d) { x *= d; y *= d; z *= d; }
		inline constexpr void operator/=(double d) { x /= d; y /= d; z /= d; }

		int x, y, z;
	};

	struct vec4 {
		constexpr vec4() : x(0), y(0), z(0), w(0) {}
		explicit constexpr vec4(double v) : x(v), y(v), z(v), w(v) {}
		constexpr vec4(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}
		constexpr vec4(vec2 v, double z, double w) : x(v.x), y(v.y), z(z), w(w) {}
		constexpr vec4(const vec3& v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}
		constexpr vec4(ivec2 v, double z, double w) : x(v.x), y(v.y), z(z), w(w) {}
		constexpr vec4(ivec3 v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}

		inline constexpr void operator+=(const vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; }
		inline constexpr void operator-=(const vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
		inline constexpr void operator*=(const vec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; }
		inline constexpr void operator/=(const vec4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; }
		inline constexpr void operator*=(double d) { x *= d; y *= d; z *= d; w *= d; }
		inline constexpr void operator/=(double d) { x /= d; y /= d; z /= d; w /= d; }

		constexpr operator vec2() { return vec2(x, y); }
		constexpr operator vec3() { return vec3(x, y, z); }
		constexpr operator ivec2() { return ivec2(x, y); }
		constexpr operator ivec3() { return ivec3(x, y, z); }

		double x, y, z, w;
	};
	struct ivec4 {
		constexpr ivec4() : x(0), y(0), z(0), w(0) {}
		explicit constexpr ivec4(int v) : x(v), y(v), z(v), w(v) {}
		constexpr ivec4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) {}
		explicit constexpr ivec4(const vec2& v, int z = 0, int w = 0) : x(v.x), y(v.y), z(z), w(w) {}
		explicit constexpr ivec4(const vec3& v, int w = 0) : x(v.x), y(v.y), z(v.z), w(w) {}
		explicit constexpr ivec4(const vec4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
		constexpr ivec4(ivec2 v, int z = 0, int w = 0) : x(v.x), y(v.y), z(z), w(w) {}
		constexpr ivec4(ivec3 v, int w = 0) : x(v.x), y(v.y), z(v.z), w(w) {}

		explicit constexpr operator ivec2() { return ivec2(x, y); }
		explicit constexpr operator ivec3() { return ivec3(x, y, z); }

		explicit constexpr operator vec2() { return vec2(x, y); }
		explicit constexpr operator vec3() { return vec3(x, y, z); }
		explicit constexpr operator vec4() { return vec4(x, y, z, w); }

		inline constexpr void operator+=(const ivec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; }
		inline constexpr void operator-=(const ivec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
		inline constexpr void operator*=(const ivec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; }
		inline constexpr void operator/=(const ivec4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; }
		inline constexpr void operator*=(double d) { x *= d; y *= d; z *= d; w *= d; }
		inline constexpr void operator/=(double d) { x /= d; y /= d; z /= d; w /= d; }

		int x, y, z, w;
	};

	inline constexpr double Length2(vec2 v) { return v.x * v.x + v.y * v.y; }
	inline constexpr double Length2(vec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }
	inline constexpr double Length2(vec4 v) { return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w; }
	inline constexpr int Length2(ivec2 v) { return v.x * v.x + v.y * v.y; }
	inline constexpr int Length2(ivec3 v) { return v.x * v.x + v.y * v.y + v.z * v.z; }
	inline constexpr int Length2(ivec4 v) { return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w; }

	inline double Length(const vec2& v) { return std::sqrt(Length2(v)); }
	inline double Length(const vec3& v) { return std::sqrt(Length2(v)); }
	inline double Length(const vec4& v) { return std::sqrt(Length2(v)); }
	inline int Length(ivec2 v)   { return std::sqrt(Length2(v)); }
	inline int Length(ivec3 v)   { return std::sqrt(Length2(v)); }
	inline int Length(ivec4 v)   { return std::sqrt(Length2(v)); }


	// {{ Operator+
	inline constexpr vec2 operator+(vec2 a, vec2 b) { return { a.x + b.x, a.y + b.y }; }
	inline constexpr vec3 operator+(vec3 a, vec3 b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
	inline constexpr vec4 operator+(vec4 a, vec4 b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
	inline constexpr ivec2 operator+(ivec2 a, ivec2 b) { return { a.x + b.x, a.y + b.y }; }
	inline constexpr ivec3 operator+(ivec3 a, ivec3 b) { return { a.x + b.x, a.y + b.y, a.z + b.z }; }
	inline constexpr ivec4 operator+(ivec4 a, ivec4 b) { return { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w }; }
	// }}

	// {{ Operator-
	inline constexpr vec2 operator-(vec2 a, vec2 b) { return { a.x - b.x, a.y - b.y }; }
	inline constexpr vec3 operator-(vec3 a, vec3 b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
	inline constexpr vec4 operator-(vec4 a, vec4 b) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }
	inline constexpr ivec2 operator-(ivec2 a, ivec2 b) { return { a.x - b.x, a.y - b.y }; }
	inline constexpr ivec3 operator-(ivec3 a, ivec3 b) { return { a.x - b.x, a.y - b.y, a.z - b.z }; }
	inline constexpr ivec4 operator-(ivec4 a, ivec4 b) { return { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w }; }
	// }}

	// {{ operator* double
	inline constexpr vec2 operator*(vec2 v, double d) { return { v.x * d, v.y * d }; }
	inline constexpr vec3 operator*(vec3 v, double d) { return { v.x * d, v.y * d, v.z * d }; }
	inline constexpr vec4 operator*(vec4 v, double d) { return { v.x * d, v.y * d, v.z * d, v.w * d }; }
	inline constexpr ivec2 operator*(ivec2 v, double d) { return { (int) (v.x * d), (int) (v.y * d) }; }
	inline constexpr ivec3 operator*(ivec3 v, double d) { return { (int) (v.x * d), (int) (v.y * d), (int) (v.z * d) }; }
	inline constexpr ivec4 operator*(ivec4 v, double d) { return { (int) (v.x * d), (int) (v.y * d), (int) (v.z * d), (int)(v.w * d) }; }
	// }}

	// {{ operator/ double
	inline constexpr vec2 operator/(vec2 v, double d) { return { v.x / d, v.y / d }; }
	inline constexpr vec3 operator/(vec3 v, double d) { return { v.x / d, v.y / d, v.z / d }; }
	inline constexpr vec4 operator/(vec4 v, double d) { return { v.x / d, v.y / d, v.z / d, v.w / d }; }
	inline constexpr ivec2 operator/(ivec2 v, double d) { return { (int) (v.x / d), (int) (v.y / d) }; }
	inline constexpr ivec3 operator/(ivec3 v, double d) { return { (int) (v.x / d), (int) (v.y / d), (int) (v.z / d) }; }
	inline constexpr ivec4 operator/(ivec4 v, double d) { return { (int) (v.x / d), (int) (v.y / d), (int) (v.z / d), (int) (v.w / d)}; }
	// }}

	// {{ operator* vec
	inline constexpr vec2 operator*(vec2 a, vec2 b) { return { a.x * b.x, a.y * b.y }; }
	inline constexpr vec3 operator*(vec3 a, vec3 b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }
	inline constexpr vec4 operator*(vec4 a, vec4 b) { return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w }; }
	inline constexpr ivec2 operator*(ivec2 a, ivec2 b) { return { a.x * b.x, a.y * b.y }; }
	inline constexpr ivec3 operator*(ivec3 a, ivec3 b) { return { a.x * b.x, a.y * b.y, a.z * b.z }; }
	inline constexpr ivec4 operator*(ivec4 a, ivec4 b) { return { a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w }; }
	// }}

	// {{ operator/ vec
	inline constexpr vec2 operator/(vec2 a, vec2 b) { return { a.x / b.x, a.y / b.y }; }
	inline constexpr vec3 operator/(vec3 a, vec3 b) { return { a.x / b.x, a.y / b.y, a.z / b.z }; }
	inline constexpr vec4 operator/(vec4 a, vec4 b) { return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }; }
	inline constexpr ivec2 operator/(ivec2 a, ivec2 b) { return { a.x / b.x, a.y / b.y }; }
	inline constexpr ivec3 operator/(ivec3 a, ivec3 b) { return { a.x / b.x, a.y / b.y, a.z / b.z }; }
	inline constexpr ivec4 operator/(ivec4 a, ivec4 b) { return { a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w }; }
	// }}

	// {{ Normalized
	inline vec2 Normalized(const vec2& v) { return v / Length(v); }
	inline vec3 Normalized(const vec3& v) { return v / Length(v); }
	inline vec4 Normalized(const vec4& v) { return v / Length(v); }
	inline ivec2 Normalized(ivec2 v) { return v / Length(v); }
	inline ivec3 Normalized(ivec3 v) { return v / Length(v); }
	inline ivec4 Normalized(ivec4 v) { return v / Length(v); }
	// }}

	inline double Dot(vec2 a, vec2 b) {
		a = Normalized(a);
		b = Normalized(b);
		return a.x * b.x + a.y * b.y;
	}
	inline double Dot(vec3 a, vec3 b) {
		a = Normalized(a);
		b = Normalized(b);
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	inline double Dot(vec4 a, vec4 b) {
		a = Normalized(a);
		b = Normalized(b);
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}
	inline double Dot(ivec2 a, ivec2 b) {
		a = Normalized(a);
		b = Normalized(b);
		return a.x * b.x + a.y * b.y;
	}
	inline double Dot(ivec3 a, ivec3 b) {
		a = Normalized(a);
		b = Normalized(b);
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
	inline double Dot(ivec4 a, ivec4 b) {
		a = Normalized(a);
		b = Normalized(b);
		return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
	}
	inline constexpr double Cross(vec2 a, vec2 b) { return a.x * b.y - a.y * b.x; }
	inline constexpr int Cross(ivec2 a, ivec2 b) { return a.x * b.y - a.y * b.x; }

	inline constexpr vec3 Cross(vec3 a, vec3 b) {
		return { a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.z };
	}
	inline constexpr ivec3 Cross(ivec3 a, ivec3 b) {
		return { a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.z };
	}

	inline constexpr bool operator==(vec2 a, vec2 b)   {return a.x == b.x && a.y == b.y;}
	inline constexpr bool operator==(vec3 a, vec3 b)   {return a.x == b.x && a.y == b.y && a.z == b.z;}
	inline constexpr bool operator==(vec4 a, vec4 b)   {return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;}
	inline constexpr bool operator==(ivec2 a, ivec2 b) {return a.x == b.x && a.y == b.y;}
	inline constexpr bool operator==(ivec3 a, ivec3 b) {return a.x == b.x && a.y == b.y && a.z == b.z;}
	inline constexpr bool operator==(ivec4 a, ivec4 b) {return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;}

	inline constexpr bool operator!=(const vec2 & a, const vec2 & b)   {return !(a == b); }
	inline constexpr bool operator!=(const vec3 & a, const vec3 & b)   {return !(a == b); }
	inline constexpr bool operator!=(const vec4 & a, const vec4 & b)   {return !(a == b); }
	inline constexpr bool operator!=(const ivec2& a, const ivec2& b) {return !(a == b); }
	inline constexpr bool operator!=(const ivec3& a, const ivec3& b) {return !(a == b); }
	inline constexpr bool operator!=(const ivec4& a, const ivec4& b) {return !(a == b); }
} // namespace SWAN

#endif
