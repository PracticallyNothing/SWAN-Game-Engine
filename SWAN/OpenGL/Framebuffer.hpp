#ifndef SWAN_GL_FRAMEBUFFER_HPP
#define SWAN_GL_FRAMEBUFFER_HPP

namespace SWAN
{
	namespace GL
	{
		struct Framebuffer;

		namespace detail
		{
			const extern Framebuffer* currBoundFB;
		}

		struct FBInfo {
			/// The OpenGL Id of the framebuffer.
			unsigned FBId = 0;

			unsigned Width = 0, Height = 0;

			/// The OpenGL Id for Color attachment
			/// Texture that the FB uses.
			unsigned ColorTextureId = 0;

			/// Does the FB have a
			/// depth+stencil attachment?
			bool HasDepthStencil = false;

			/// The OpenGL Id for
			/// the FB's depth+stencil attachment.
			unsigned DepthStencilId = 0;

			/// Whether the depth+stencil attachment
			/// is a renderbuffer.
			/// True means it's a renderbuffer,
			/// false means it's a texture.
			bool DepthStencilIsRenderbuffer = false;
		};

		class Framebuffer
		{
		  public:
			Framebuffer(unsigned width,
			            unsigned height,
			            bool depthAndStencil = true,
			            bool depthStencilAsRenderbuf = true);
			~Framebuffer();

			void Bind();
			void Unbind();

			void BindColorTexture(unsigned textureUnit = 0) const;

			/// Binds the depth+stencil attachment
			/// if it's a texture, otherwise does nothing.
			/// Returns whether anything was bound.
			bool BindDepthStencil(unsigned textureUnit = 0) const;

			FBInfo GetFBInfo() const { return fbInfo; }

		  private:
			void Init(unsigned width,
			          unsigned height,
			          bool depthAndStencil = true,
			          bool depthStencilAsRenderbuf = true);
			FBInfo fbInfo;
		};
	} // namespace GL
} // namespace SWAN

#endif
