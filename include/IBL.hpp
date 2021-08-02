#ifndef IBL_HPP
#define IBL_HPP

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <utility>
#include <array>
#include "shader_light.hpp"

struct IBL_DATA
{
	GLuint irradiance;
	GLuint prefilter;
	GLuint brdf;
};

class IBL
{
	public:
		IBL(std::string env_map, bool flip, int clientWidth, int clientHeight);
		~IBL();
		struct IBL_DATA get_IBL_data();
		void draw(glm::mat4 aView, glm::mat4 aProj);

	private:
		void create_geometry();
		void create_cubemaps();
		void init_env_cubemap(std::string env_map, GLuint fbo, glm::mat4 proj, std::array<glm::mat4, 6> & views, bool flip);
	
		// geometry ##########
		// ###################
		GLuint cube_vao;
		GLuint cube_vbo;
		GLuint quad_vao;
		GLuint quad_vbo;

		// 2D texture ##########
		// ######################
		GLuint brdfLUT;

		// cubemaps ##########
		// ###################
		GLuint env_cubeMap;
		GLuint irradiance_cubeMap;
		GLuint prefilter_cubeMap;

		// shaders ##########
		// ##################
		Shader equirectangular_to_cubemap_shader;
		Shader irradiance_shader;
		Shader prefilter_shader;
		Shader brdf_shader;
		Shader skybox_shader;
};

#endif
