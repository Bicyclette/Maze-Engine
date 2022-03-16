#include "renderTexture.hpp"

RenderTexture::RenderTexture(int width, int height, int depth, GLint storedFormat, GLint passedFormat, GLenum type) :
    m_width(width),
    m_height(height),
    m_depth(depth),
    m_format(storedFormat)
{
    glGenTextures(1, &m_id);
    if(width > 1 && height > 1 && depth > 1)
    {
        m_target = GL_TEXTURE_3D;
        glBindTexture(GL_TEXTURE_3D, m_id);
        glTexImage3D(GL_TEXTURE_3D, 0, storedFormat, m_width, m_height, m_depth, 0, passedFormat, type, nullptr);
    }
    else if(width > 1 && height > 1 && depth == 1)
    {
        m_target = GL_TEXTURE_2D;
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, storedFormat, m_width, m_height, 0, passedFormat, type, nullptr);
    }
    else if(width > 1 && height == 1 && depth == 1)
    {
        m_target = GL_TEXTURE_1D;
        glBindTexture(GL_TEXTURE_1D, m_id);
        glTexImage1D(GL_TEXTURE_1D, 0, storedFormat, m_width, 0, passedFormat, type, nullptr);
    }
    glBindTexture(m_target, 0);
}

RenderTexture::~RenderTexture()
{
    glDeleteTextures(1, &m_id);
}

void RenderTexture::set_min_mag_filter(GLint minifying, GLint magnification)
{
    glBindTexture(m_target, m_id);
    glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, minifying);
    glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, magnification);
    glBindTexture(m_target, 0);
}

void RenderTexture::set_wrap_method(GLint wrap)
{
    glBindTexture(m_target, m_id);
    if(m_target == GL_TEXTURE_3D)
    {
        glTexParameteri(m_target, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(m_target, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(m_target, GL_TEXTURE_WRAP_R, wrap);
    }
    else if(m_target == GL_TEXTURE_2D)
    {
        glTexParameteri(m_target, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(m_target, GL_TEXTURE_WRAP_T, wrap);
    }
    else
    {
        glTexParameteri(m_target, GL_TEXTURE_WRAP_S, wrap);
    }
    glBindTexture(m_target, 0);
}

void RenderTexture::set_border_color(const GLfloat * border)
{
    glBindTexture(m_target, m_id);
    glTexParameterfv(m_target, GL_TEXTURE_BORDER_COLOR, border);
    glBindTexture(m_target, 0);
}

GLuint RenderTexture::getId()
{
    return m_id;
}
