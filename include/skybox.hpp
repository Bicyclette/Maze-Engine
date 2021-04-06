#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <utility>
#include "shader_light.hpp"

class Skybox
{
	public:

		Skybox(std::vector<std::string> & textures, bool flip);
		~Skybox();
		void draw(glm::mat4 aView, glm::mat4 aProj);

	private:

		GLuint vao;
		GLuint vbo;
		GLuint cubeMap;
		Shader shader;
};

#endif
