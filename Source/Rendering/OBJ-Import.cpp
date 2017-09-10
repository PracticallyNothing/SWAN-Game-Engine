#include "../Rendering/OBJ-Import.hpp"

#include <algorithm>  // For std::find(), std::distance()
#include <array>      // For std::array<T,N>
#include <fstream>    // For std::file
#include <iostream>   // For std::cout
#include <sstream>    // For std::stringstream
#include <string>     // For std::string, std::stoi(), std::stof()
#include <vector>     // For std::vector<T>

#include "../Utility/Collect.hpp"  // For Util::CollectIterIf()
#include "../Utility/Debug.hpp"    // For DEBUG_OUT()
#include "../Utility/Profile.hpp"  // For UTIL_PROFILE()
#include "../Utility/StringUtil.hpp"

#include <cctype>  // For std::isspace()
using std::isspace;

using std::find;
using std::distance;
using std::fill;

using std::cout;
using std::ifstream;
using std::string;
using std::vector;
using std::array;
using std::stringstream;

using std::stof;
using std::unique_ptr;
using std::make_unique;

struct XVertex {
	XVertex(glm::vec3 pos = glm::vec3(), glm::vec2 UV = glm::vec2(),
			glm::vec3 norm = glm::vec3(), bool hasUV = true,
			bool hasNorm = true)
		: pos(pos), UV(UV), norm(norm), hasUV(hasUV), hasNorm(hasNorm) {}

	glm::vec3 pos;
	glm::vec2 UV;
	glm::vec3 norm;

	bool hasUV = true;
	bool hasNorm = true;

	operator Vertex() {
		return Vertex(pos, (hasUV ? UV : glm::vec2()),
				(hasNorm ? norm : glm::vec3()));
	}
};

bool operator==(const Vertex& lhs, const XVertex& rhs) {
	return lhs.pos == rhs.pos && lhs.UV == rhs.UV && lhs.norm == rhs.norm;
}

bool operator==(const XVertex& lhs, const Vertex& rhs) {
	return lhs.pos == rhs.pos && lhs.UV == rhs.UV && lhs.norm == rhs.norm;
}
struct Face {
	vector<XVertex> verts;
};
struct Model {
	vector<Face> faces;
};

enum ReadMode {
	READ_NONE,
	READ_POS,
	READ_UV,
	READ_NORM,
	READ_FACE,
};

Model importOBJ(string filename) {
	using Util::Trim;
	using Util::ReadFace;
	using Util::SplitOn;

	Model res;

	ifstream file(filename.c_str());
	if (!file) {
		// TODO: throw exception
		return res;
	}

	string line;
	size_t linenum = 0;
	ReadMode mode = READ_NONE;

	vector<glm::vec3> pos;
	vector<glm::vec2> UVs;
	vector<glm::vec3> norms;

	while (!file.eof()) {
		std::getline(file, line);

		if (line[0] == '#' || line.length() < 2) {
			// Skip the line, it's either a comment or an empty line.
			continue;
		}

		switch (line[0]) {
			case 'f':
				mode = READ_FACE;
				break;
			case 'v':
				switch (line[1]) {
					case ' ':
						mode = READ_POS;
						break;
					case 't':
						mode = READ_UV;
						break;
					case 'n':
						mode = READ_NORM;
						break;
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

		switch (mode) {
			case READ_NONE:
				break;
			case READ_POS:
				{
					auto vec = SplitOn(line);
					pos.push_back(glm::vec3{stof(vec[1]),    // X
							stof(vec[2]),    // Y
							stof(vec[3])});  // Z
					break;
				}
			case READ_UV:
				{
					auto vec = SplitOn(line);
					UVs.push_back(glm::vec2{stof(vec[1]),    // X
							stof(vec[2])});  // Y
					break;
				}
			case READ_NORM:
				{
					auto vec = SplitOn(line);
					norms.push_back(glm::vec3{stof(vec[1]),    // X
							stof(vec[2]),    // Y
							stof(vec[3])});  // Z
					break;
				}
			case READ_FACE:
				{
					auto contents = SplitOn(line);
					Face face;

					for (size_t i = 1; i < contents.size(); i++) {
						XVertex vert;
						auto faceV = ReadFace(contents[i]);

						vert.pos = pos.at(faceV[0] - 1);

						vert.hasUV = faceV[1];
						if (vert.hasUV) vert.UV = UVs.at(faceV[2] - 1);

						vert.hasNorm = faceV[3];
						if (vert.hasNorm) vert.norm = norms.at(faceV[4] - 1);

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

unique_ptr<Mesh> Import::OBJ(std::string filename, Import::Settings s) {
	UTIL_PROFILE();

	Model m = importOBJ(filename);

	vector<Vertex> rVerts;
	vector<uint> rInds;

	int i = 0;
	for (auto& face : m.faces) {
		for (auto& vert : face.verts) {
			auto it = find(rVerts.begin(), rVerts.end(), vert);

			if (it == rVerts.end()) {
				Vertex v(vert.pos, (vert.hasUV ? vert.UV : glm::vec2()),
						(vert.hasNorm ? vert.norm : glm::vec3()));
				rVerts.push_back(v);
				rInds.push_back(i++);
			} else {
				rInds.push_back(distance(rVerts.begin(), it));
			}
		}
	}

	if (s.smoothNormals) {
		vector<bool> checked(rVerts.size());

		fill(checked.begin(), checked.end(), false);

		for (unsigned i = 0; i < rVerts.size(); i++) {
			if (checked[i]) continue;

			auto v = Util::CollectIterIf(rVerts.begin(), rVerts.end(),
					[& target = rVerts[i]](Vertex v)->bool {
					return v.pos == target.pos;
					}
					);

			glm::vec3 resNorm = glm::vec3(0, 0, 0);
			for (auto it : v) resNorm += it->norm;

			if (glm::length(resNorm) != 0.0) resNorm = glm::normalize(resNorm);

			for (auto it : v) {
				checked.at(distance(rVerts.begin(), it)) = true;
				it->norm = resNorm;
			}
		}
	}

	auto res = make_unique<Mesh>(rVerts, rInds);

	auto genAABB = [](vector<Vertex> v) -> AABB {
		UTIL_PROFILE();
		AABB res;

		res.min = v[0].pos;
		res.max = v[1].pos;

		for (const auto& vert : v) {
			if (vert.pos.x < res.min.x) {
				res.min.x = vert.pos.x;
			} else if (vert.pos.x > res.max.x) {
				res.max.x = vert.pos.x;
			}

			if (vert.pos.y < res.min.y) {
				res.min.y = vert.pos.y;
			} else if (vert.pos.y > res.max.y) {
				res.max.y = vert.pos.y;
			}

			if (vert.pos.z < res.min.z) {
				res.min.z = vert.pos.z;
			} else if (vert.pos.z > res.max.z) {
				res.max.z = vert.pos.z;
			}
		}

		res.min -= glm::vec3(0.002, 0.002, 0.002);
		res.max += glm::vec3(0.002, 0.002, 0.002);

		return res;
	};

	if (s.createAABB || s.colType == ColWrapper::COL_AABB) {
		res->setAABB(genAABB(rVerts));
		res->setColWrapper(ColWrapper(res->getAABB()));
	} /*else if (s.colType == ColWrapper::COL_SPHERE) {
		BoundingSphere sphere;

		// Really ugly (and probably slow) method, but gives good results.
		// To find the proper center of the mesh, we create its AABB and use
		// its center as the sphere's.
		auto box = genAABB(rVerts);
		sphere.center = (box.max - box.min) / 2.0f;

		sphere.radius = std::max(box.max.x - box.min.x, std::max(box.max.y - box.min.y, box.max.z - box.min.z)) / 2.0f;

		res->setColWrapper(ColWrapper(sphere));
	}
	*/

	return res;
}
