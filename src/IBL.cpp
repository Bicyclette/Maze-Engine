#include "IBL.hpp"
#include "stb_image.h"

IBL::IBL(std::string env_map, bool flip, int clientWidth, int clientHeight) :
	equirectangular_to_cubemap_shader("shaders/HDRI/equirec_to_cubemap/vertex.glsl", "shaders/HDRI/equirec_to_cubemap/fragment.glsl"),
	irradiance_shader("shaders/HDRI/diffuse_irradiance/vertex.glsl", "shaders/HDRI/diffuse_irradiance/fragment.glsl"),
	prefilter_shader("shaders/HDRI/prefilter/vertex.glsl", "shaders/HDRI/prefilter/fragment.glsl"),
	brdf_shader("shaders/HDRI/brdf/vertex.glsl", "shaders/HDRI/brdf/fragment.glsl"),
	skybox_shader("shaders/skybox/vertex.glsl", "shaders/skybox/fragment.glsl")
{
	create_geometry();
	create_cubemaps();

	// #################### create look at matrices
	// ############################################

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	std::array<glm::mat4, 6> captureViews =
	{
		glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
	};

	// #################### create capture framebuffers
	// ################################################

	GLuint captureFBO[2];
	GLuint captureRBO[2];
	glGenFramebuffers(2, captureFBO);
	glGenRenderbuffers(2, captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO[0]);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureFBO[0]);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO[1]);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO[1]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureFBO[1]);

	// #################### BRDF Look Up Texture
	// #########################################

	glGenTextures(1, &brdfLUT);

	glBindTexture(GL_TEXTURE_2D, brdfLUT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, 512, 512, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// #################### create env cubemap
	// #######################################

	init_env_cubemap(env_map, captureFBO[0], captureProjection, captureViews, flip);

	// #################### env cubemap to diffuse irradiance cubemap
	// ##############################################################

	irradiance_shader.use();
	irradiance_shader.setInt("env_map", 0);
	irradiance_shader.setMatrix("proj", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubeMap);
	
	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO[1]);
	for(int i{0}; i < 6; ++i)
	{
		irradiance_shader.setMatrix("view", captureViews[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradiance_cubeMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// #################### pre-filter env map with multiple roughness levels over
	// #################### multiple mipmaps levels
	// ###########################################################################

	prefilter_shader.use();
	prefilter_shader.setInt("env_map", 0);
	prefilter_shader.setMatrix("proj", captureProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubeMap);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO[0]);
	int maxMipLevels{5};
	for(int mip{0}; mip < maxMipLevels; ++mip)
	{
		int mipWidth = 128 * std::pow(0.5, mip);
		int mipHeight = mipWidth;
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO[0]);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = static_cast<float>(mip) / static_cast<float>(maxMipLevels - 1);
		prefilter_shader.setFloat("roughness", roughness);

		for(int i{0}; i < 6; ++i)
		{
			prefilter_shader.setMatrix("view", captureViews[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilter_cubeMap, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
			glBindVertexArray(cube_vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	

	// #################### Fill BRDF LUT
	// ##################################

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO[0]);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO[0]);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT, 0);

	glViewport(0, 0, 512, 512);
	brdf_shader.use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(quad_vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);
	
	// #################### BIND TO DEFAULT FRAMEBUFFER
	// ################################################
	glViewport(0, 0, clientWidth, clientHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);	
}

void IBL::create_geometry()
{
	// #################### create quad
	// ################################

	float quad[] =
	{
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 
		-1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &quad_vao);
	glBindVertexArray(quad_vao);

	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// #################### create cube
	// ################################

	float cube[] =
	{
		0.5f, 0.5f, 0.5f, 
		-0.5f, -0.5f, 0.5f, 
		-0.5f, 0.5f, 0.5f, 
		0.5f, 0.5f, 0.5f, 
		0.5f, -0.5f, 0.5f, 
		-0.5f, -0.5f, 0.5f, // back
		
		-0.5f, -0.5f, -0.5f, 
		0.5f, 0.5f, -0.5f, 
		-0.5f, 0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f, // front

		-0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f, // left
		
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f, // right

		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, // bottom
		
		-0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f // top
	};

	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);

	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
}

void IBL::create_cubemaps()
{
	glGenTextures(1, &env_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubeMap);
	for(int i{0}; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glGenTextures(1, &irradiance_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, irradiance_cubeMap);
	for(int i{0}; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenTextures(1, &prefilter_cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, prefilter_cubeMap);

	for(int i{0}; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void IBL::init_env_cubemap(std::string env_map, GLuint fbo, glm::mat4 proj, std::array<glm::mat4, 6> & views, bool flip)
{
	// #################### load HDR image
	// ###################################

	stbi_set_flip_vertically_on_load(flip);
	int width;
	int height;
	int channels;
	GLuint hdrTexture;

	float * data = stbi_loadf(env_map.c_str(), &width, &height, &channels, 0);

	if(data)
	{
		glGenTextures(1, &hdrTexture);
		glBindTexture(GL_TEXTURE_2D, hdrTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load HDR image." << std::endl;
	}

	// #################### hdr texture to env cubemap
	// ###############################################

	equirectangular_to_cubemap_shader.use();
	equirectangular_to_cubemap_shader.setInt("equirectangular_to_cubemap_shader", 0);
	equirectangular_to_cubemap_shader.setMatrix("proj", proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrTexture);
	equirectangular_to_cubemap_shader.setInt("env_map", 0);

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	for(int i{0}; i < 6; ++i)
	{
		equirectangular_to_cubemap_shader.setMatrix("view", views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, env_cubeMap, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(cube_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubeMap);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

IBL::~IBL()
{
	glBindVertexArray(cube_vao);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &cube_vbo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &cube_vao);
	
	glBindVertexArray(quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &quad_vbo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &quad_vao);
	
	glDeleteTextures(1, &brdfLUT);
	glDeleteTextures(1, &env_cubeMap);
	glDeleteTextures(1, &irradiance_cubeMap);
	glDeleteTextures(1, &prefilter_cubeMap);
}

struct IBL_DATA IBL::get_IBL_data()
{
	struct IBL_DATA data{irradiance_cubeMap, prefilter_cubeMap, brdfLUT};
	return data;
}

void IBL::draw(glm::mat4 aView, glm::mat4 aProj)
{
	// set shader
	skybox_shader.use();
	skybox_shader.setMatrix("view", glm::mat4(glm::mat3(aView)));
	skybox_shader.setMatrix("proj", aProj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, env_cubeMap);
	skybox_shader.setInt("skybox", 0);

	// draw env cubemap
	glBindVertexArray(cube_vao);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);

	// unbind vao
	glBindVertexArray(0);
}
