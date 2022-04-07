#include "framebuffer.hpp"

Framebuffer::Framebuffer(bool color, bool ms, bool hdr) :
	renderColor(color),
	multiSample(ms),
	HDR(hdr)
{
	glGenFramebuffers(1, &fbo);
}

Framebuffer::~Framebuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	for(int i{0}; i < attachment.size(); ++i)
	{
		if(attachment[i].type == ATTACHMENT_TYPE::TEXTURE)
		{
			switch(attachment.at(i).target)
			{
				case ATTACHMENT_TARGET::COLOR:
					if(multiSample)
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, 0, 0);
					else
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
					break;
				case ATTACHMENT_TARGET::DEPTH:
					if(multiSample)
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, 0, 0);
					else
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
					break;
				default:
					break;
			}
			glDeleteTextures(1, &attachment[i].id);
		}
		else if(attachment[i].type == ATTACHMENT_TYPE::TEXTURE_CUBE_MAP)
		{
			switch(attachment[i].target)
			{
				case ATTACHMENT_TARGET::COLOR:
					glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
					break;
				case ATTACHMENT_TARGET::DEPTH:
					glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, 0, 0);
					break;
				default:
					break;
			}
			glDeleteTextures(1, &attachment[i].id);
		}
		else if(attachment[i].type == ATTACHMENT_TYPE::RENDER_BUFFER)
		{
			switch(attachment[i].target)
			{
				case ATTACHMENT_TARGET::COLOR:
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
					break;
				case ATTACHMENT_TARGET::DEPTH:
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
					break;
				case ATTACHMENT_TARGET::STENCIL:
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
					break;
				case ATTACHMENT_TARGET::DEPTH_STENCIL:
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
					break;
				default:
					break;
			}
			glDeleteRenderbuffers(1, &attachment[i].id);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &fbo);
}

void Framebuffer::addAttachment(ATTACHMENT_TYPE type, ATTACHMENT_TARGET target, int width, int height, GLenum minMagFilter, int insertPos)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	if(type == ATTACHMENT_TYPE::TEXTURE)
	{
		switch(target)
		{
			case ATTACHMENT_TARGET::COLOR:
				addColorTextureAttachment(width, height, insertPos, minMagFilter);
				break;
			case ATTACHMENT_TARGET::DEPTH:
				addDepthTextureAttachment(width, height, insertPos);
				break;
			default:
				break;
		}
	}
	else if(type == ATTACHMENT_TYPE::TEXTURE_CUBE_MAP)
	{
		switch(target)
		{
			case ATTACHMENT_TARGET::COLOR:
				addColorTextureCubemapAttachment(width, height, insertPos);
				break;
			case ATTACHMENT_TARGET::DEPTH:
				addDepthTextureCubemapAttachment(width, height, insertPos);
				break;
			default:
				break;
		}
	}
	else if(type == ATTACHMENT_TYPE::RENDER_BUFFER)
	{
		switch(target)
		{
			case ATTACHMENT_TARGET::COLOR:
				addColorRenderbufferAttachment(width, height, insertPos);
				break;
			case ATTACHMENT_TARGET::DEPTH:
				addDepthRenderbufferAttachment(width, height, insertPos);
				break;
			case ATTACHMENT_TARGET::DEPTH_STENCIL:
				addDepthStencilRenderbufferAttachment(width, height, insertPos);
				break;
			default:
				break;
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::addColorTextureAttachment(int width, int height, int insertPos, GLenum minMagFilter)
{
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

	struct Attachment buffer;
	buffer.type = ATTACHMENT_TYPE::TEXTURE;
	buffer.target = ATTACHMENT_TARGET::COLOR;
	GLint internalFormat = (HDR) ? GL_RGBA16F : GL_RGBA;
	GLenum type = (HDR) ? GL_FLOAT : GL_UNSIGNED_BYTE;
	int count{getColorAttachmentCount()};

	if(multiSample)
	{
		glGenTextures(1, &buffer.id);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, buffer.id);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, internalFormat, width, height, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, minMagFilter);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, minMagFilter);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + count, GL_TEXTURE_2D_MULTISAMPLE, buffer.id, 0);
	}
	else
	{
		glGenTextures(1, &buffer.id);
		glBindTexture(GL_TEXTURE_2D, buffer.id);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, GL_RGBA, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + count, GL_TEXTURE_2D, buffer.id, 0);
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: framebuffer is not complete !" << std::endl;
	else
	{
		if(insertPos == -1)
			attachment.push_back(buffer);
		else
			attachment.insert(attachment.begin() + insertPos, buffer);
	}

	int index{0};
	std::vector<GLenum> colorAttachments;
	for(int i{0}; i < attachment.size(); ++i)
	{
		if(attachment.at(i).target == ATTACHMENT_TARGET::COLOR)
		{
			colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + index);
			index++;
		}
	}
	glDrawBuffers(colorAttachments.size(), colorAttachments.data());
}

void Framebuffer::addDepthTextureAttachment(int width, int height, int insertPos)
{
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
	
	struct Attachment buffer;
	buffer.type = ATTACHMENT_TYPE::TEXTURE;
	buffer.target = ATTACHMENT_TARGET::DEPTH;
	GLint internalFormat = (HDR) ? GL_RGBA16F : GL_RGBA;

	if(multiSample)
	{
		glGenTextures(1, &buffer.id);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, buffer.id);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT32F, width, height, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D_MULTISAMPLE, buffer.id, 0);
	}
	else
	{
		glGenTextures(1, &buffer.id);
		glBindTexture(GL_TEXTURE_2D, buffer.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, buffer.id, 0);
	}

	if(!renderColor)
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: framebuffer is not complete !" << std::endl;
	else
	{
		if(insertPos == -1)
			attachment.push_back(buffer);
		else
			attachment.insert(attachment.begin() + insertPos, buffer);
	}
}

void Framebuffer::addColorTextureCubemapAttachment(int width, int height, int insertPos)
{
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

	struct Attachment buffer;
	buffer.type = ATTACHMENT_TYPE::TEXTURE_CUBE_MAP;
	buffer.target = ATTACHMENT_TARGET::COLOR;
	GLint internalFormat = (HDR) ? GL_RGBA16F : GL_RGBA;
	GLenum type = (HDR) ? GL_FLOAT : GL_UNSIGNED_BYTE;

	glGenTextures(1, &buffer.id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, buffer.id);
	for(int i{0}; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, GL_RGBA, type, nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, buffer.id, 0);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: framebuffer is not complete !" << std::endl;
	else
	{
		if(insertPos == -1)
			attachment.push_back(buffer);
		else
			attachment.insert(attachment.begin() + insertPos, buffer);
	}
}

void Framebuffer::addDepthTextureCubemapAttachment(int width, int height, int insertPos)
{
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

	struct Attachment buffer;
	buffer.type = ATTACHMENT_TYPE::TEXTURE_CUBE_MAP;
	buffer.target = ATTACHMENT_TARGET::DEPTH;

	glGenTextures(1, &buffer.id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, buffer.id);
	for(int i{0}; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, buffer.id, 0);
	if(!renderColor)
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: framebuffer is not complete !" << std::endl;
	else
	{
		if(insertPos == -1)
			attachment.push_back(buffer);
		else
			attachment.insert(attachment.begin() + insertPos, buffer);
	}
}

void Framebuffer::addColorRenderbufferAttachment(int width, int height, int insertPos)
{
	struct Attachment buffer;
	buffer.type = ATTACHMENT_TYPE::RENDER_BUFFER;
	buffer.target = ATTACHMENT_TARGET::COLOR;
	GLint internalFormat = (HDR) ? GL_RGBA16F : GL_RGBA8;
	
	glGenRenderbuffers(1, &buffer.id);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer.id);
	if(multiSample)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, internalFormat, width, height);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, buffer.id);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: framebuffer is not complete !" << std::endl;
	else
	{
		if(insertPos == -1)
			attachment.push_back(buffer);
		else
			attachment.insert(attachment.begin() + insertPos, buffer);
	}
}

void Framebuffer::addDepthRenderbufferAttachment(int width, int height, int insertPos)
{
	struct Attachment buffer;
	buffer.type = ATTACHMENT_TYPE::RENDER_BUFFER;
	buffer.target = ATTACHMENT_TARGET::DEPTH;

	glGenRenderbuffers(1, &buffer.id);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer.id);
	if(multiSample)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, width, height);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer.id);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: framebuffer is not complete !" << std::endl;
	else
	{
		if(insertPos == -1)
			attachment.push_back(buffer);
		else
			attachment.insert(attachment.begin() + insertPos, buffer);
	}
}

void Framebuffer::addDepthStencilRenderbufferAttachment(int width, int height, int insertPos)
{
	struct Attachment buffer;
	buffer.type = ATTACHMENT_TYPE::RENDER_BUFFER;
	buffer.target = ATTACHMENT_TARGET::DEPTH_STENCIL;

	glGenRenderbuffers(1, &buffer.id);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer.id);
	if(multiSample)
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
	else
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer.id);
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: framebuffer is not complete !" << std::endl;
	else
	{
		if(insertPos == -1)
			attachment.push_back(buffer);
		else
			attachment.insert(attachment.begin() + insertPos, buffer);
	}
}

void Framebuffer::updateColorTextureAttachment(int index, int width, int height, int insertPos)
{
	if(multiSample)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D_MULTISAMPLE, 0, 0);
	else
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, 0, 0);
	glDeleteTextures(1, &attachment.at(insertPos).id);

	attachment.erase(attachment.begin() + insertPos);

	if(insertPos == attachment.size())
		addColorTextureAttachment(width, height, -1);
	else
		addColorTextureAttachment(width, height, insertPos);
}

void Framebuffer::updateDepthTextureAttachment(int width, int height, int insertPos)
{
	glBindTexture(GL_TEXTURE_2D, attachment[insertPos].id);
	if(multiSample)
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH_COMPONENT32F, width, height, GL_TRUE);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
}

void Framebuffer::updateColorTextureCubemapAttachment(int width, int height, int insertPos)
{
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
	glDeleteTextures(1, &attachment[insertPos].id);

	attachment.erase(attachment.begin() + insertPos);

	if(insertPos == attachment.size())
		addColorTextureCubemapAttachment(width, height, -1);
	else
		addColorTextureCubemapAttachment(width, height, insertPos);
}

void Framebuffer::updateDepthTextureCubemapAttachment(int width, int height, int insertPos)
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, attachment[insertPos].id);
	for (int i{ 0 }; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
}

void Framebuffer::updateColorRenderbufferAttachment(int width, int height, int insertPos)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &attachment.at(insertPos).id);

	attachment.erase(attachment.begin() + insertPos);

	if(insertPos == attachment.size())
		addColorRenderbufferAttachment(width, height, -1);
	else
		addColorRenderbufferAttachment(width, height, insertPos);
}

void Framebuffer::updateDepthRenderbufferAttachment(int width, int height, int insertPos)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &attachment[insertPos].id);

	attachment.erase(attachment.begin() + insertPos);

	if(insertPos == attachment.size())
		addDepthRenderbufferAttachment(width, height, -1);
	else
		addDepthRenderbufferAttachment(width, height, insertPos);
}

void Framebuffer::updateDepthStencilRenderbufferAttachment(int width, int height, int insertPos)
{
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &attachment[insertPos].id);

	attachment.erase(attachment.begin() + insertPos);

	if(insertPos == attachment.size())
		addDepthStencilRenderbufferAttachment(width, height, -1);
	else
		addDepthStencilRenderbufferAttachment(width, height, insertPos);
}

void Framebuffer::updateAttachment(ATTACHMENT_TYPE type, ATTACHMENT_TARGET target, int width, int height)
{
	int index{0};
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	for(int i{0}; i < attachment.size(); ++i)
	{
		if(attachment[i].type == type && attachment[i].target == target)
		{
			if(type == ATTACHMENT_TYPE::TEXTURE)
			{
				switch(target)
				{
					case ATTACHMENT_TARGET::COLOR:
						updateColorTextureAttachment(index, width, height, i);
						index++;
						break;
					case ATTACHMENT_TARGET::DEPTH:
						updateDepthTextureAttachment(width, height, i);
						break;
					default:
						break;
				}
			}
			else if(type == ATTACHMENT_TYPE::TEXTURE_CUBE_MAP)
			{
				switch(target)
				{
					case ATTACHMENT_TARGET::COLOR:
						updateColorTextureCubemapAttachment(width, height, i);
						break;
					case ATTACHMENT_TARGET::DEPTH:
						updateDepthTextureCubemapAttachment(width, height, i);
						break;
					default:
						break;
				}
			}
			else if(type == ATTACHMENT_TYPE::RENDER_BUFFER)
			{
				switch(target)
				{
					case ATTACHMENT_TARGET::COLOR:
						updateColorRenderbufferAttachment(width, height, i);
						break;
					case ATTACHMENT_TARGET::DEPTH:
						updateDepthRenderbufferAttachment(width, height, i);
						break;
					case ATTACHMENT_TARGET::DEPTH_STENCIL:
						updateDepthStencilRenderbufferAttachment(width, height, i);
						break;
					default:
						break;
				}
			}
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

std::vector<Attachment> & Framebuffer::getAttachments()
{
	return attachment;
}

void Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::blitFramebuffer(Framebuffer & writeFBO, int width, int height)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, writeFBO.getId());
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void Framebuffer::blitFramebuffer(std::unique_ptr<Framebuffer> & writeFBO, int width, int height)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, writeFBO->getId());
	glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

GLuint Framebuffer::getId()
{
	return fbo;
}

int Framebuffer::getColorAttachmentCount()
{
	int count{0};
	for(int i{0}; i < attachment.size(); ++i)
	{
		if(attachment.at(i).target == ATTACHMENT_TARGET::COLOR)
			count++;
	}
	return count;
}

void Framebuffer::addSingleColorTextureAttachment(GLint format, GLenum minMagFilter, int width, int height)
{
	float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};

	struct Attachment buffer;
	buffer.type = ATTACHMENT_TYPE::TEXTURE;
	buffer.target = ATTACHMENT_TARGET::COLOR;
	GLenum type = (HDR) ? GL_FLOAT : GL_UNSIGNED_BYTE;
	int count{getColorAttachmentCount()};

	if(!multiSample)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glGenTextures(1, &buffer.id);
		glBindTexture(GL_TEXTURE_2D, buffer.id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFilter);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + count, GL_TEXTURE_2D, buffer.id, 0);
	}
	else
	{
		std::cerr << "Error : cannot add single color texture attachment ! This function holds for non MSAA FBOs." << std::endl;
	}

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "Error: framebuffer is not complete !" << std::endl;
	else
	{
		attachment.push_back(buffer);
	}

	int index{0};
	std::vector<GLenum> colorAttachments;
	for(int i{0}; i < attachment.size(); ++i)
	{
		if(attachment.at(i).target == ATTACHMENT_TARGET::COLOR)
		{
			colorAttachments.push_back(GL_COLOR_ATTACHMENT0 + index);
			index++;
		}
	}
	glDrawBuffers(colorAttachments.size(), colorAttachments.data());
}
