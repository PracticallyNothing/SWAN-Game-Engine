#ifndef SWAN_UTIL_STREAM_OPS_HPP
#define SWAN_UTIL_STREAM_OPS_HPP

#include <iostream> // For std::ostream
#include <vector>   // For std::vector<T>

#include "Core/Input.hpp"        // For SWAN::MouseState
#include "Maths/Vector.hpp"      // For SWAN::vec*
#include "Physics/Transform.hpp" // For SWAN::Transform
#include "Rendering/Shader.hpp"  // For SWAN::ShaderUniform

namespace SWAN {
namespace Util {
	namespace StreamOps {
		using std::ostream;
		using std::vector;

		inline ostream& operator<<(ostream& os, ivec2 v) {
			os << "ivec2 (" << v.x << ", " << v.y << ")";
			return os;
		}
		inline ostream& operator<<(ostream& os, ivec3 v) {
			os << "ivec3 (" << v.x << ", " << v.y << ", " << v.z << ")";
			return os;
		}
		inline ostream& operator<<(ostream& os, ivec4 v) {
			os << "ivec4 (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w
			   << ")";
			return os;
		}

		inline ostream& operator<<(ostream& os, vec2 v) {
			os << "vec2 (" << v.x << ", " << v.y << ")";
			return os;
		}
		inline ostream& operator<<(ostream& os, vec3 v) {
			os << "vec3 (" << v.x << ", " << v.y << ", " << v.z << ")";
			return os;
		}
		inline ostream& operator<<(ostream& os, vec4 v) {
			os << "vec4 (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w
			   << ")";
			return os;
		}

		inline ostream& operator<<(ostream& os, const mat2& m) {
			os << "mat2 {\n"
			   << "      (" << m(0,0) << ", " << m(0,1) << "),\n"
			   << "      (" << m(1,0) << ", " << m(1,1) << ")\n"
			   << "}";

			return os;
		}
		inline ostream& operator<<(ostream& os, const mat3& m) {
			os << "mat3 {\n"
			   << "      (" << m(0,0) << ", " << m(0,1) << ", " << m(0,2) << "),\n"
			   << "      (" << m(1,0) << ", " << m(1,1) << ", " << m(1,2) << "),\n"
			   << "      (" << m(2,0) << ", " << m(2,1) << ", " << m(2,2) << ")\n"
			   << "}";

			return os;
		}

		inline ostream& operator<<(ostream& os, const mat4& m) {
			os << "mat4 {\n"
			   << "      (" << m(0,0) << ", " << m(0,1) << ", " << m(0,2) << ", " << m(0,3) << "),\n"
			   << "      (" << m(1,0) << ", " << m(1,1) << ", " << m(1,2) << ", " << m(1,3) << "),\n"
			   << "      (" << m(2,0) << ", " << m(2,1) << ", " << m(2,2) << ", " << m(2,3) << "),\n"
			   << "      (" << m(3,0) << ", " << m(3,1) << ", " << m(3,2) << ", " << m(3,3) << ")\n"
			   << "}";

			return os;
		}

		inline ostream& operator<<(ostream& os, Transform t) {
			os << "Transform (pos: " << t.pos << ", rot: " << t.rot
			   << ", scale: " << t.scale << ")";
			return os;
		}

		inline ostream& operator<<(ostream& os, ShaderUniform s) {
			os << "ShaderUniform (\"" << s.name << "\": ";

			switch(s.type) {
				case ShaderUniform::T_INT: os << "[int] " << s.data.i; break;
				case ShaderUniform::T_BOOL: os << "[bool] " << s.data.b; break;
				case ShaderUniform::T_FLOAT: os << "[float] " << s.data.f; break;
				case ShaderUniform::T_DOUBLE: os << "[double] " << s.data.d; break;

				case ShaderUniform::T_VEC2: os << "[vec2] " << s.data.v2; break;
				case ShaderUniform::T_VEC3: os << "[vec3] " << s.data.v3; break;
				case ShaderUniform::T_VEC4: os << "[vec4] " << s.data.v4; break;
				case ShaderUniform::T_MAT2: os << "[mat2] " << s.data.m2; break;
				case ShaderUniform::T_MAT3: os << "[mat3] " << s.data.m3; break;
				case ShaderUniform::T_MAT4: os << "[mat4] " << s.data.m4; break;

				case ShaderUniform::T_TRANSFORM: os << "[transform] " << s.data.transf; break;
			}
			os << ")";

			return os;
		}

		inline ostream& operator<<(ostream& os, MouseState s) {
			os << "MouseState (x: " << s.x
			   << ", y: " << s.y
			   << ", LMB: " << s.LMB
			   << ", MMB: " << s.MMB
			   << ", RMB: " << s.RMB << ")";
			return os;
		}

		inline ostream& operator<<(ostream& os, vector<ShaderUniform> v) {
			os << "std::vector<T> {\n";

			for(const auto& e : v)
				os << "    " << e << ",\n";

			os << "}";

			return os;
		}
	} // namespace StreamOps
} // namespace Util
} // namespace SWAN

#endif
