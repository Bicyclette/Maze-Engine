#ifndef MOUSE_HPP
#define MOUSE_HPP

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "shader_light.hpp"

class Mouse
{
public:
	Mouse(int pos[2], int size[2], std::string img_normal, std::string img_hover, int screenW, int screenH);
	~Mouse();
	void update_position();
	void set_bloom_strength(float strength);
	void use_normal();
	void use_hover();
	void draw();
	int* get_position(); // bottom left corner
	int* get_size();
	bool is_active() { return m_active; }
	void activate() { m_active = true; SDL_ShowCursor(false); }
	void deactivate() { m_active = false; SDL_ShowCursor(true); }

private:

	GLuint m_vao;
	GLuint m_vbo;
	int m_screen[2];
	int m_pos[2]; // [x,y] top left corner
	int m_size[2]; // [x,y] size
	std::vector<struct Texture> m_img; // [0] = normal, [1] = hover
	int m_img_index;
	float m_bloom_strength;
	Shader m_shader;
	glm::mat4 m_projection;
	bool m_active;
};

#endif