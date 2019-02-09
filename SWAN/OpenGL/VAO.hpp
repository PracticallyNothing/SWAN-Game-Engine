#ifndef SWAN_VERTEX_ARRAY_OBJECT_HPP
#define SWAN_VERTEX_ARRAY_OBJECT_HPP

#include <glad/glad.h>
#include <map>

namespace SWAN
{
	namespace GL
	{
		struct VAO;

		namespace detail
		{
			const extern VAO* currBoundVAO;
			extern unsigned long numInits;
			extern unsigned long numDeletions;
			extern unsigned long numBinds;
			extern unsigned long numDrawCalls;
		} // namespace detail

		/// A structure describing an OpenGL vertex array object.
		struct VAO {
			/// Construct a VAO.
			VAO();
			/// Destroy a VAO.
			~VAO();

			/// Is this VAO currently bound?
			inline bool isBound() const { return this == detail::currBoundVAO; }

			/// Bind this VAO (if necessary).
			void bind() const;

			/// Unbind this VAO (if necessary).
			void unbind() const;

			/// Render the contents of the VAO onscreen.
			void draw(int count, GLenum renderType = GL_TRIANGLES) const;

			/// Add indices to the VAO.
			void storeIndices(const unsigned* indices, size_t size, GLenum drawType = GL_STATIC_DRAW);

			/// Add an attribute to the VAO.
			void storeAttribData(unsigned attribNumber,
			                     size_t glNumComponents,
			                     const float* data,
			                     size_t dataSize,
			                     GLenum drawType = GL_STATIC_DRAW);

			/// OpenGL ID for the VAO.
			GLuint id = 0;

			/// Whether the VAO has indices.
			/// @warning Do not change this manually, it may cause rendering errors. Use storeIndices() instead.
			bool hasIndices = false;

			/// ID of index VBO.
			GLuint indexBuffer = 0;
			/// A mapping of attribute numbers to the VBOs that store their data.
			std::map<unsigned, GLuint> attribVBOs;
		};
	} // namespace GL
} // namespace SWAN

#endif //SWAN_VERTEX_ARRAY_OBJECT_HPP
