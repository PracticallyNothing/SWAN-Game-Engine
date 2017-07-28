#ifndef UTIL_ANGLE_UNITS_HPP
#define UTIL_ANGLE_UNITS_HPP

#include <cmath> // For M_PI
#include <iostream> // For std::ostream

namespace Util {
	class Radians;
	class Degrees;

	class Radians {
		public:
			explicit constexpr Radians(double value = 0.0) : v(value){}
			explicit constexpr operator double() const { return v; }
			explicit constexpr operator float()  const { return v; }
			constexpr static Radians FromDegrees(double deg);

			explicit constexpr Radians(Degrees deg);
			explicit constexpr operator Degrees() const;

			Radians& operator+=(Radians other){ v += other.v; return *this; }
			Radians& operator-=(Radians other){ v -= other.v; return *this; }
			Radians& operator*=(double  other){ v *= other;   return *this; }
			Radians& operator/=(double  other){ v /= other;   return *this; }
		private:
			double v;
	};

	class Degrees {
		public:
			explicit constexpr Degrees(double value = 0.0) : v(value){}
			explicit constexpr Degrees(Radians rad) : v((double)rad * 180 / M_PI) {}
			constexpr static Degrees FromRadians(double rad){ return Degrees(Radians(rad)); }

			explicit constexpr operator float()   const { return v; }
			explicit constexpr operator double()  const { return v; }
			explicit constexpr operator Radians() const { return Radians(v * M_PI / 180.0); }

			Degrees& operator+=(Degrees other){ v += other.v; return *this; }
			Degrees& operator-=(Degrees other){ v -= other.v; return *this; }
			Degrees& operator*=(double  other){ v *= other;   return *this; }
			Degrees& operator/=(double  other){ v /= other;   return *this; }
		private:
			double v;
	};

	constexpr Radians::Radians(Degrees deg) : v((double)deg * M_PI / 180.0) {}
	constexpr Radians::operator Degrees() const { return Degrees(v * 180 / M_PI); }
	constexpr Radians Radians::FromDegrees(double deg){ return Radians(Degrees(deg)); }

	inline constexpr Radians operator+ (Radians lhs) { return lhs; }
	inline Radians operator- (Radians lhs) { return Radians(-(double) lhs); }
	inline Radians operator+ (Radians lhs, Radians rhs) { return Radians((double) lhs + (double) rhs); }
	inline Radians operator- (Radians lhs, Radians rhs) { return Radians((double) lhs - (double) rhs); }
	inline Radians operator* (Radians lhs, double  rhs) { return Radians((double) lhs * rhs); }
	inline Radians operator/ (Radians lhs, double  rhs) { return Radians((double) lhs / rhs); }

	inline bool operator== (Radians lhs, Radians rhs){ return (double) lhs == (double) rhs; }
	inline bool operator!= (Radians lhs, Radians rhs){ return (double) lhs != (double) rhs; }
	inline bool operator>= (Radians lhs, Radians rhs){ return (double) lhs >= (double) rhs; }
	inline bool operator<= (Radians lhs, Radians rhs){ return (double) lhs <= (double) rhs; }
	inline bool operator<  (Radians lhs, Radians rhs){ return (double) lhs <  (double) rhs; }
	inline bool operator>  (Radians lhs, Radians rhs){ return (double) lhs >  (double) rhs; }

	inline constexpr Degrees operator+ (Degrees lhs) { return lhs; }
	inline Degrees operator- (Degrees lhs) { return Degrees(-(double) lhs); }
	inline Degrees operator+ (Degrees lhs, Degrees rhs) { return Degrees((double) lhs + (double) rhs); }
	inline Degrees operator- (Degrees lhs, Degrees rhs) { return Degrees((double) lhs - (double) rhs); }
	inline Degrees operator* (Degrees lhs, double  rhs) { return Degrees((double) lhs * rhs); }
	inline Degrees operator/ (Degrees lhs, double  rhs) { return Degrees((double) lhs / rhs); }

	inline bool operator== (Degrees lhs, Degrees rhs){ return (double) lhs == (double) rhs; }
	inline bool operator!= (Degrees lhs, Degrees rhs){ return (double) lhs != (double) rhs; }
	inline bool operator>= (Degrees lhs, Degrees rhs){ return (double) lhs >= (double) rhs; }
	inline bool operator<= (Degrees lhs, Degrees rhs){ return (double) lhs <= (double) rhs; }
	inline bool operator<  (Degrees lhs, Degrees rhs){ return (double) lhs <  (double) rhs; }
	inline bool operator>  (Degrees lhs, Degrees rhs){ return (double) lhs >  (double) rhs; }

	namespace StreamOps {
		inline std::ostream& operator<< (std::ostream& os, Radians rad){
			os << "Radians(" << (double)rad << ")";
			return os;
		}

		inline std::ostream& operator<< (std::ostream& os, Degrees deg){
			os << "Degrees(" << (double)deg << ")";
			return os;
		}
	}
}

#endif
