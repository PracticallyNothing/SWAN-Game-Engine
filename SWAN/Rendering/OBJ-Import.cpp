#include "Rendering/OBJ-Import.hpp"

#include <algorithm> // For std::find(), std::distance()
#include <array>     // For std::array<T,N>
#include <fstream>   // For std::file
#include <iostream>  // For std::cout
#include <sstream>   // For std::stringstream
#include <string>    // For std::string, std::stoi(), std::stof()
#include <vector>    // For std::vector<T>

#include "Utility/Collect.hpp" // For SWAN::Util::CollectIterIf()
#include "Utility/Debug.hpp"   // For SWAN_DEBUG_OUT()
#include "Utility/Profile.hpp" // For UTIL_PROFILE()
#include "Utility/StreamOps.hpp"
#include "Utility/StringUtil.hpp"

#include "Maths/Vector.hpp"

#include <cctype> // For std::isspace()
using std::isspace;

using std::distance;
using std::fill;
using std::find;

using std::array;
using std::cout;
using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;

using std::make_unique;
using std::stof;
using std::unique_ptr;

using namespace SWAN;
using namespace SWAN::Util::StreamOps;

struct XVertex {
    XVertex(vec3 pos = vec3(), vec2 UV = vec2(),
	    vec3 norm = vec3(), bool hasUV = true,
	    bool hasNorm = true)
	: pos(pos), UV(UV), norm(norm), hasUV(hasUV), hasNorm(hasNorm) {}

    vec3 pos;
    vec2 UV;
    vec3 norm;

    bool hasUV   = true;
    bool hasNorm = true;

    operator SWAN::Vertex() {
	return SWAN::Vertex(pos,
			    (hasUV ? UV : vec2()),
			    (hasNorm ? norm : vec3()));
    }
};

bool operator==(const SWAN::Vertex& lhs, const XVertex& rhs) {
    return lhs.pos == rhs.pos && lhs.UV == rhs.UV && lhs.norm == rhs.norm;
}

bool operator==(const XVertex& lhs, const SWAN::Vertex& rhs) {
    return lhs.pos == rhs.pos && lhs.UV == rhs.UV && lhs.norm == rhs.norm;
}

struct Face { vector<XVertex> verts; };
struct Model { vector<Face> faces; };

enum ReadMode {
	       READ_NONE,
	       READ_POS,
	       READ_UV,
	       READ_NORM,
	       READ_FACE,
	       READ_MTL,
	       READ_NAME
};

Model importOBJ(string filename) {
    using SWAN::Util::ReadFace;
    using SWAN::Util::SplitOn;
    using SWAN::Util::Trim;

    Model res;

    ifstream file(filename.c_str());
    if(!file) {
	// TODO: throw exception
	return res;
    }

    string line;
    size_t linenum = 0;
    ReadMode mode  = READ_NONE;

    vector<vec3> pos;
    vector<vec2> UVs;
    vector<vec3> norms;

    while(std::getline(file, line)) {

	if(line[0] == '#' || line.length() < 2) {
	    // Skip the line, it's either a comment or an empty line.
	    continue;
	}

	switch(line[0]) {
	case 'f': mode = READ_FACE; break;
	case 'v':
	    switch(line[1]) {
	    case ' ': mode = READ_POS; break;
	    case 't': mode = READ_UV; break;
	    case 'n': mode = READ_NORM; break;
	    default:
		/*cout << "ERROR: Unknown or unsupported vertex spec 'v"
		  << line[1] << "' on line " << linenum
		  << ", skipping...\n";*/
		break;
	    }
	    break;
	default:
	    /*cout << "ERROR: Unknown or unsupported spec '" << line[0]
	      << "' on line " << linenum << ",\n    " << line
	      << "\nskipping...\n";*/
	    break;
	}

	switch(mode) {
	case READ_NONE: break;
	case READ_POS: {
	    auto vec = SplitOn(line);
	    pos.push_back(vec3{ stof(vec[1]),    // X
				stof(vec[2]),    // Y
				stof(vec[3]) }); // Z
	    break;
	}
	case READ_UV: {
	    auto vec = SplitOn(line);
	    UVs.push_back(vec2{ stof(vec[1]),    // X
				stof(vec[2]) }); // Y
	    break;
	}
	case READ_NORM: {
	    auto vec = SplitOn(line);
	    norms.push_back(vec3{ stof(vec[1]),    // X
				  stof(vec[2]),    // Y
				  stof(vec[3]) }); // Z
	    break;
	}
	case READ_FACE: {
	    auto contents = SplitOn(line);
	    Face face;

	    for(size_t i = 1; i < contents.size(); i++) {
		XVertex vert;
		auto faceV = ReadFace(contents[i]);

		vert.pos = pos.at(faceV[0] - 1);

		vert.hasUV = faceV[1];
		if(vert.hasUV)
		    vert.UV = UVs.at(faceV[2] - 1);

		vert.hasNorm = faceV[3];
		if(vert.hasNorm)
		    vert.norm = norms.at(faceV[4] - 1);

		face.verts.push_back(vert);
	    }

	    res.faces.push_back(face);

	    break;
	}
	}

	linenum++;
	mode = READ_NONE;
    }

    return res;
}

namespace SWAN {
    using namespace Util::StreamOps;
    unique_ptr<Mesh> Import::OBJ(std::string filename, Import::Settings s) {
	//SWAN_UTIL_PROFILE();

	Model m = importOBJ(filename);

	/*
	cout << "Model: \n";
	for(auto& face : m.faces) {
	    cout << "    Face: ";
	    for(auto& v : face.verts)
		cout << "(pos: " << v.pos << ", UV: " << v.UV << ", norm: " << v.norm << "), ";
	    cout << '\n';
	}
	*/

	vector<Vertex> rVerts;
	vector<uint> rInds;

	int i = 0;
	for(auto& face : m.faces) {
	    for(auto& vert : face.verts) {
		auto it = find(rVerts.begin(), rVerts.end(), vert);

		if(it == rVerts.end()) {
		    Vertex v(vert.pos, (vert.hasUV ? vert.UV : vec2()),
			     (vert.hasNorm ? vert.norm : vec3()));
		    rVerts.push_back(v);
		    rInds.push_back(i++);
		} else {
		    rInds.push_back(distance(rVerts.begin(), it));
		}
	    }
	}

	if(s.smoothNormals) {
	    vector<bool> checked(rVerts.size());

	    fill(checked.begin(), checked.end(), false);

	    for(unsigned i = 0; i < rVerts.size(); i++) {
		if(checked[i])
		    continue;

		auto v =
		    SWAN::Util::CollectIterIf(rVerts.begin(),
					      rVerts.end(),
					      [& target = rVerts[i]](Vertex v) -> bool {
						  return v.pos == target.pos;
					      });

		vec3 resNorm = vec3(0, 0, 0);
		for(auto it : v)
		    resNorm += it->norm;

		if(Length(resNorm) != 0.0)
		    resNorm = Normalized(resNorm);

		for(auto it : v) {
		    checked.at(distance(rVerts.begin(), it)) = true;
		    it->norm = resNorm;
		}
	    }
	}

	auto res = make_unique<Mesh>(rVerts, rInds);
	return res;
    }
} // namespace SWAN
