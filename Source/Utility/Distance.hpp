#ifndef UTIL_DISTANCE_HPP
#define UTIL_DISTANCE_HPP

#include <cstdint> // For int64_t

namespace Util {
	template<unsigned FromUnits, unsigned ToBaseUnit>
	class Distance {
		public:
			explicit constexpr Distance(int64_t value = 0) : amt(value) {}
			explicit constexpr operator int() 	  { return amt; }
			explicit constexpr operator int64_t() { return amt; }
			constexpr int64_t amount(){ return amt; }
		private:
			int64_t amt;
	};

	using Millimeters = Distance<1000, 1>;
	using Centimeters = Distance<100, 1>;
	using Decimeters  = Distance<10, 1>;
	using Meters      = Distance<1, 1>;
	using Kilometers  = Distance<1, 1000>;
}

#endif
