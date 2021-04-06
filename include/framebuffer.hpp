#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <GL/glew.h>
#include <vector>
#include <iostream>
#include <memory>
#include <utility>

enum class ATTACHMENT_TYPE
{
	TEXTURE,
	TEXTURE_CUBE_MAP,
	RENDER_BUFFER
};

enum class ATTACHMENT_TARGET
{
	COLOR,
	DEPTH,
	STENCIL,
	DEPTH_STENCIL
};

struct Attachment
{
	GLuint id;
	ATTACHMENT_TYPE type;
	ATTACHMENT_TARGET target;
};

class Framebuffer
{
	public:

		Framebuffer(bool color = true, bool ms = false, bool hdr = false);
		~Framebuffer();

		void addAttachment(ATTACHMENT_TYPE type, ATTACHMENT_TARGET target, int width, int height, GLenum minMagFilter = GL_LINEAR, int insertPos = -1);
		void updateAttachment(ATTACHMENT_TYPE type, ATTACHMENT_TARGET target, int width, int height);
		std::vector<Attachment> & getAttachments();
		void bind();
		void unbind();
		void blitFramebuffer(Framebuffer & writeFBO, int width, int height);
		void blitFramebuffer(std::unique_ptr<Framebuffer> & writeFBO, int width, int height);
		GLuint getId();
		int getColorAttachmentCount();
		void addSingleColorTextureAttachment(GLint format, GLenum minMagFilter, int width, int height);

	private:

		GLuint fbo;
		bool renderColor;
		bool multiSample;
		bool HDR;
		std::vector<Attachment> attachment;

		// ADD ##########
		void addColorTextureAttachment(int width, int height, int insertPos, GLenum minMagFilter = GL_LINEAR);
		void addDepthTextureAttachment(int width, int height, int insertPos);
		
		void addColorTextureCubemapAttachment(int width, int height, int insertPos);
		void addDepthTextureCubemapAttachment(int width, int height, int insertPos);
		
		void addColorRenderbufferAttachment(int width, int height, int insertPos);
		void addDepthRenderbufferAttachment(int width, int height, int insertPos);
		void addDepthStencilRenderbufferAttachment(int width, int height, int insertPos);
		
		// UPDATE ##########
		void updateColorTextureAttachment(int index, int width, int height, int insertPos);
		void updateDepthTextureAttachment(int width, int height, int insertPos);
		
		void updateColorTextureCubemapAttachment(int width, int height, int insertPos);
		void updateDepthTextureCubemapAttachment(int width, int height, int insertPos);
		
		void updateColorRenderbufferAttachment(int width, int height, int insertPos);
		void updateDepthRenderbufferAttachment(int width, int height, int insertPos);
		void updateDepthStencilRenderbufferAttachment(int width, int height, int insertPos);
};

#endif
