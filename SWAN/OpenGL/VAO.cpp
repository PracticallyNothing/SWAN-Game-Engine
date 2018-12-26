#include "VAO.hpp"

#include "OnGLInit.hpp"

namespace SWAN {
    namespace GL {
	namespace detail {
	    const VAO* currBoundVAO = nullptr;
	    unsigned long numInits = 0;
	    unsigned long numBinds = 0;
	    unsigned long numDrawCalls = 0;
	    unsigned long numDeletions = 0;
	}

	VAO::VAO() {
	    if(IsGLInitialized())
		glGenVertexArrays(1, &id);
	    else
		OnGLInit([this]() mutable { glGenVertexArrays(1, &id); });

	    detail::numInits++;
	}

	VAO::~VAO() {
	    unbind();
	    glDeleteVertexArrays(1, &id);
	    detail::numDeletions++;
	}

	void VAO::bind() const {
	    if(isBound())
		return;

	    detail::currBoundVAO = this;
	    glBindVertexArray(id);

	    detail::numBinds++;
	}

	void VAO::unbind() const {
	    if(!isBound())
		return;

	    detail::currBoundVAO = nullptr;
	    glBindVertexArray(0);
	}

	void VAO::draw(int count, GLenum renderType) const {
	    bind();
	    if(hasIndices)
		glDrawElements(renderType, count, GL_UNSIGNED_INT, 0);
	    else
		glDrawArrays(renderType, 0, count);
	    unbind();

	    detail::numDrawCalls++;
	}

	void VAO::storeIndices(const unsigned* indices, size_t size, GLenum drawType) {
	    if(!indices || size == 0)
		return;

	    bind();

	    if(indexBuffer == 0) {glGenBuffers(1, &indexBuffer);}
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	    glBindBuffer(GL_ARRAY_BUFFER, 0);

	    unbind();
	    hasIndices = true;
	}

	void VAO::storeAttribData(unsigned attribNumber,
				  size_t glNumComponents,
				  const float* data,
				  size_t dataSize,
				  GLenum drawType) {
	    if(!data || dataSize == 0) // No data
		return;

	    bind();

	    auto vboIter = attribVBOs.find(attribNumber);
	    if(vboIter == attribVBOs.end())
	    {
		GLuint vbo;
		glGenBuffers(1, &vbo);
		attribVBOs.insert({attribNumber, vbo});
		vboIter = attribVBOs.find(attribNumber);
	    }

	    glBindBuffer(GL_ARRAY_BUFFER, vboIter->second);
	    glBufferData(GL_ARRAY_BUFFER, dataSize, data, drawType);
	    glVertexAttribPointer(attribNumber, glNumComponents, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(attribNumber);

	    glBindBuffer(GL_ARRAY_BUFFER, 0);
	    unbind();
	}
    } // namespace GL
} // namespace SWAN
