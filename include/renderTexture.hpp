#ifndef RENDER_TEXTURE_HPP
#define RENDER_TEXTURE_HPP

#include <GL/glew.h>
#include <iostream>
#include <memory>
#include <utility>

class RenderTexture
{
	public:

		RenderTexture(int width, int height, int depth, GLint storedFormat, GLint passedFormat, GLenum type);
		~RenderTexture();
        void set_min_mag_filter(GLint minifying, GLint magnification);
        void set_wrap_method(GLint wrap);
        void set_border_color(const GLfloat * border);
		GLuint getId();

	private:

		GLuint m_id;
        int m_width;
        int m_height;
        int m_depth;
        GLint m_format;
        GLenum m_target;
};

#endif
