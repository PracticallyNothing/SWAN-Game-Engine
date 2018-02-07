#ifndef SWAN_VERTEX_ARRAY_OBJECT_HPP
#define SWAN_VERTEX_ARRAY_OBJECT_HPP

#include <glad/glad.h>

namespace SWAN {
namespace GL {
	struct VAO;

	namespace detail {
		const extern VAO* currBoundVAO;
	}

	struct VAO {
		VAO() { glGenVertexArrays(1, &id); }
		~VAO() {
			unbind();
			glDeleteVertexArrays(1, &id);
		}

		inline bool isBound() const { return this == detail::currBoundVAO; }

		void bind() const {
			if(isBound())
				return;

			detail::currBoundVAO = this;
			glBindVertexArray(id);
		}

		void unbind() const {
			if(!isBound())
				return;

			detail::currBoundVAO = nullptr;
			glBindVertexArray(0);
		}

		void draw(int count, GLenum renderType = GL_TRIANGLES) const {
			bind();
			if(hasIndices)
				glDrawElements(renderType, count, GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(renderType, 0, count);
			unbind();
		}

		void storeIndices(unsigned* indices, size_t size, GLenum drawType = GL_STATIC_DRAW) {
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

		void storeAttribData(unsigned attribNumber, size_t glNumComponents, float* data, size_t dataSize) {
			if(!data || dataSize == 0) // No data
				return;

			bind();

			GLuint vbo;
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
			glEnableVertexAttribArray(attribNumber);
			glVertexAttribPointer(attribNumber, glNumComponents, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			unbind();
		}

		GLuint id = 0;

		bool hasIndices = false;
	};
} // namespace GL
} // namespace SWAN

#endif //SWAN_VERTEX_ARRAY_OBJECT_HPP
