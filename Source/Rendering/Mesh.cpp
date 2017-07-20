#include "Mesh.hpp"

#include <GL/gl.h>

Mesh::Mesh(uint numVerts, Vertex* verts, uint numInds, uint* inds) {
    this->vertCount = numVerts;
    this->indCount = numInds;
    init(verts, inds);
}

Mesh::Mesh(std::vector<Vertex> verts, std::vector<uint> inds) {
    vertCount = verts.size();
    indCount = inds.size();

    init(&verts[0], &inds[0]);
}

Mesh::Mesh(std::initializer_list<Vertex> verts, std::vector<uint> inds) {
    Vertex* v2 = new Vertex[verts.size()];

    vertCount = verts.size();
    indCount = inds.size();

    int i = 0;
    for (const Vertex *v = verts.begin(); v < verts.end(); ++v, ++i) {
        v2[i] = *v;
    }

    init(v2, &inds[0]);
    delete[] v2;
}

Mesh::Mesh(std::vector<Vertex> verts, std::initializer_list<uint> inds) {
    uint* i2 = new uint[inds.size()];

    vertCount = verts.size();
    indCount = inds.size();

    int i = 0;
    for (const uint *ind = inds.begin(); ind < inds.end(); ++ind, ++i) {
        i2[i] = *ind;
    }

    init(&verts[0], i2);
    delete[] i2;
}

Mesh::Mesh(std::initializer_list<Vertex> verts,
           std::initializer_list<uint> inds) {
    Vertex* v2 = new Vertex[verts.size()];
    vertCount = verts.size();

    uint* i2 = new uint[inds.size()];
    indCount = inds.size();

    int i = 0;
    for (const Vertex *v = verts.begin(); v < verts.end(); ++v, ++i) {
        v2[i] = *v;
    }
    i = 0;

    for (const uint *ind = inds.begin(); ind < inds.end(); ++ind, ++i) {
        i2[i] = *ind;
    }

    init(v2, i2);
    delete[] v2;
    delete[] i2;
}

void Mesh::init(Vertex* verts, uint* inds) {
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    std::vector<glm::vec3> posV;
    std::vector<glm::vec2> texCoordV;
    std::vector<glm::vec3> normV;

    for (uint i = 0; i < vertCount; i++) {
        posV.push_back(verts[i].pos);
        texCoordV.push_back(verts[i].UV);
        normV.push_back(verts[i].norm);
    }

    glGenBuffers(VBO_TOTAL, vboID);

    //---------------Positions---------------//
    glBindBuffer(GL_ARRAY_BUFFER, vboID[VBO_POS]);
    glBufferData(GL_ARRAY_BUFFER, posV.size() * sizeof(posV[0]), &posV[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(VBO_POS);  // This enables the first vertex attribute

    glVertexAttribPointer(
        VBO_POS,   // index 	 - This is the Vertex Attribute we just enabled, we'll be changing that
		3, 		   // size 		 - Since this attribute is the vertex's position, i.e. vec3, we need an attribute of size 3
		GL_FLOAT,  // type 		 - Specifies the type of the attribute, or in this case, floats for the positions in OpenGL coordinates [-1.0, 1.0]
		GL_FALSE,  // normalized - If these are vertex normals, this should be GL_TRUE
		0,   	   // stride 	 - This tells OpenGL how we are going to pass in the vertex attribute data. 0 means we are sending it all in one go.
		0);  	   // pointer 	 - A pointer that represents the offset of the data we are sending. If stride is 0, this should be zero too.

    //---------------Texture coordinates---------------//
    glBindBuffer(GL_ARRAY_BUFFER, vboID[VBO_UV]);
    glBufferData(GL_ARRAY_BUFFER, texCoordV.size() * sizeof(texCoordV[0]),
                 &texCoordV[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(VBO_UV);
    glVertexAttribPointer(VBO_UV, 2, GL_FLOAT, GL_FALSE, 0, 0);

    //---------------Normals---------------//
    glBindBuffer(GL_ARRAY_BUFFER, vboID[VBO_NORM]);
    glBufferData(GL_ARRAY_BUFFER, normV.size() * sizeof(normV[0]), &normV[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(VBO_NORM);
    glVertexAttribPointer(VBO_NORM, 3, GL_FLOAT, GL_TRUE, 0, 0);

    //---------------Indices---------------//
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID[VBO_INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indCount * sizeof(uint), inds,
                 GL_STATIC_DRAW);

    glBindVertexArray(0);
}

Mesh::~Mesh() { glDeleteVertexArrays(1, &vaoID); }

void Mesh::render() const {
    glBindVertexArray(vaoID);
    // glDrawArrays(GL_TRIANGLES, 0, drawCount);
    glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::renderWireframe() const {
    glBindVertexArray(vaoID);
    // glDrawArrays(GL_TRIANGLES, 0, drawCount);
    glDrawElements(GL_LINE_LOOP, indCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}


void Mesh::renderVerts() const {
    glBindVertexArray(vaoID);
    // glDrawArrays(GL_TRIANGLES, 0, drawCount);
    glDrawElements(GL_POINTS, indCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
