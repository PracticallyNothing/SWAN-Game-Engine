#ifndef UTIL_STREAM_OPS_HPP
#define UTIL_STREAM_OPS_HPP

#include <iostream> // For std::ostream

#include <glm/fwd.hpp>              // For glm::vec*
#include "../Physics/Transform.hpp" // For Transform

namespace Util {
namespace StreamOps {
	using std::ostream;

	inline ostream& operator<<(ostream& os, glm::vec2 v){
		os << "vec2 (" << v.x << ", " << v.y  << ")";
		return os;
	}
	inline ostream& operator<<(ostream& os, glm::vec3 v){
		os << "vec3 (" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}
	inline ostream& operator<<(ostream& os, glm::vec4 v){
		os << "vec4 (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return os;
	}

	inline ostream& operator<<(ostream& os, glm::mat4 m){
		os << "mat4 {\n"
		   << "      (" << m[0][0] << ", " << m[0][1] << ", " << m[0][2] << ", " << m[0][3] << "),\n"
		   << "      (" << m[1][0] << ", " << m[1][1] << ", " << m[1][2] << ", " << m[1][3] << "),\n"
		   << "      (" << m[2][0] << ", " << m[2][1] << ", " << m[2][2] << ", " << m[2][3] << "),\n"
		   << "      (" << m[3][0] << ", " << m[3][1] << ", " << m[3][2] << ", " << m[3][3] << ")\n"
		   << "}";

		return os;
	}

	inline ostream& operator<<(ostream& os, Transform t){
		os << "Transform (pos: " << t.pos << ", rot: " << t.rot << ", scale: " << t.scale << ")";
		return os;
	}
}
}

#endif
