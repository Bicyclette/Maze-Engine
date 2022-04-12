#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <utility>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>
#include <cstdio>
#include <string.h>

class Light;
class PointLight;
class DirectionalLight;
class SpotLight;

enum class SHADER_TYPE
{
	BLINN_PHONG,
	PBR,
	SHADOWS,
	GBUFFER,
	AO,
	BLUR,
	SAMPLING,
	VOLUMETRIC_LIGHTING,
    COMPUTE,
    UI,
    TEXT,
	COMPOSITING,
	FINAL
};

class Shader
{
	public:

		Shader(const std::string & vertex_shader_file, const std::string & fragment_shader_file, SHADER_TYPE t = SHADER_TYPE::BLINN_PHONG);
		Shader(const std::string & vertex_shader_file, const std::string & geometry_shader_file, const std::string & fragment_shader_file, SHADER_TYPE t = SHADER_TYPE::BLINN_PHONG);
		Shader(const std::string & compute_shader_file, SHADER_TYPE t = SHADER_TYPE::COMPUTE);
		~Shader();
		GLuint getId() const;
		SHADER_TYPE getType();
		void setInt(const std::string & name, int v) const;
		void setFloat(const std::string & name, float v) const;
		void setBool(const std::string & name, bool v) const;
		void setVec2f(const std::string & name, glm::vec2 v) const;
		void setVec3f(const std::string & name, glm::vec3 v) const;
		void setVec4f(const std::string & name, glm::vec4 v) const;
		void setMatrix(const std::string & name, glm::mat4 m) const;
		void setLighting(std::vector<std::shared_ptr<PointLight>> & pLights, std::vector<std::shared_ptr<DirectionalLight>> & dLights, std::vector<std::shared_ptr<SpotLight>> & sLight);
		void use() const;
        void dispatch(int blocks_x, int blocks_y, int blocks_z, GLbitfield barriers);

	private:

		void compile(const char * vertex_shader_code, const char * fragment_shader_code);
		void compile(const char * vertex_shader_code, const char * geometry_shader_code, const char * fragment_shader_code);
		void compile(const char * compute_shader_code);

		GLuint id;
		SHADER_TYPE type;
};

enum class TEXTURE_TYPE
{
	DIFFUSE,
	SPECULAR,
	NORMAL,
	METALLIC_ROUGHNESS,
	EMISSIVE
};

struct Texture
{
	GLuint id;
	TEXTURE_TYPE type;
	std::string path;

	Texture() : id(-1) {}

	Texture(GLuint aId, TEXTURE_TYPE aType, std::string aPath)
	{
		id = aId;
		type = aType;
		path = aPath;
	}
};

struct Material
{
    int opaque;
	float opacity;
    glm::vec3 color_diffuse;
	glm::vec3 color_specular;
	glm::vec3 color_ambient;
	glm::vec3 color_emissive;
	float shininess;
	float roughness;
	float metallic;
	float emission_intensity;
	std::vector<Texture> textures; // [0] = diffuse, [1] = specular, [2] = normal, [3] = metallicRough, [4] = emissive
};

struct Texture createTexture(const std::string & texPath, TEXTURE_TYPE t, bool flip);
struct Texture createTextureFromData(aiTexture* embTex, TEXTURE_TYPE t, bool flip);

enum class LIGHT_TYPE
{
	POINT,
	DIRECTIONAL,
	SPOT
};

enum class SHADOW_QUALITY
{
	OFF = 0,
	TINY = 256,
	SMALL = 512,
	MED = 1024,
	HIGH = 2048,
	ULTRA = 4096
};

class Light
{
	public:

		Light(SHADOW_QUALITY quality, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
		virtual ~Light();
		virtual void draw() = 0;
		glm::vec3 getPosition();
		void setPosition(glm::vec3 pos);
		glm::vec3 getAmbientStrength();
		glm::vec3 getDiffuseStrength();
		glm::vec3 getSpecularStrength();
		void setAmbientStrength(glm::vec3 c);
		void setDiffuseStrength(glm::vec3 c);
		void setSpecularStrength(glm::vec3 c);
		void setModelMatrix(glm::mat4 m);
		void setViewMatrix(glm::mat4 m);
		void setProjMatrix(glm::mat4 m);
		virtual LIGHT_TYPE getType() = 0;
		void setShadowQuality(SHADOW_QUALITY quality);
		SHADOW_QUALITY getShadowQuality();
        bool getVolumetric();
        void setVolumetric(bool volumetric);
        bool getFog();
        void setFog(bool fog);
        float getFogGain();
        void setFogGain(float fog_gain);
        float get_tau();
        void set_tau(float tau);
        float get_phi();
        void set_phi(float phi);

	protected:

		SHADOW_QUALITY shadow_quality;
		glm::vec3 ambientStrength;
		glm::vec3 diffuseStrength;
		glm::vec3 specularStrength;

		glm::vec3 position;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
		
		GLuint vao;
		GLuint vbo;

        bool m_volumetric;
        bool m_fog;
        float m_fog_gain;
        float m_tau; // light collision probability
        float m_phi; // light power
		
		struct Texture icon;
};

class PointLight : public Light
{
	public:

		PointLight(SHADOW_QUALITY quality, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float aKc, float aKl, float aKq);
		virtual void draw() override;
		float getKc();
		float getKl();
		float getKq();
		virtual LIGHT_TYPE getType() override;

		static constexpr float attenuation[12][4] =
		{
			{7.0f, 1.0f, 0.7f, 1.8f},				//0
			{13.0f, 1.0f, 0.35f, 0.44f},			//1
			{20.0f, 1.0f, 0.22f, 0.2f},				//2
			{32.0f, 1.0f, 0.14f, 0.07f},			//3
			{50.0f, 1.0f, 0.09f, 0.032f},			//4
			{65.0f, 1.0f, 0.07f, 0.017f},			//5
			{100.0f, 1.0f, 0.045f, 0.0075f},		//6
			{160.0f, 1.0f, 0.027f, 0.0028f},		//7
			{200.0f, 1.0f, 0.022f, 0.0019f},		//8
			{325.0f, 1.0f, 0.014f, 0.0007f},		//9
			{600.0f, 1.0f, 0.007f, 0.0002f},		//10
			{3250.0f, 1.0f, 0.0014f, 0.000007f}		//11
		};

	private:

		float kc;
		float kl;
		float kq;
		Shader shader;
};

class DirectionalLight : public Light
{
	public:

		DirectionalLight(SHADOW_QUALITY quality, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir, float orthoDim = 10.0f);
		virtual void draw() override;
		void drawDebug();
		glm::vec3 getDirection();
		void setDirection(glm::vec3 dir);
		void setOrthoDimension(float dimension);
		float getOrthoDimension();
		virtual LIGHT_TYPE getType() override;

	private:

		float orthoDimension;
		glm::vec3 direction;
		Shader shaderIcon;
		Shader shaderDirection;
};

class SpotLight : public Light
{
	public:

		SpotLight(SHADOW_QUALITY quality, glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, glm::vec3 dir, float innerAngle, float outerAngle);
		virtual void draw() override;
		glm::vec3 getDirection();
		void setDirection(glm::vec3 dir);
		float getCutOff();
		float getOuterCutOff();
		virtual LIGHT_TYPE getType() override;

	private:

		glm::vec3 direction;
		float cutOff; // radians
		float outerCutOff; // radians
		Shader shaderIcon;
		Shader shaderCutOff;
};

#endif
