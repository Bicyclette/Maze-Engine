#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <memory>
#include <utility>
#include <array>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "mesh.hpp"
#include "framebuffer.hpp"
#include "shader_light.hpp"
#include "helpers.hpp"

enum class SHADOW_QUALITY
{
	OFF = 0,
	TINY = 256,
	SMALL = 512,
	MED = 1024,
	HIGH = 2048,
	ULTRA = 4096
};

class Graphics
{
	public:

		Graphics(int width, int height);
		void setBloomEffect(bool b);
		bool bloomOn();
		float getBloomSigma();
		int getBloomSize();
		void setBloomSigma(float sigma);
		void setBloomSize(int size);
		void setSSAOEffect(bool ao);
		bool ssaoOn();
		void setShadowQuality(SHADOW_QUALITY quality);
		SHADOW_QUALITY getShadowQuality();
		void setLightView(glm::mat4 view);
		glm::mat4 & getLightView();
		glm::mat4 & getOrthoProjection();
		float getOrthoDimension();
		glm::mat4 & getOmniPerspProjection();
		glm::mat4 getSpotPerspProjection(float outerCutOff);
		Shader & getBlinnPhongShader();
		Shader & getPBRShader();
		Shader & getShadowMappingShader();
		Shader & getGBufferShader();
		Shader & getAOShader();
		Shader & getAOBlurShader();
		Shader & getGaussianBlurShader();
		Shader & getTentBlurShader();
		Shader & getUpSamplingShader();
		Shader & getDownSamplingShader();
		Shader & getFinalShader();
		std::unique_ptr<Framebuffer> & getMultisampleFBO();
		std::unique_ptr<Framebuffer> & getNormalFBO(int index);
		std::unique_ptr<Framebuffer> & getOmniDepthFBO(int index);
		std::unique_ptr<Framebuffer> & getStdDepthFBO(int index);
		std::unique_ptr<Framebuffer> & getGBufferFBO();
		std::unique_ptr<Framebuffer> & getAOFBO(int index);
		std::unique_ptr<Framebuffer> & getDownSamplingFBO(int index);
		std::unique_ptr<Framebuffer> & getPingPongFBO(int index);
		std::unique_ptr<Framebuffer> & getUpSamplingFBO(int index);
		std::unique_ptr<Mesh> & getQuadMesh();
		void resizeScreen(int width, int height);
		std::vector<glm::vec3> & getAOKernel();
		GLuint getAONoiseTexture();

	private:

		std::unique_ptr<Framebuffer> multisample; // color + depth + stencil
		std::array<std::unique_ptr<Framebuffer>, 2> normal; // only color, no multisampling
		std::array<std::unique_ptr<Framebuffer>, 10> omniDepth; // for omnidirectional shadow mapping
		std::array<std::unique_ptr<Framebuffer>, 10> stdDepth; // for directional and spotlight shadow mapping
		std::unique_ptr<Framebuffer> GBuffer; // position + normal + color
		std::array<std::unique_ptr<Framebuffer>, 2> AOBuffer; // single color component (RED) for ambient occlusion data
		std::array<std::unique_ptr<Framebuffer>, 6> downSampling; // only color, no multisampling
		std::array<std::unique_ptr<Framebuffer>, 12> ping_pong; // only color, no multisampling
		std::array<std::unique_ptr<Framebuffer>, 12> upSampling; // only color, no multisampling

		bool bloomEffect;
		float bloomSigma;
		int bloomSize;
		bool ssaoEffect;
		SHADOW_QUALITY shadowQuality;
		float orthoDimension;
		glm::mat4 orthoProjection; // for directional light
		glm::mat4 omniPerspProjection; // for point lights
		GLuint aoNoiseTexture;
		std::vector<glm::vec3> aoKernel;

		Shader blinnPhong;
		Shader pbr;
		Shader shadowMapping;
		Shader gBuffer;
		Shader ao;
		Shader aoBlur;
		Shader gaussianBlur;
		Shader tentBlur;
		Shader downSample;
		Shader upSample;
		Shader end;

		std::unique_ptr<Mesh> quad;
		Material quadMaterial;
};

#endif
