#include "skybox.hpp"
#include "stb_image.h"

Skybox::Skybox(std::vector<std::string> & textures, bool flip) :
	shader("shaders/skybox/vertex.glsl", "shaders/skybox/fragment.glsl")
{
	// create cube map
	glGenTextures(1, &cubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	GLenum srcFormat;
	GLenum destFormat = GL_RGB;
	int width;
	int height;
	int channels;

	stbi_set_flip_vertically_on_load(flip);

	// order: right, left, top, bottom, back, front
	for(int i{0}; i <  textures.size(); ++i)
	{
		unsigned char* data = stbi_load(textures[i].c_str(), &width, &height, &channels, 0);
		if(data)
		{
			if(channels == 3)
			{
				srcFormat = GL_SRGB;
				destFormat = GL_RGB;
			}
			else if(channels == 4)
			{
				srcFormat = GL_SRGB_ALPHA;
				destFormat = GL_RGBA;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, srcFormat, width, height, 0, destFormat, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cerr << "Error while trying to load texture : " << textures.at(i) << " !\n";
			stbi_image_free(data);
			std::exit(-1);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// create the actual cube shape
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

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

Skybox::~Skybox()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
	glDeleteTextures(1, &cubeMap);
}

void Skybox::draw(glm::mat4 aView, glm::mat4 aProj)
{
	// set shader
	shader.use();
	shader.setMatrix("view", glm::mat4(glm::mat3(aView)));
	shader.setMatrix("proj", aProj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);
	shader.setInt("skybox", 0);

	// draw skybox
	glBindVertexArray(vao);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthFunc(GL_LESS);

	// unbind vao
	glBindVertexArray(0);
}
