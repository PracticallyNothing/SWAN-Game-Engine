#include "VAO.hpp"

namespace SWAN {
namespace GL {
	namespace detail {
		const VAO* currBoundVAO = nullptr;
	}

	VAO::VAO() {
		glGenVertexArrays(1, &id);
	}

	VAO::~VAO() {
		unbind();
		glDeleteVertexArrays(1, &id);
	}

	void VAO::bind() const {
		if(isBound())
			return;

		detail::currBoundVAO = this;
		glBindVertexArray(id);
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
	}

	void VAO::storeIndices(unsigned* indices, size_t size, GLenum drawType) {
		if(!indices || size == 0)
			return;

		bind();

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		unbind();
		hasIndices = true;
	}

	void VAO::storeAttribData(unsigned attribNumber, size_t glNumComponents, float* data, size_t dataSize) {
		if(!data || dataSize == 0) // No data
			return;

		bind();

		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
		glVertexAttribPointer(attribNumber, glNumComponents, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(attribNumber);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		unbind();
	}
} // namespace GL
} // namespace SWAN
