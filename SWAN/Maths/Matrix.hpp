#ifndef SWAN_MATRIX_HPP
#define SWAN_MATRIX_HPP

#include <algorithm> // For std::swap(a, b)
#include <array>     // For std::array<T, N>
#include "../Utility/CxArray.hpp"

#include "Vector.hpp" // For SWAN::vec{2,3,4}

namespace SWAN {
	struct mat2;
	struct mat3;
	struct mat4;

	struct mat2 {
		constexpr mat2() : data({1, 0, 0, 1}) {}
		explicit constexpr mat2(double v) : data({v, 0, 0, v}) {}
		constexpr mat2(double a, double b, double c, double d) : data({a, b, c, d}) {}

		constexpr double& operator()(int x, int y)       { return data[x + y * 2]; }
		constexpr double  operator()(int x, int y) const { return data[x + y * 2]; }

		constexpr double& get(int x, int y)       { return data[x + y * 2]; }
		constexpr double  get(int x, int y) const { return data[x + y * 2]; }

		Util::CxArray<double, 4> data;
	};
	struct mat3 {
		constexpr mat3() : data({1, 0, 0, 0, 1, 0, 0, 0, 1}) {}
		explicit constexpr mat3(double v) : data({v, 0, 0, 0, v, 0, 0, 0, v}) {}
		constexpr mat3(double a, double b, double c,
					   double d, double e, double f,
					   double g, double h, double i) : data({a, b, c, d, e, f, g, h, i}) {}

		Util::CxArray<double, 9> data;
		constexpr double& operator()(int x, int y) { return data[x + y * 3]; }
		constexpr double operator()(int x, int y) const { return data[x + y * 3]; }

		constexpr double& get(int x, int y)       { return data[x + y * 3]; }
		constexpr double  get(int x, int y) const { return data[x + y * 3]; }

		constexpr operator mat2() { return mat2(get(0, 0), get(1,0), get(0,1), get(1,1)); }
	};
	struct mat4 {
		constexpr mat4() : data({1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}) {}
		explicit constexpr mat4(double v) : data({v, 0, 0, 0, 0, v, 0, 0, 0, 0, v, 0, 0, 0, 0, v}) {}
		constexpr mat4(double a, double b, double c, double d,
					   double e, double f, double g, double h,
					   double i, double j, double k, double l,
					   double m, double n, double o, double p) : data({a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p}) {}

		Util::CxArray<double, 16> data;
		constexpr double& operator()(int x, int y)       { return data[x + y * 4]; }
		constexpr double  operator()(int x, int y) const { return data[x + y * 4]; }

		constexpr double& get(int x, int y)       { return data[x + y * 4]; }
		constexpr double  get(int x, int y) const { return data[x + y * 4]; }

		constexpr operator mat2() { return mat2(get(0, 0), get(1,0), get(0,1), get(1,1)); }
		constexpr operator mat3() { return mat3(get(0, 0), get(1, 0), get(2, 0),
												get(0, 1), get(1, 1), get(2, 1),
												get(0, 2), get(1, 2), get(2, 2)); }
	};

	constexpr mat2 operator-(const mat2& m);
	constexpr mat3 operator-(const mat3& m);
	constexpr mat4 operator-(const mat4& m);

	constexpr mat2 operator+(const mat2& a, const mat2& b);
	constexpr mat3 operator+(const mat3& a, const mat3& b);
	constexpr mat4 operator+(const mat4& a, const mat4& b);

	constexpr mat2 operator-(const mat2& a, const mat2& b);
	constexpr mat3 operator-(const mat3& a, const mat3& b);
	constexpr mat4 operator-(const mat4& a, const mat4& b);

	constexpr mat2 operator*(const mat2& m, double d);
	constexpr mat3 operator*(const mat3& m, double d);
	constexpr mat4 operator*(const mat4& m, double d);

	constexpr mat2 operator/(const mat2& m, double d);
	constexpr mat3 operator/(const mat3& m, double d);
	constexpr mat4 operator/(const mat4& m, double d);

	constexpr mat2 operator*(const mat2& a, const mat2& b);
	constexpr mat3 operator*(const mat3& a, const mat3& b);
	constexpr mat4 operator*(const mat4& a, const mat4& b);

	constexpr mat2 Inverse(const mat2& m);
	constexpr mat3 Inverse(const mat3& m);
	constexpr mat4 Inverse(const mat4& m);

	constexpr ivec2 operator*(ivec2 v, const mat2& m);
	constexpr ivec3 operator*(ivec3 v, const mat3& m);
	constexpr ivec4 operator*(ivec4 v, const mat4& m);

	constexpr vec2 operator*(vec2 v, const mat2& m);
	constexpr vec3 operator*(vec3 v, const mat3& m);
	constexpr vec4 operator*(vec4 v, const mat4& m);

	constexpr mat2 Transpose(const mat2& m);
	constexpr mat3 Transpose(const mat3& m);
	constexpr mat4 Transpose(const mat4& m);

	constexpr double Determinant(const mat2& m);
	constexpr double Determinant(const mat3& m);
	constexpr double Determinant(const mat4& m);

	constexpr mat4 Translate(const vec2& m);
	constexpr mat4 Translate(const vec3& m);
	constexpr mat4 Translate(const vec4& m);

	inline mat4 Rotate(double v, const vec3& m);
	constexpr mat4 Scale(const vec3& m);

	constexpr mat4 OrthoProject (double left, double right,
						 double bottom, double top,
						 double zNear, double zFar);
	inline mat4 Perspective (double fov, double aspect, double zNear, double zFar);
	inline mat4 LookAt(vec3 eye, vec3 center, vec3 up);

	// ---------------------------------- //
	// ---------------------------------- //
	// --------- IMPLEMENTATION --------- //
	// ---------------------------------- //
	// ---------------------------------- //
	constexpr mat2 operator-(const mat2& m) {
		mat2 res;

		const int size = 2;
		for(int y = 0; y < size; y++)
			for(int x = 0; x < size; x++)
				res(x, y) = -m(x, y);

		return res;
	}
	constexpr mat3 operator-(const mat3& m) {
		mat3 res;

		const int size = 3;
		for(int y = 0; y < size; y++)
			for(int x = 0; x < size; x++)
				res(x, y) = -m(x, y);

		return res;
	}
	constexpr mat4 operator-(const mat4& m) {
		mat4 res;

		const int size = 4;
		for(int y = 0; y < size; y++)
			for(int x = 0; x < size; x++)
				res(x, y) = -m(x, y);

		return res;
	}

	constexpr mat2 operator+(const mat2& a, const mat2& b) {
		mat2 res;

		const int size = 2;
		for(int y = 0; y < size; y++)
			for(int x = 0; x < size; x++)
				res(x, y) = a(x, y) + b(x, y);

		return res;
	}

	constexpr mat3 operator+(const mat3& a, const mat3& b) {
		mat3 res;

		const int size = 3;
		for(int y = 0; y < size; y++)
			for(int x = 0; x < size; x++)
				res(x, y) = a(x, y) + b(x, y);

		return res;
	}

	constexpr mat4 operator+(const mat4& a, const mat4& b) {
		mat4 res;

		const int size = 4;
		for(int y = 0; y < size; y++)
			for(int x = 0; x < size; x++)
				res(x, y) = a(x, y) + b(x, y);

		return res;
	}

	constexpr mat2 operator-(const mat2& a, const mat2& b) {
		mat2 res;

		const int size = 2;
		for(int y = 0; y < size; y++)
			for(int x = 0; x < size; x++)
				res(x, y) = a(x, y) - b(x, y);

		return res;
	}

	constexpr mat3 operator-(const mat3& a, const mat3& b) {
		mat3 res;

		const int size = 3;
		for(int y = 0; y < size; y++)
			for(int x = 0; x < size; x++)
				res(x, y) = a(x, y) - b(x, y);

		return res;
	}

	constexpr mat4 operator-(const mat4& a, const mat4& b) {
		mat4 res;

		const int size = 4;
		for(int y = 0; y < size; y++)
			for(int x = 0; x < size; x++)
				res(x, y) = a(x, y) - b(x, y);

		return res;
	}

	template<typename T>
	static constexpr void Swap(T& a, T& b) {
		T tmp = a;
		a = b;
		b = tmp;
	}

	constexpr mat2 Transpose(const mat2& m) {
		mat2 res = m;
		Swap(res(1, 0), res(0, 1));
		return res;
	}

	constexpr mat3 Transpose(const mat3& m) {
		mat3 res = m;
		Swap(res(1, 0), res(0, 1));
		Swap(res(2, 0), res(0, 2));
		Swap(res(2, 1), res(1, 2));
		return res;
	}

	constexpr mat4 Transpose(const mat4& m) {
		mat4 res = m;
		Swap(res(1, 0), res(0, 1));
		Swap(res(2, 0), res(0, 2));
		Swap(res(2, 1), res(1, 2));
		Swap(res(3, 1), res(1, 3));
		Swap(res(3, 2), res(2, 3));
		return res;
	}

	constexpr double Determinant(const mat2& m) { return m(0, 0) * m(1, 1) - m(1, 0) * m(0, 1); }
	constexpr double Determinant(const mat3& m) {
		mat2 ma(m(1, 1), m(2, 1), m(1,2), m(2,2));
		mat2 mb(m(0, 1), m(2, 1), m(0,2), m(2,2));
		mat2 mc(m(0, 1), m(1, 1), m(0,2), m(1,2));

		double a = m(0,0),
			b = m(1,0),
			c = m(2,0);

		return a * Determinant(ma) - b * Determinant(mb) + c * Determinant(mc);
	}
	constexpr double Determinant(const mat4& m) {
		mat3 ma(m(1,1), m(2,1), m(3,1),
				m(1,2), m(2,2), m(3,2),
				m(1,3), m(2,3), m(3,3));

		mat3 mb(m(0,1), m(2,1), m(3,1),
				m(0,2), m(2,2), m(3,2),
				m(0,3), m(2,3), m(3,3));

		mat3 mc(m(0,1), m(1,1), m(3,1),
				m(0,2), m(1,2), m(3,2),
				m(0,3), m(1,3), m(3,3));

		mat3 md(m(0,1), m(1,1), m(2,1),
				m(0,2), m(1,2), m(2,2),
				m(0,3), m(1,3), m(2,3));

		double a = m(0, 0), b = m(1, 0), c = m(2, 0), d = m(3, 0);

		return
			+ a * Determinant(ma)
			- b * Determinant(mb)
			+ c * Determinant(mc)
			- d * Determinant(md);
	}

	constexpr mat2 operator*(const mat2& m, double d) {
		return mat2(m(0,0) * d, m(1,0) * d,
					m(0,1) * d, m(1,1) * d);
	}
	constexpr mat3 operator*(const mat3& m, double d) {
		return mat3(m(0,0) * d, m(1,0) * d, m(2,0) * d, 
					m(0,1) * d, m(1,1) * d, m(2,1) * d, 
					m(0,2) * d, m(1,2) * d, m(2,2) * d);
	}

	constexpr mat4 operator*(const mat4& m, double d) {
		return mat4(m(0,0)*d, m(1,0)*d, m(2,0)*d, m(3,0)*d,
					m(0,1)*d, m(1,1)*d, m(2,1)*d, m(3,1)*d,
					m(0,2)*d, m(1,2)*d, m(2,2)*d, m(3,2)*d,
					m(0,3)*d, m(1,3)*d, m(2,3)*d, m(3,3)*d);
	}

	constexpr mat2 operator/(const mat2& m, double d) {
		return mat2(m(0,0) / d, m(1,0) / d,
					m(0,1) / d, m(1,1) / d);
	}
	constexpr mat3 operator/(const mat3& m, double d) {
		return mat3(m(0,0) / d, m(1,0) / d, m(2,0) / d, 
					m(0,1) / d, m(1,1) / d, m(2,1) / d, 
					m(0,2) / d, m(1,2) / d, m(2,2) / d);
	}

	constexpr mat4 operator/(const mat4& m, double d) {
		return mat4(m(0,0)/d, m(1,0)/d, m(2,0)/d, m(3,0)/d,
					m(0,1)/d, m(1,1)/d, m(2,1)/d, m(3,1)/d,
					m(0,2)/d, m(1,2)/d, m(2,2)/d, m(3,2)/d,
					m(0,3)/d, m(1,3)/d, m(2,3)/d, m(3,3)/d);
	}

	template<typename T, int N>
	static constexpr T _DoMult(const T& a, const T& b) {
		T res;
		for(int y = 0; y < N; y++) {
			for(int x = 0; x < N; x++) {
				double sum = 0;
				for(int i = 0; i < N; i++) {
					sum += a(x+i, y) * b(x, y+i);
				}
				res(x, y) = sum;
			}
		}
		return res;
	}
	constexpr mat2 operator*(const mat2& a, const mat2& b) {
		return _DoMult<mat2, 2>(a, b);
	}
	constexpr mat3 operator*(const mat3& a, const mat3& b) {
		return _DoMult<mat3, 3>(a, b);
	}
	constexpr mat4 operator*(const mat4& a, const mat4& b) {
		return _DoMult<mat4, 4>(a, b);
	}

	constexpr mat2 Inverse(const mat2& m) {
		if(Determinant(m) == 0)
			return {}; 

		return mat2(m(1,1), -m(1,0), -m(0,1), m(0,0)) * Determinant(m);
	}
	constexpr mat3 Inverse(const mat3& m) {
		if(Determinant(m) == 0)
			return {}; 

		return mat3(Determinant(mat2(m(1,1), m(1,2), m(2,1), m(2,2))),
					Determinant(mat2(m(0,2), m(0,1), m(2,2), m(2,1))),
					Determinant(mat2(m(0,1), m(0,2), m(1,1), m(1,2))),

					Determinant(mat2(m(1,2), m(1,0), m(2,2), m(2,0))),
					Determinant(mat2(m(0,0), m(0,2), m(2,0), m(2,2))),
					Determinant(mat2(m(0,2), m(0,0), m(1,2), m(1,0))),

					Determinant(mat2(m(1,0), m(1,1), m(2,0), m(2,1))),
					Determinant(mat2(m(0,1), m(0,0), m(2,1), m(2,0))),
					Determinant(mat2(m(0,0), m(0,1), m(1,0), m(1,1))))
			* (1.0 / Determinant(m));
	}

	static constexpr mat4 _GetMinor(const mat4& m) {
		mat4 res;

		mat3 tmp;
		for(int y = 0; y < 4; y++) {
			for(int x = 0; x < 4; x++) {
				int xNext = 0, yNext = 0;
				for(int yy = 0; yy < 4; yy++) {
					if(yy == y)
						continue;
					for(int xx = 0; xx < 4; xx++) {
						if(xx == x)
							continue;
						tmp(xNext, yNext) = m(xx, yy);
						xNext++;
					}
					yNext++;
				}
				res(x, y) = Determinant(tmp);
			}
		}

		return res;
	}
	constexpr mat4 Inverse(const mat4& m) {
		if(Determinant(m) == 0)
			return {}; 

		mat4 res = _GetMinor(m);

		for(int y = 0; y < 4; y++) {
			for(int x = 0; x < 4; x++) {
				if(x + y % 2)
					res(x, y) = -res(x, y);
			}
		}

		res = Transpose(res);
		
		return res * (1.0 / Determinant(m));
	}

	constexpr ivec2 operator*(ivec2 v, const mat2& m) {
		return ivec2(v.x * m(0,0) + v.y * m(0,1),
					 v.x * m(1,0) + v.y * m(1,1));
	}	
	constexpr ivec3 operator*(ivec3 v, const mat3& m) {
		return ivec3(v.x * m(0,0) + v.y * m(0,1) + v.z * m(0,2),
					 v.x * m(1,0) + v.y * m(1,1) + v.z * m(1,2),
					 v.x * m(2,0) + v.y * m(2,1) + v.z * m(2,2));
	}
	constexpr ivec4 operator*(ivec4 v, const mat4& m) {
		return ivec4(v.x * m(0,0) + v.y * m(0,1) + v.z * m(0,2) + v.w * m(0,3),
					 v.x * m(1,0) + v.y * m(1,1) + v.z * m(1,2) + v.w * m(1,3),
					 v.x * m(2,0) + v.y * m(2,1) + v.z * m(2,2) + v.w * m(2,3),
					 v.x * m(3,0) + v.y * m(3,1) + v.z * m(3,2) + v.w * m(3,3));
	}

	constexpr vec2 operator*(vec2 v, const mat2& m) {
		return vec2(v.x * m(0,0) + v.y * m(0,1),
					v.x * m(1,0) + v.y * m(1,1));
	}
	constexpr vec3 operator*(vec3 v, const mat3& m) {
		return vec3(v.x * m(0,0) + v.y * m(0,1) + v.z * m(0,2),
					v.x * m(1,0) + v.y * m(1,1) + v.z * m(1,2),
					v.x * m(2,0) + v.y * m(2,1) + v.z * m(2,2));
	}
	constexpr vec4 operator*(vec4 v, const mat4& m) {
		return vec4(v.x * m(0,0) + v.y * m(0,1) + v.z * m(0,2) + v.w * m(0,3),
					v.x * m(1,0) + v.y * m(1,1) + v.z * m(1,2) + v.w * m(1,3),
					v.x * m(2,0) + v.y * m(2,1) + v.z * m(2,2) + v.w * m(2,3),
					v.x * m(3,0) + v.y * m(3,1) + v.z * m(3,2) + v.w * m(3,3));
	}

	constexpr mat4 Translate(const vec2& v) {
		mat4 res;	
		res(3, 0) = v.x;
		res(3, 1) = v.y;
		return res;
	}
	constexpr mat4 Translate(const vec3& v) {
		mat4 res;	
		res(3, 0) = v.x;
		res(3, 1) = v.y;
		res(3, 2) = v.z;
		return res;
	}
	constexpr mat4 Translate(const vec4& v) {
		mat4 res;	
		res(3, 0) = v.x;
		res(3, 1) = v.y;
		res(3, 2) = v.z;
		res(3, 3) = v.w;
		return res;
	}

	constexpr static vec4 _GetRow (const mat4& m, int row) {	
		return vec4(m(row, 0), m(row, 1), m(row, 2), m(row, 3));
	}
	constexpr static void _SetRow (mat4& m, int row,
								   double a, double b,
								   double c, double d) {	
		m(row, 0) = a;
		m(row, 1) = b;
		m(row, 2) = c;
		m(row, 3) = d;
	}

	constexpr static void _SetRow (mat4& m, int row, const vec4& v) {	
		_SetRow(m, row, v.x, v.y, v.z, v.w);
	}

	inline mat4 Rotate(double angle, const vec3& vec) {
		double a = angle;
		double c = cos(a);
		double s = sin(a);

		vec3 axis(Normalized(vec));
		vec3 temp(axis * (1 - c));

		mat4 rot;
		rot(0,0) = c + temp.x * axis.x;
		rot(0,1) =     temp.x * axis.y + s * axis.z;
		rot(0,2) =     temp.x * axis.z - s * axis.y;

		rot(1,0) =     temp.y * axis.x - s * axis.z;
		rot(1,1) = c + temp.y * axis.y;
		rot(1,2) =     temp.y * axis.z + s * axis.x;

		rot(2,0) =     temp.z * axis.x + s * axis.y;
		rot(2,1) =     temp.z * axis.y - s * axis.x;
		rot(2,2) = c + temp.z * axis.z;

		mat4 I = mat4();
		mat4 res;
		_SetRow(res, 0, _GetRow(I, 0) * rot(0,0) + _GetRow(I, 1) * rot(0,1) + _GetRow(I,2) * rot(0,2));
		_SetRow(res, 1, _GetRow(I, 0) * rot(1,0) + _GetRow(I, 1) * rot(1,1) + _GetRow(I,2) * rot(1,2));
		_SetRow(res, 2, _GetRow(I, 0) * rot(2,0) + _GetRow(I, 1) * rot(2,1) + _GetRow(I,2) * rot(2,2));
		_SetRow(res, 3, _GetRow(I, 3));
		return res;
	}

	constexpr mat4 Scale(const vec3& m) {
		mat4 res;
		res(0,0) = m.x;
		res(1,1) = m.y;
		res(2,2) = m.z;
		return res;
	}

	constexpr mat4 OrthoProject (double left, double right,
								 double bottom, double top,
								 double zNear, double zFar) {
		mat4 res;

		res(0,0) = 2 / (right - left);
		res(3,0) = -(right + left) / (right - left);

		res(1,1) = 2 / (top - bottom);
		res(3,1) = -(top + bottom) / (top - bottom);

		res(2, 2) = 2 / (zFar - zNear);
		res(3, 2) = -(zFar + zNear) / (zFar - zNear);

		return res;
	}

	inline mat4 Perspective (double fov, double aspect, double zNear, double zFar) {
		const double tanHalfFov = tan(fov / 2.0);

		mat4 res;
		res(0, 0) = 1.0 / (aspect * tanHalfFov);
		res(1, 1) = 1.0 / (tanHalfFov);
		res(2, 3) = - 1.0;

		res(2, 2) = zFar / (zNear - zFar);
		res(3, 2) = -(zFar * zNear) / (zFar - zNear);

		return res;
	}

	inline mat4 LookAt(vec3 eye, vec3 center, vec3 up) {
		const vec3 f(Normalized(center - eye));
		const vec3 s(Normalized(Cross(up, f)));
		const vec3 u(Cross(f, s));

		mat4 res(1);
		res(0, 0) = s.x;
		res(1, 0) = s.y;
		res(2, 0) = s.z;
		res(0, 1) = u.x;
		res(1, 1) = u.y;
		res(2, 1) = u.z;
		res(0, 2) = f.x;
		res(1, 2) = f.y;
		res(2, 2) = f.z;
		res(3, 0) = -Dot(s, eye);
		res(3, 1) = -Dot(u, eye);
		res(3, 2) = -Dot(f, eye);

		return res;
	}

} // namespace SWAN

#endif
