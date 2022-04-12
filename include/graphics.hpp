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
#include "renderTexture.hpp"
#include "shader_light.hpp"
#include "helpers.hpp"

enum class TONE_MAPPING
{
	REINHARD = 0,
	ACES = 1,
	OFF = 2
};

class Graphics
{
	public:

		Graphics(int width, int height);
		void setNearPlane(float nearPlane);
		void setFarPlane(float farPlane);
		void setShadows(bool s);
		bool shadowsOn();
		void setBloomEffect(bool b);
		bool bloomOn();
		float getBloomSigma();
		int getBloomSize();
		void setBloomSigma(float sigma);
		void setBloomSize(int size);
		void setSSAOEffect(bool ao);
		bool ssaoOn();
        int getAOSampleCount();
        void setAOSampleCount(int samples);
        float getAORadius();
        void setAORadius(float radius);
		void setVolumetricLighting(bool v);
		bool volumetricLightingOn();
		void set_scene_tone_mapping(TONE_MAPPING tone);
		void set_ui_tone_mapping(TONE_MAPPING tone);
		TONE_MAPPING get_scene_tone_mapping();
		TONE_MAPPING get_ui_tone_mapping();
		void setStdShadowQuality(SHADOW_QUALITY quality, int index);
		void setOmniShadowQuality(SHADOW_QUALITY quality, int index);
		glm::mat4 getOrthoProjection(float orthoDimension);
		glm::mat4 & getOmniPerspProjection();
		glm::mat4 getSpotPerspProjection(float outerCutOff, float shadowQuality);
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
		Shader & getVolumetricLightingShader();
        Shader & getVolumetricDownSamplingShader();
        Shader & getBilateralBlurShader();
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
		std::unique_ptr<Framebuffer> & getVolumetricsFBO(int index);
		std::unique_ptr<Mesh> & getQuadMesh();
		void resizeScreen(int width, int height);
		std::vector<glm::vec3> & getAOKernel();
		GLuint getAONoiseTexture();
		GLuint getBloomTexture(int index);

	public:

		std::unique_ptr<Framebuffer> multisample; // color + depth + stencil
		std::array<std::unique_ptr<Framebuffer>, 2> normal; // only color, no multisampling
		std::array<std::unique_ptr<Framebuffer>, 10> omniDepth; // for omnidirectional shadow mapping
		std::array<std::unique_ptr<Framebuffer>, 10> stdDepth; // for directional and spotlight shadow mapping
		std::unique_ptr<Framebuffer> GBuffer; // view position + normal + view depth + world position
		std::array<std::unique_ptr<Framebuffer>, 2> AOBuffer; // single color component (RED) for ambient occlusion data
		std::array<std::unique_ptr<Framebuffer>, 6> downSampling; // only color, no multisampling
		std::array<std::unique_ptr<Framebuffer>, 12> ping_pong; // only color, no multisampling
		std::array<std::unique_ptr<Framebuffer>, 12> upSampling; // only color, no multisampling
		std::array<GLuint, 2> bloomTexture;
        std::array<std::unique_ptr<Framebuffer>, 4> volumetrics; // hdr color, no multisampling
        std::unique_ptr<Framebuffer> motionBlurFBO;
        std::unique_ptr<Framebuffer> userInterfaceFBO;
		std::array<std::unique_ptr<Framebuffer>, 2> compositeFBO;

		TONE_MAPPING scene_tone_mapping;
		TONE_MAPPING ui_tone_mapping;
		float near;
		float far;
		bool shadows;
		bool bloomEffect;
		float bloomSigma;
		int bloomSize;
		bool ssaoEffect;
        int ssaoSampleCount;
        float ssaoRadius;
		bool volumetricsOn;
        bool motionBlurFX;
        int motionBlurStrength;
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
		Shader volumetricLighting;
		Shader VLDownSample;
        Shader bilateralBlur;
        Shader motionBlur;
		Shader sceneCompositing;
		Shader uiCompositing;
		Shader end;

		std::unique_ptr<Mesh> quad;
		Material quadMaterial;
};

#endif
