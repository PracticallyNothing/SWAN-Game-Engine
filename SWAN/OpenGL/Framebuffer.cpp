#include "Framebuffer.hpp"
#include "OnGLInit.hpp"

#include <glad/glad.h>

namespace SWAN
{
	namespace GL
	{
		namespace detail
		{
			const Framebuffer* currBoundFB = nullptr;
		}

		Framebuffer::Framebuffer(unsigned width,
		                         unsigned height,
		                         bool depthAndStencil,
		                         bool depthStencilAsRenderbuf)
		{
			OnGLInit([=] {
				glGenFramebuffers(1, &fbInfo.FBId);
				Init(width, height, depthAndStencil, depthStencilAsRenderbuf);
			});
		}

		Framebuffer::~Framebuffer()
		{
			glDeleteTextures(1, &fbInfo.ColorTextureId);
			if(fbInfo.HasDepthStencil) {
				if(fbInfo.DepthStencilIsRenderbuffer)
					glDeleteRenderbuffers(1, &fbInfo.DepthStencilId);
				else
					glDeleteTextures(1, &fbInfo.DepthStencilId);
			}

			glDeleteFramebuffers(1, &fbInfo.FBId);
		}

		static unsigned GenDepthStencilRenderbuffer(unsigned width, unsigned height)
		{
			unsigned rbId;

			glGenRenderbuffers(1, &rbId);
			glBindRenderbuffer(GL_RENDERBUFFER, rbId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			return rbId;
		}

		static unsigned GenDepthStencilTexture(unsigned width, unsigned height)
		{
			unsigned depthTex;

			glGenTextures(1, &depthTex);
			glBindTexture(GL_TEXTURE_2D, depthTex);
			glTexImage2D(GL_TEXTURE_2D, 0,
			             GL_DEPTH24_STENCIL8, width, height, 0,
			             GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);

			return depthTex;
		}

		static unsigned GenColorTexture(unsigned width, unsigned height)
		{
			unsigned texId;

			glGenTextures(1, &texId);
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexImage2D(GL_TEXTURE_2D, 0,
			             GL_RGB, width, height, 0,
			             GL_RGB, GL_UNSIGNED_BYTE, nullptr);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			return texId;
		}

		void Framebuffer::Init(unsigned width,
		                       unsigned height,
		                       bool depthAndStencil,
		                       bool depthStencilAsRenderbuf)
		{
			Bind();

			fbInfo.Width = width;
			fbInfo.Height = height;

			fbInfo.ColorTextureId = GenColorTexture(width, height);
			glFramebufferTexture2D(GL_FRAMEBUFFER,
			                       GL_COLOR_ATTACHMENT0,
			                       GL_TEXTURE_2D, fbInfo.ColorTextureId, 0);

			fbInfo.HasDepthStencil = depthAndStencil;
			fbInfo.DepthStencilIsRenderbuffer = depthStencilAsRenderbuf;

			if(depthAndStencil) {
				if(!depthStencilAsRenderbuf) {
					fbInfo.DepthStencilId = GenDepthStencilTexture(width, height);

					glFramebufferTexture2D(GL_FRAMEBUFFER,
					                       GL_DEPTH_STENCIL_ATTACHMENT,
					                       GL_TEXTURE_2D, fbInfo.DepthStencilId, 0);
				} else {
					fbInfo.DepthStencilId = GenDepthStencilRenderbuffer(width, height);

					glFramebufferRenderbuffer(GL_FRAMEBUFFER,
					                          GL_DEPTH_STENCIL_ATTACHMENT,
					                          GL_RENDERBUFFER, fbInfo.DepthStencilId);
				}
			}

			Unbind();
		}

		void Framebuffer::Bind()
		{
			if(detail::currBoundFB != this) {

				glBindFramebuffer(GL_FRAMEBUFFER, fbInfo.FBId);
				detail::currBoundFB = this;
			}
		}

		void Framebuffer::Unbind()
		{
			if(detail::currBoundFB == this) {
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				detail::currBoundFB = nullptr;
			}
		}

		void Framebuffer::BindColorTexture(unsigned textureUnit) const
		{
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, fbInfo.ColorTextureId);
		}

		bool Framebuffer::BindDepthStencil(unsigned textureUnit) const
		{
			if(!fbInfo.HasDepthStencil || fbInfo.DepthStencilIsRenderbuffer)
				return false;

			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, fbInfo.DepthStencilId);

			return true;
		}

	} // namespace GL
} // namespace SWAN
