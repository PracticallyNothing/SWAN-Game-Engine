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
};

typedef unsigned int uint;

class Mesh {
    template<typename T>
    using Vector = std::vector<T>;

    template<typename T>
    using InitList = std::initializer_list<T>;
public:
    [[deprecated]]
    Mesh(uint numVerts, Vertex* verts, uint numInds, uint* inds);

    Mesh(Vector<Vertex>   verts, Vector<uint>   inds);
    Mesh(Vector<Vertex>   verts, InitList<uint> inds);
    Mesh(InitList<Vertex> verts, Vector<uint>   inds);
    Mesh(InitList<Vertex> verts, InitList<uint> inds);

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
