#include "shader_light.hpp"
#include "stb_image.h"

Shader::Shader(const std::string & vertex_shader_file, const std::string & fragment_shader_file, SHADER_TYPE t) :
	type(t)
{
	int vShader_codeLength;
	int fShader_codeLength;

	char* vShaderCode;
	char* fShaderCode;

	std::ifstream v_shader_stream, f_shader_stream;
	v_shader_stream.open(vertex_shader_file, std::ifstream::binary);
	f_shader_stream.open(fragment_shader_file, std::ifstream::binary);

	// compute code lengths
	v_shader_stream.seekg(0, v_shader_stream.end);
	vShader_codeLength = v_shader_stream.tellg();
	v_shader_stream.seekg(0, v_shader_stream.beg);
	
	f_shader_stream.seekg(0, f_shader_stream.end);
	fShader_codeLength = f_shader_stream.tellg();
	f_shader_stream.seekg(0, f_shader_stream.beg);

	// create arrays
	vShaderCode = new char[vShader_codeLength + 1];
	fShaderCode = new char[fShader_codeLength + 1];

	vShaderCode[vShader_codeLength] = '\0';
	fShaderCode[fShader_codeLength] = '\0';

	v_shader_stream.read(vShaderCode, vShader_codeLength);
	f_shader_stream.read(fShaderCode, fShader_codeLength);

	if(!v_shader_stream)
		std::cerr << "Error while trying to read the vertex shader file !" << std::endl;
	if(!f_shader_stream)
		std::cerr << "Error while trying to read the fragment shader file !" << std::endl;

	// Now compile the shaders, create the shader program and link
	compile(vShaderCode, fShaderCode);

	delete[](vShaderCode);
	delete[](fShaderCode);

	v_shader_stream.close();
	f_shader_stream.close();
}

Shader::Shader(const std::string & vertex_shader_file, const std::string & geometry_shader_file, const std::string & fragment_shader_file, SHADER_TYPE t) :
	type(t)
{
	int vShader_codeLength;
	int gShader_codeLength;
	int fShader_codeLength;

	char* vShaderCode;
	char* gShaderCode;
	char* fShaderCode;

	std::ifstream v_shader_stream, g_shader_stream, f_shader_stream;
	v_shader_stream.open(vertex_shader_file, std::ifstream::binary);
	g_shader_stream.open(geometry_shader_file, std::ifstream::binary);
	f_shader_stream.open(fragment_shader_file, std::ifstream::binary);

	// compute code lengths
	v_shader_stream.seekg(0, v_shader_stream.end);
	vShader_codeLength = v_shader_stream.tellg();
	v_shader_stream.seekg(0, v_shader_stream.beg);
	
	g_shader_stream.seekg(0, g_shader_stream.end);
	gShader_codeLength = g_shader_stream.tellg();
	g_shader_stream.seekg(0, g_shader_stream.beg);
	
	f_shader_stream.seekg(0, f_shader_stream.end);
	fShader_codeLength = f_shader_stream.tellg();
	f_shader_stream.seekg(0, f_shader_stream.beg);

	// create arrays
	vShaderCode = new char[vShader_codeLength + 1];
	gShaderCode = new char[gShader_codeLength + 1];
	fShaderCode = new char[fShader_codeLength + 1];

	vShaderCode[vShader_codeLength] = '\0';
	gShaderCode[gShader_codeLength] = '\0';
	fShaderCode[fShader_codeLength] = '\0';

	v_shader_stream.read(vShaderCode, vShader_codeLength);
	g_shader_stream.read(gShaderCode, gShader_codeLength);
	f_shader_stream.read(fShaderCode, fShader_codeLength);

	if(!v_shader_stream)
		std::cerr << "Error while trying to read the vertex shader file !" << std::endl;
	if(!g_shader_stream)
		std::cerr << "Error while trying to read the geometry shader file !" << std::endl;
	if(!f_shader_stream)
		std::cerr << "Error while trying to read the fragment shader file !" << std::endl;

	// Now compile the shaders, create the shader program and link
	compile(vShaderCode, gShaderCode, fShaderCode);

	delete[](vShaderCode);
	delete[](gShaderCode);
	delete[](fShaderCode);

	v_shader_stream.close();
	g_shader_stream.close();
	f_shader_stream.close();
}

Shader::~Shader()
{
	glDeleteShader(id);
}

void Shader::compile(const char * vertex_shader_code, const char * fragment_shader_code)
{
	GLuint vertex_shader, fragment_shader, shader_program;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	shader_program = glCreateProgram();

	glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);	
	glCompileShader(vertex_shader);
	
	glShaderSource(fragment_shader, 1, &fragment_shader_code, nullptr);	
	glCompileShader(fragment_shader);

	// Check for errors
	int success;
	int logLength;
	char* log;

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE)
	{
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &logLength);
		log = new char[logLength];
		glGetShaderInfoLog(vertex_shader, logLength, nullptr, log);
		std::cerr << "Error while compiling the vertex shader : " << log << std::endl;
		delete[](log);
		glDeleteShader(vertex_shader);
	}
	
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE)
	{
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &logLength);
		log = new char[logLength];
		glGetShaderInfoLog(fragment_shader, logLength, nullptr, log);
		std::cerr << "Error while compiling the fragment shader : " << log << std::endl;
		delete[](log);
		glDeleteShader(fragment_shader);
	}

	// Final step
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if(success == GL_FALSE)
	{
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &logLength);
		log = new char[logLength];
		glGetProgramInfoLog(shader_program, logLength, nullptr, log);
		std::cerr << "Error while linking shaders into a program : " << log << std::endl;
		delete[](log);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	glDetachShader(shader_program, vertex_shader);
	glDetachShader(shader_program, fragment_shader);

	id = shader_program;
}

void Shader::compile(const char * vertex_shader_code, const char * geometry_shader_code, const char * fragment_shader_code)
{
	GLuint vertex_shader, geometry_shader, fragment_shader, shader_program;
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	shader_program = glCreateProgram();

	glShaderSource(vertex_shader, 1, &vertex_shader_code, nullptr);	
	glCompileShader(vertex_shader);
	
	glShaderSource(geometry_shader, 1, &geometry_shader_code, nullptr);	
	glCompileShader(geometry_shader);
	
	glShaderSource(fragment_shader, 1, &fragment_shader_code, nullptr);	
	glCompileShader(fragment_shader);

	// Check for errors
	int success;
	int logLength;
	char* log;

	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE)
	{
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &logLength);
		log = new char[logLength];
		glGetShaderInfoLog(vertex_shader, logLength, nullptr, log);
		std::cerr << "Error while compiling the vertex shader : " << log << std::endl;
		delete[](log);
		glDeleteShader(vertex_shader);
	}
	
	glGetShaderiv(geometry_shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE)
	{
		glGetShaderiv(geometry_shader, GL_INFO_LOG_LENGTH, &logLength);
		log = new char[logLength];
		glGetShaderInfoLog(geometry_shader, logLength, nullptr, log);
		std::cerr << "Error while compiling the geometry shader : " << log << std::endl;
		delete[](log);
		glDeleteShader(geometry_shader);
	}
	
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if(success == GL_FALSE)
	{
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &logLength);
		log = new char[logLength];
		glGetShaderInfoLog(fragment_shader, logLength, nullptr, log);
		std::cerr << "Error while compiling the fragment shader : " << log << std::endl;
		delete[](log);
		glDeleteShader(fragment_shader);
	}

	// Final step
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, geometry_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if(success == GL_FALSE)
	{
		glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &logLength);
		log = new char[logLength];
		glGetProgramInfoLog(shader_program, logLength, nullptr, log);
		std::cerr << "Error while linking shaders into a program : " << log << std::endl;
		delete[](log);
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);
	}

	glDetachShader(shader_program, vertex_shader);
	glDetachShader(shader_program, geometry_shader);
	glDetachShader(shader_program, fragment_shader);

	id = shader_program;
}

GLuint Shader::getId() const { return id; }

SHADER_TYPE Shader::getType() { return type; }

void Shader::setInt(const std::string & name, int v) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), v);
}

void Shader::setFloat(const std::string & name, float v) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), v);
}

void Shader::setVec2f(const std::string & name, glm::vec2 v) const
{
	glUniform2f(glGetUniformLocation(id, name.c_str()), v.x, v.y);
}

void Shader::setVec3f(const std::string & name, glm::vec3 v) const
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z);
}

void Shader::setVec4f(const std::string & name, glm::vec4 v) const
{
	glUniform4f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z, v.w);
}

void Shader::setMatrix(const std::string & name, glm::mat4 m) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::setLighting(std::vector<std::shared_ptr<PointLight>> & pLights, std::vector<std::shared_ptr<DirectionalLight>> & dLights, std::vector<std::shared_ptr<SpotLight>> & sLights)
{
	int pCount = pLights.size();
	int dCount = dLights.size();
	int sCount = sLights.size();
	int lightCount = pCount + dCount + sCount;
	int dOffset = pCount;
	int sOffset = pCount + dCount;
	setInt("lightCount", lightCount);

	for(int i{0}; i < pCount; ++i)
	{
		setInt(std::string("light[" + std::to_string(i) + "].type"), static_cast<int>(pLights.at(i)->getType()));
		setFloat(std::string("light[" + std::to_string(i) + "].kc"), pLights.at(i)->getKc());
		setFloat(std::string("light[" + std::to_string(i) + "].kl"), pLights.at(i)->getKl());
		setFloat(std::string("light[" + std::to_string(i) + "].kq"), pLights.at(i)->getKq());
		setVec3f(std::string("light[" + std::to_string(i) + "].position"), pLights.at(i)->getPosition());
		if(type == SHADER_TYPE::BLINN_PHONG)
		{
			setVec3f(std::string("light[" + std::to_string(i) + "].ambientStrength"), pLights.at(i)->getAmbientStrength());
			setVec3f(std::string("light[" + std::to_string(i) + "].diffuseStrength"), pLights.at(i)->getDiffuseStrength());
			setVec3f(std::string("light[" + std::to_string(i) + "].specularStrength"), pLights.at(i)->getSpecularStrength());
		}
		else if(type == SHADER_TYPE::PBR)
			setVec3f(std::string("light[" + std::to_string(i) + "].color"), pLights.at(i)->getDiffuseStrength());
	}

	for(int i{0}; i < dLights.size(); ++i)
	{
		setInt(std::string("light[" + std::to_string(i + dOffset) + "].type"), static_cast<int>(dLights.at(i)->getType()));
		setVec3f(std::string("light[" + std::to_string(i + dOffset) + "].position"), dLights.at(i)->getPosition());
		setVec3f(std::string("light[" + std::to_string(i + dOffset) + "].direction"), dLights.at(i)->getDirection());
		if(type == SHADER_TYPE::BLINN_PHONG)
		{
			setVec3f(std::string("light[" + std::to_string(i + dOffset) + "].ambientStrength"), dLights.at(i)->getAmbientStrength());
			setVec3f(std::string("light[" + std::to_string(i + dOffset) + "].diffuseStrength"), dLights.at(i)->getDiffuseStrength());
			setVec3f(std::string("light[" + std::to_string(i + dOffset) + "].specularStrength"), dLights.at(i)->getSpecularStrength());
		}
		else if(type == SHADER_TYPE::PBR)
			setVec3f(std::string("light[" + std::to_string(i + dOffset) + "].color"), dLights.at(i)->getDiffuseStrength());
	}

	for(int i{0}; i < sLights.size(); ++i)
	{
		setInt(std::string("light[" + std::to_string(i + sOffset) + "].type"), static_cast<int>(sLights.at(i)->getType()));
		setFloat(std::string("light[" + std::to_string(i + sOffset) + "].cutOff"), cos(sLights.at(i)->getCutOff()));
		setFloat(std::string("light[" + std::to_string(i + sOffset) + "].outerCutOff"), cos(sLights.at(i)->getOuterCutOff()));
		setVec3f(std::string("light[" + std::to_string(i + sOffset) + "].position"), sLights.at(i)->getPosition());
		setVec3f(std::string("light[" + std::to_string(i + sOffset) + "].direction"), sLights.at(i)->getDirection());
		if(type == SHADER_TYPE::BLINN_PHONG)
		{
			setVec3f(std::string("light[" + std::to_string(i + sOffset) + "].ambientStrength"), sLights.at(i)->getAmbientStrength());
			setVec3f(std::string("light[" + std::to_string(i + sOffset) + "].diffuseStrength"), sLights.at(i)->getDiffuseStrength());
			setVec3f(std::string("light[" + std::to_string(i + sOffset) + "].specularStrength"), sLights.at(i)->getSpecularStrength());
		}
		else if(type == SHADER_TYPE::PBR)
			setVec3f(std::string("light[" + std::to_string(i + sOffset) + "].color"), sLights.at(i)->getDiffuseStrength());
	}
}

void Shader::use() const
{
	glUseProgram(id);
}

struct Texture createTexture(const std::string & texPath, TEXTURE_TYPE t, bool flip)
{
	GLuint texId;
	GLenum srcFormat;
	GLenum destFormat;
	int width;
	int height;
	int channels;
	stbi_set_flip_vertically_on_load(flip);
	unsigned char* data = stbi_load(texPath.c_str(), &width, &height, &channels, 0);

	if(data)
	{
		if(channels == 3)
		{
			if(t == TEXTURE_TYPE::DIFFUSE)
				srcFormat = GL_SRGB;
			else
				srcFormat = GL_RGB;
			destFormat = GL_RGB;
		}
		else if(channels == 4)
		{
			if(t == TEXTURE_TYPE::DIFFUSE)
				srcFormat = GL_SRGB_ALPHA;
			else
				srcFormat = GL_RGBA;
			destFormat = GL_RGBA;
		}

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, srcFormat, width, height, 0, destFormat, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cerr << "Error while trying to load texture : " << texPath << " !\n";
	}

	stbi_image_free(data);

	struct Texture tex(texId, t, texPath);

	return tex;
}

struct Texture createTextureFromData(aiTexture* embTex, TEXTURE_TYPE t, bool flip)
{
	GLuint texId;
	GLenum srcFormat;
	GLenum destFormat;
	int width;
	int height;
	int channels;
	unsigned char* data;
	stbi_set_flip_vertically_on_load(flip);

	if(embTex->mHeight == 0)
		data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(embTex->pcData), embTex->mWidth, &width, &height, &channels, 0);
	else
		data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(embTex->pcData), embTex->mWidth * embTex->mHeight, &width, &height, &channels, 0);

	if(data)
	{
		if(channels == 3)
		{
			if(t == TEXTURE_TYPE::DIFFUSE)
				srcFormat = GL_SRGB;
			else
				srcFormat = GL_RGB;
			destFormat = GL_RGB;
		}
		else if(channels == 4)
		{
			if(t == TEXTURE_TYPE::DIFFUSE)
				srcFormat = GL_SRGB_ALPHA;
			else
				srcFormat = GL_RGBA;
			destFormat = GL_RGBA;
		}

		glGenTextures(1, &texId);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, srcFormat, width, height, 0, destFormat, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cerr << "Error while trying to load embedded texture !\n";
	}

	stbi_image_free(data);

	struct Texture tex(texId, t, "embedded");

	return tex;
}

Light::Light(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) :
	position(pos),
	ambientStrength(amb),
	diffuseStrength(diff),
	specularStrength(spec)
{
	model = glm::mat4(1.0f);
}

Light::~Light()
{
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vao);
	glDeleteTextures(1, &icon.id);
}

PointLight::PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float aKc, float aKl, float aKq) :
	Light(pos, amb, diff, spec),
	kc(aKc),
	kl(aKl),
	kq(aKq),
	shader("shaders/light/point/vertex.glsl", "shaders/light/point/geometry.glsl", "shaders/light/point/fragment.glsl")
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float data[3] = {position.x, position.y, position.z};
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);

	shader.use();
	shader.setInt("icon", 0);
	
	icon = createTexture("assets/light_icons/point.png", TEXTURE_TYPE::DIFFUSE, true);
}

void PointLight::draw()
{
	glBindVertexArray(vao);
	shader.use();
	shader.setMatrix("model", model);
	shader.setMatrix("view", view);
	shader.setMatrix("proj", proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, icon.id);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

LIGHT_TYPE PointLight::getType()
{
	return LIGHT_TYPE::POINT;
}

float PointLight::getKc()
{
	return kc;
}

float PointLight::getKl()
{
	return kl;
}

float PointLight::getKq()
{
	return kq;
}

DirectionalLight::DirectionalLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir) :
	Light(pos, amb, diff, spec),
	direction(glm::normalize(dir)),
	shaderIcon("shaders/light/directional/vertex.glsl", "shaders/light/directional/geometry.glsl", "shaders/light/directional/fragment.glsl"),
	shaderDirection("shaders/light/directional/vertex_direction.glsl", "shaders/light/directional/geometry_direction.glsl", "shaders/light/directional/fragment_direction.glsl")
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float data[3] = {position.x, position.y, position.z};
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);

	shaderIcon.use();
	shaderIcon.setInt("icon", 0);
	shaderDirection.use();
	shaderDirection.setVec3f("direction", direction);
	
	icon = createTexture("assets/light_icons/directional.png", TEXTURE_TYPE::DIFFUSE, true);
}

void DirectionalLight::draw()
{
	glBindVertexArray(vao);

	shaderIcon.use();
	shaderIcon.setMatrix("model", model);
	shaderIcon.setMatrix("view", view);
	shaderIcon.setMatrix("proj", proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, icon.id);
	glDrawArrays(GL_POINTS, 0, 1);
	
	shaderDirection.use();
	shaderDirection.setMatrix("model", model);
	shaderDirection.setMatrix("view", view);
	shaderDirection.setMatrix("proj", proj);
	shaderDirection.setVec3f("direction", glm::normalize(direction));
	shaderDirection.setVec3f("right", glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))));
	shaderDirection.setFloat("boxDim", -1.0f);
	// wireframe on
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.5f);
	glDrawArrays(GL_POINTS, 0, 1);
	// wireframe off
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glBindVertexArray(0);
}

void DirectionalLight::draw(float orthoDim)
{
	glBindVertexArray(vao);

	shaderIcon.use();
	shaderIcon.setMatrix("model", model);
	shaderIcon.setMatrix("view", view);
	shaderIcon.setMatrix("proj", proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, icon.id);
	glDrawArrays(GL_POINTS, 0, 1);
	
	shaderDirection.use();
	shaderDirection.setMatrix("model", model);
	shaderDirection.setMatrix("view", view);
	shaderDirection.setMatrix("proj", proj);
	shaderDirection.setVec3f("direction", direction);
	shaderDirection.setVec3f("right", glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))));
	shaderDirection.setFloat("boxDim", orthoDim);
	// wireframe on
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.5f);
	glDrawArrays(GL_POINTS, 0, 1);
	// wireframe off
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glBindVertexArray(0);
}

LIGHT_TYPE DirectionalLight::getType()
{
	return LIGHT_TYPE::DIRECTIONAL;
}

glm::vec3 DirectionalLight::getDirection()
{
	return direction;
}

void DirectionalLight::setDirection(glm::vec3 dir)
{
	direction = glm::normalize(dir);
}

SpotLight::SpotLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir, float innerAngle, float outerAngle) :
	Light(pos, amb, diff, spec),
	direction(dir),
	cutOff(glm::radians(innerAngle)),
	outerCutOff(glm::radians(outerAngle)),
	shaderIcon("shaders/light/spot/vertex.glsl", "shaders/light/spot/geometry.glsl", "shaders/light/spot/fragment.glsl"),
	shaderCutOff("shaders/light/spot/vertex_cutoff.glsl", "shaders/light/spot/geometry_cutoff.glsl", "shaders/light/spot/fragment_cutoff.glsl")
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float data[3] = {position.x, position.y, position.z};
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);

	shaderIcon.use();
	shaderIcon.setInt("icon", 0);
	shaderCutOff.use();
	shaderCutOff.setVec3f("direction", direction);
	shaderCutOff.setFloat("cutOff", cutOff);
	
	icon = createTexture("assets/light_icons/spot.png", TEXTURE_TYPE::DIFFUSE, true);
}

void SpotLight::draw()
{
	glBindVertexArray(vao);
	shaderIcon.use();
	shaderIcon.setMatrix("model", model);
	shaderIcon.setMatrix("view", view);
	shaderIcon.setMatrix("proj", proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, icon.id);
	glDrawArrays(GL_POINTS, 0, 1);

	shaderCutOff.use();
	shaderCutOff.setMatrix("model", model);
	shaderCutOff.setMatrix("view", view);
	shaderCutOff.setMatrix("proj", proj);
	shaderCutOff.setVec3f("direction", glm::normalize(direction));
	shaderCutOff.setVec3f("right", glm::normalize(glm::cross(direction, glm::vec3(0.0f, 1.0f, 0.0f))));
	shaderCutOff.setFloat("cutOff", cutOff);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}

LIGHT_TYPE SpotLight::getType()
{
	return LIGHT_TYPE::SPOT;
}

glm::vec3 SpotLight::getDirection()
{
	return direction;
}

void SpotLight::setDirection(glm::vec3 dir)
{
	direction = glm::normalize(dir);
}

float SpotLight::getCutOff()
{
	return cutOff;
}

float SpotLight::getOuterCutOff()
{
	return outerCutOff;
}

glm::vec3 Light::getPosition()
{
	return model * glm::vec4(position, 1.0f);
}

void Light::setPosition(glm::vec3 pos)
{
	model = glm::translate(glm::mat4(1.0f), pos - position);
}

glm::vec3 Light::getAmbientStrength()
{
	return ambientStrength;
}

glm::vec3 Light::getDiffuseStrength()
{
	return diffuseStrength;
}

glm::vec3 Light::getSpecularStrength()
{
	return specularStrength;
}

void Light::setAmbientStrength(glm::vec3 c)
{
	ambientStrength = c;
}

void Light::setDiffuseStrength(glm::vec3 c)
{
	diffuseStrength = c;
}

void Light::setSpecularStrength(glm::vec3 c)
{
	specularStrength = c;
}

void Light::setModelMatrix(glm::mat4 m)
{
	model = m;
	position = glm::vec3(m * glm::vec4(position, 1.0f));
}

void Light::setViewMatrix(glm::mat4 m)
{
	view = m;
}

void Light::setProjMatrix(glm::mat4 m)
{
	proj = m;
}
