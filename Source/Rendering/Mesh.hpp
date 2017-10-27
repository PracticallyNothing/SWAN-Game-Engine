#ifndef SWAN_MESH_HPP
#define SWAN_MESH_HPP

#include "Physics/AABB.hpp"
#include "Physics/ColWrapper.hpp"

#include <initializer_list>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace SWAN {
class Shader;

struct Vertex {
	glm::vec3 pos;
	glm::vec2 UV;
	glm::vec3 norm;

	Vertex(glm::vec3 pos = glm::vec3(), glm::vec2 UV = glm::vec2(),
	       glm::vec3 norm = glm::vec3(0, 0, 1))
	    : pos(pos), UV(UV), norm(norm) {}
};

typedef unsigned int uint;

class Mesh {
	template <typename T>
	using Vector = std::vector<T>;
	template <typename T>
	using InitList = std::initializer_list<T>;

	friend class Shader;

  public:
	Mesh(uint numVerts, Vertex* verts, uint numInds, uint* inds);

	Mesh(Vector<Vertex> verts, Vector<uint> inds);
	Mesh(Vector<Vertex> verts, InitList<uint> inds);
	Mesh(InitList<Vertex> verts, Vector<uint> inds);
	Mesh(InitList<Vertex> verts, InitList<uint> inds);

	~Mesh();

	// TODO: Deprecate in favour of ColWrappers
	inline bool hasAABB() const { return _hasAABB; }
	inline auto getAABB() const { return aabb; }
	inline void setAABB(AABB newAABB) {
		aabb     = newAABB;
		_hasAABB = true;
	}

	inline bool hasColWrapper() const { return _hasColWrapper; }
	inline auto getColWrapper() const { return colWrapper; }
	inline void setColWrapper(ColWrapper newColWrapper) {
		colWrapper     = newColWrapper;
		_hasColWrapper = true;
	}

  private:
	void render() const;
	void renderWireframe() const;
	void renderVerts() const;

	void init(Vertex* verts, uint* inds);

	uint vertCount;
	uint indCount;

	bool _hasAABB = false;
	AABB aabb;

	bool _hasColWrapper = false;
	ColWrapper colWrapper;

	enum {
		VBO_POS,
		VBO_UV,
		VBO_NORM,

		VBO_INDEX,

		VBO_TOTAL
	};

	GLuint vaoID;
	GLuint vboID[VBO_TOTAL];
};
}

#endif
