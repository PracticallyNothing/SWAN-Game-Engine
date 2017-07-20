#ifndef MESH_HPP
#define MESH_HPP

#include <initializer_list>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 pos;
	glm::vec2 UV;
    glm::vec3 norm;

    Vertex(glm::vec3 pos = glm::vec3(), glm::vec2 UV = glm::vec2(),
           glm::vec3 norm = glm::vec3(0, 0, 1))
        : pos(pos), UV(UV), norm(norm) {}

    void setPos (glm::vec3 pos)  { this->pos = pos; }
    void setUV  (glm::vec2 UV)   { this->UV = UV; }
    void setNorm(glm::vec3 norm) { this->norm = norm; }

    glm::vec3 getPos() { return pos; }
    glm::vec2 getUV() { return UV; }
    glm::vec3 getNorm() { return norm; }
};

typedef unsigned int uint;

class Mesh {
public:
    Mesh(uint numVerts, Vertex* verts, uint numInds, uint* inds);

    Mesh(std::vector<Vertex> verts, std::vector<uint> inds);
    Mesh(std::initializer_list<Vertex> verts, std::vector<uint> inds);
    Mesh(std::vector<Vertex> verts, std::initializer_list<uint> inds);
    Mesh(std::initializer_list<Vertex> verts, std::initializer_list<uint> inds);

    ~Mesh();

    void render() const;
    void renderWireframe() const;
	void renderVerts() const;
private:
    void init(Vertex* verts, uint* inds);

    uint vertCount;
    uint indCount;

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

#endif
