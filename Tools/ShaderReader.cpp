#define SDL_MAIN_HANDLED

#include "SWAN/Core/Defs.hpp"
#include "SWAN/Core/Format.hpp"
#include "SWAN/Core/Logging.hpp"
#include "Utility/StringUtil.hpp"

const char* const unlitFrag = R"ddd(
#version 130

out vec4 fCol;

void main() { 
    fCol = vec4(0.3, 0.3, 0.3, 1.0); 
}
)ddd";

const char* const unlitVert = R"ddd(
#version 130
in vec3 pos;

  uniform    mat4 perspective;
   uniform mat4 view;
uniform    mat4 transform;

void main() {
    gl_Position = perspective * view * transform * vec4(pos, 1);
    gl_PointSize = 20;
}
)ddd";

struct Attribute {
	SWAN::String Type;
	SWAN::String Name;
};

struct Uniform {
	SWAN::String Type;
	SWAN::String Name;
};

struct ShaderData {
	SWAN::Vector<Uniform> Uniforms;
	SWAN::Vector<Attribute> Attributes;
};

ShaderData ReadShader(const char* vertTxt, const char* fragTxt)
{
	ShaderData res;

	std::stringstream ssVert(vertTxt);
	SWAN::String line;

	while(std::getline(ssVert, line)) {
		SWAN::Vector<SWAN::String> lineV;

		std::stringstream ssLine(line);
		SWAN::String word;
		while(ssLine >> word)
			lineV.push_back(word);

		for(int i = 0; i < lineV.size(); i++) {
			if(lineV[i] == "in") {
				res.Attributes.push_back({ lineV[i + 1], lineV[i + 2] });
				res.Attributes.back().Name.erase(res.Attributes.back().Name.length() - 1);
				i += 2;
			} else if(lineV[i] == "uniform") {
				res.Uniforms.push_back({ lineV[i + 1], lineV[i + 2] });
				res.Uniforms.back().Name.erase(res.Uniforms.back().Name.length() - 1);
				i += 2;
			}
		}
	}

	return res;
}

int main()
{
	auto si = ReadShader(unlitVert, unlitFrag);

	SWAN::Log("Attributes:");
	for(auto a : si.Attributes)
		SWAN::Log(SWAN::Format("    {} {}", a.Type, a.Name));

	SWAN::Log("Uniforms:");
	for(auto u : si.Uniforms)
		SWAN::Log(SWAN::Format("    {} {}", u.Type, u.Name));

	return 0;
}
