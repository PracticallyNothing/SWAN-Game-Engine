#include "Mesh.hpp"
#include "SWAN/OpenGL/VAO.hpp"

#include <GL/gl.h>

#include "SWAN/Utility/Debug.hpp"

using std::initializer_list;
using std::vector;

namespace SWAN {
    Mesh::Mesh(uint numVerts, Vertex* verts, uint numInds, uint* inds) {
	vertCount = numVerts;
	indCount  = numInds;

	init(verts, inds);
    }

    Mesh::Mesh(vector<Vertex> verts, vector<uint> inds) {
	vertCount = verts.size();
	indCount  = inds.size();

	init(verts.data(), inds.data());
    }

    Mesh::Mesh(initializer_list<Vertex> verts, vector<uint> inds) {
	Vertex* v2 = new Vertex[verts.size()];

	vertCount = verts.size();
	indCount  = inds.size();

	int i = 0;
	for(const Vertex* v = verts.begin(); v < verts.end(); ++v, ++i) {
	    v2[i] = *v;
	}

	init(v2, inds.data());
	delete[] v2;
    }

    Mesh::Mesh(vector<Vertex> verts, initializer_list<uint> inds) {
	uint* i2 = new uint[inds.size()];

	vertCount = verts.size();
	indCount  = inds.size();

	int i = 0;
	for(const uint* ind = inds.begin(); ind < inds.end(); ++ind, ++i) {
	    i2[i] = *ind;
	}

	init(verts.data(), i2);
	delete[] i2;
    }

    Mesh::Mesh(initializer_list<Vertex> verts, initializer_list<uint> inds)
    {
	Vertex* v2 = new Vertex[verts.size()];
	uint* i2   = new uint[inds.size()];

	vertCount = verts.size();
	indCount  = inds.size();

	int i = 0;
	for(const Vertex* v = verts.begin(); v < verts.end(); ++v, ++i) {
	    v2[i] = *v;
	}

	i = 0;
	for(const uint* ind = inds.begin(); ind < inds.end(); ++ind, ++i) {
	    i2[i] = *ind;
	}

	init(v2, i2);
	delete[] v2;
	delete[] i2;
    }

    void Mesh::init(Vertex* verts, uint* inds) {
	std::vector<fvec3> posV;
	std::vector<fvec2> UVs;
	std::vector<fvec3> normV;

	for(uint i = 0; i < vertCount; i++) {
	    posV.push_back(verts[i].pos);
	    UVs.push_back(verts[i].UV);
	    normV.push_back(verts[i].norm);
	}

	points = posV;
	for(int i = 0; i < indCount; i++) indices.push_back(inds[i]);

	vao.bind();
	vao.storeAttribData(0, 3, (float*) posV.data(), posV.size() * sizeof(vec3));
	vao.storeAttribData(1, 2, (float*) UVs.data(), UVs.size() * sizeof(vec2));
	vao.storeAttribData(2, 3, (float*) normV.data(), normV.size() * sizeof(vec3));
	vao.storeIndices(inds, indCount * sizeof(unsigned));
	vao.unbind();
    }
    
    void Mesh::render() const { vao.draw(indCount); }
    void Mesh::renderWireframe() const { vao.draw(indCount, GL_LINE_LOOP); }
    void Mesh::renderVerts() const { vao.draw(indCount, GL_POINTS); }
} // namespace SWAN
