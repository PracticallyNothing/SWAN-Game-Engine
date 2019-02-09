#ifndef SWAN_MESH_HPP
#define SWAN_MESH_HPP

#include "SWAN/OpenGL/VAO.hpp"

#include "SWAN/Maths/Vector.hpp"

#include <initializer_list>
#include <vector>

namespace SWAN
{
	class Shader;

	struct Vertex {
		vec3 pos;
		vec2 UV;
		vec3 norm;

		Vertex(vec3 pos = vec3(),
		       vec2 UV = vec2(),
		       vec3 norm = vec3(0, 0, 1))
		    : pos(pos), UV(UV), norm(norm) {}
	};

	typedef unsigned int uint;

	class Mesh
	{
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

		//	~Mesh();

		void render() const;
		void renderWireframe() const;
		void renderVerts() const;

		auto GetPoints() const { return points; }
		auto GetIndices() const { return indices; }

	  private:
		void init(Vertex* verts, uint* inds);

		std::vector<fvec3> points;
		std::vector<uint> indices;

		uint vertCount;
		uint indCount;

		GL::VAO vao;
	};
} // namespace SWAN

#endif
