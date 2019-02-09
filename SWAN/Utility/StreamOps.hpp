#ifndef SWAN_UTIL_STREAM_OPS_HPP
#define SWAN_UTIL_STREAM_OPS_HPP

#include <iostream> // For std::ostream
#include <vector>   // For std::vector<T>

// #include "Core/Input.hpp"        // For SWAN::MouseState
#include "Maths/Vector.hpp"      // For SWAN::vec*
#include "Physics/Transform.hpp" // For SWAN::Transform
#include "Rendering/Shader.hpp"  // For SWAN::ShaderUniform
// #include "Core/Input.hpp"
#include "Importing/INI.hpp"

namespace SWAN
{
	namespace Util
	{
		namespace StreamOps
		{
			using std::ostream;
			using std::vector;

			inline ostream& operator<<(ostream& os, ivec2 v)
			{
				os << "ivec2 (" << v.x << ", " << v.y << ")";
				return os;
			}
			inline ostream& operator<<(ostream& os, ivec3 v)
			{
				os << "ivec3 (" << v.x << ", " << v.y << ", " << v.z << ")";
				return os;
			}
			inline ostream& operator<<(ostream& os, ivec4 v)
			{
				os << "ivec4 (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w
				   << ")";
				return os;
			}

			inline ostream& operator<<(ostream& os, SWAN::vec2 v)
			{
				os << "vec2 (" << v.x << ", " << v.y << ")";
				return os;
			}
			inline ostream& operator<<(ostream& os, SWAN::vec3 v)
			{
				os << "vec3 (" << v.x << ", " << v.y << ", " << v.z << ")";
				return os;
			}
			inline ostream& operator<<(ostream& os, vec4 v)
			{
				os << "vec4 (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w
				   << ")";
				return os;
			}

			inline ostream& operator<<(ostream& os, const mat2& m)
			{
#define W std::setw(10)
				os << "mat2 {\n"
				   << std::fixed << std::setprecision(4)
				   << "    (" << W << m(0, 0) << ", " << W << m(0, 1) << "),\n"
				   << "    (" << W << m(1, 0) << ", " << W << m(1, 1) << ")\n"
				   << "}";

				return os;
			}
			inline ostream& operator<<(ostream& os, const mat3& m)
			{
				os << "mat3 {\n"
				   << std::fixed << std::setprecision(4)
				   << "    (" << W << m(0, 0) << ", " << W << m(0, 1) << ", " << W << m(0, 2) << "),\n"
				   << "    (" << W << m(1, 0) << ", " << W << m(1, 1) << ", " << W << m(1, 2) << "),\n"
				   << "    (" << W << m(2, 0) << ", " << W << m(2, 1) << ", " << W << m(2, 2) << ")\n"
				   << "}";

				return os;
			}

			inline ostream& operator<<(ostream& os, const mat4& m)
			{
				os << "SWAN::mat4 {\n"
				   << std::fixed << std::setprecision(4)
				   << "    (" << W << m(0, 0) << ", " << W << m(0, 1) << ", " << W << m(0, 2) << ", " << W << m(0, 3) << "),\n"
				   << "    (" << W << m(1, 0) << ", " << W << m(1, 1) << ", " << W << m(1, 2) << ", " << W << m(1, 3) << "),\n"
				   << "    (" << W << m(2, 0) << ", " << W << m(2, 1) << ", " << W << m(2, 2) << ", " << W << m(2, 3) << "),\n"
				   << "    (" << W << m(3, 0) << ", " << W << m(3, 1) << ", " << W << m(3, 2) << ", " << W << m(3, 3) << ")\n"
				   << "}";

				return os;
#undef W
			}

			inline ostream& operator<<(ostream& os, Transform t)
			{
				os << "Transform (pos: " << t.pos << ", rot: " << t.rot
				   << ", scale: " << t.scale << ")";
				return os;
			}

			/*
	    inline ostream& operator<<(ostream& os, ShaderUniform s) {
		os << "ShaderUniform (\"" << s.Name << "\": ";

		switch(s.Type) {
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

	    inline ostream& operator<<(ostream& os, vector<ShaderUniform> v) {
		os << "std::vector<T> {\n";

		for(const auto& e : v)
		    os << "    " << e << ",\n";

		os << "}";

		return os;
	    }

	    */

			// inline ostream& operator<<(ostream& os, MouseState s) {
			// 	os << "MouseState (x: " << s.x
			// 	   << ", y: " << s.y
			// 	   << ", LMB: " << s.LMB
			// 	   << ", MMB: " << s.MMB
			// 	   << ", RMB: " << s.RMB << ")";
			// 	return os;
			// }

			inline std::ostream& operator<<(std::ostream& os, const INI::Config& cfg)
			{
				os << "INI::Config {\n";
				for(const auto& kv : cfg) {
					os << "    [" << kv.first << "]\n";
					for(const auto& kv2 : kv.second) {
						os << "    " << kv2.first << " = " << kv2.second << '\n';
					}
					os << '\n';
				}
				os << '}';
				return os;
			}
		} // namespace StreamOps
	}     // namespace Util
} // namespace SWAN

#endif
