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
		VAO();
		~VAO();

		inline bool isBound() const { return this == detail::currBoundVAO; }

		void bind() const;
		void unbind() const;

		void draw(int count, GLenum renderType = GL_TRIANGLES) const;
		void storeIndices(unsigned* indices, size_t size, GLenum drawType = GL_STATIC_DRAW);

		void storeAttribData(unsigned attribNumber, size_t glNumComponents, float* data, size_t dataSize);

		GLuint id = 0;

		bool hasIndices = false;
	};
} // namespace GL
} // namespace SWAN

#endif //SWAN_VERTEX_ARRAY_OBJECT_HPP
