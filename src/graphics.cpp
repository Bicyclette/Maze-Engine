#include "graphics.hpp"

Graphics::Graphics(int width, int height) :
	tone_mapping(TONE_MAPPING::REINHARD),
	near(0.1f),
	far(100.0f),
	shadows(true),
	bloomEffect{true},
	bloomSigma(4.0f),
	bloomSize(15),
	ssaoEffect{true},
	multisample{std::make_unique<Framebuffer>(true, true, true)},
	normal{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
	},
	omniDepth{
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false)
	},
	stdDepth{
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false),
		std::make_unique<Framebuffer>(false)
		},
	GBuffer{std::make_unique<Framebuffer>(true, false, true)},
	AOBuffer{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
	},
	downSampling{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
		},
	ping_pong{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
		},
	upSampling{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
		},
	omniPerspProjection(glm::perspective(glm::radians(90.0f), 1.0f, near, far)),
	blinnPhong("shaders/blinn_phong/vertex.glsl", "shaders/blinn_phong/fragment.glsl", SHADER_TYPE::BLINN_PHONG),
	pbr("shaders/PBR/vertex.glsl", "shaders/PBR/fragment.glsl", SHADER_TYPE::PBR),
	shadowMapping("shaders/shadowMapping/vertex.glsl", "shaders/shadowMapping/geometry.glsl", "shaders/shadowMapping/fragment.glsl", SHADER_TYPE::SHADOWS),
	gBuffer("shaders/GBuffer/vertex.glsl", "shaders/GBuffer/fragment.glsl", SHADER_TYPE::GBUFFER),
	ao("shaders/AO/vertex.glsl", "shaders/AO/fragment.glsl", SHADER_TYPE::AO),
	aoBlur("shaders/AO/blur/vertex.glsl", "shaders/AO/blur/fragment.glsl", SHADER_TYPE::AO),
	gaussianBlur("shaders/gaussianBlur/vertex.glsl", "shaders/gaussianBlur/fragment.glsl", SHADER_TYPE::BLUR),
	tentBlur("shaders/tentBlur/vertex.glsl", "shaders/tentBlur/fragment.glsl", SHADER_TYPE::BLUR),
	downSample("shaders/downSampling/vertex.glsl", "shaders/downSampling/fragment.glsl", SHADER_TYPE::SAMPLING),
	upSample("shaders/upSampling/vertex.glsl", "shaders/upSampling/fragment.glsl", SHADER_TYPE::SAMPLING),
	end("shaders/final/vertex.glsl", "shaders/final/fragment.glsl", SHADER_TYPE::FINAL)
{
	// Generic Multisample FBO
	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::RENDER_BUFFER, ATTACHMENT_TARGET::DEPTH_STENCIL, width, height);

	for(int i{0}; i < 2; ++i)
		normal[i]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);

	for(int i{0}; i < 10; ++i)
	{
		omniDepth[i]->addAttachment(ATTACHMENT_TYPE::TEXTURE_CUBE_MAP, ATTACHMENT_TARGET::DEPTH, static_cast<int>(SHADOW_QUALITY::TINY), static_cast<int>(SHADOW_QUALITY::TINY));
		stdDepth[i]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::DEPTH, static_cast<int>(SHADOW_QUALITY::TINY), static_cast<int>(SHADOW_QUALITY::TINY));
	}

	// SSAO G-BUFFER FBO
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height, GL_NEAREST);
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	GBuffer->addAttachment(ATTACHMENT_TYPE::RENDER_BUFFER, ATTACHMENT_TARGET::DEPTH, width, height);

	// AMBIENT OCCLUSION
	for(int i{0}; i < 2; ++i)
		AOBuffer[i]->addSingleColorTextureAttachment(GL_RED, GL_NEAREST, width, height);

	std::uniform_real_distribution<float> randomFloats(0.0f, 1.0f);
	std::default_random_engine generator;
	for(int i{0}; i < 32; ++i)
	{
		glm::vec3 sample(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);

		float scale = static_cast<float>(i) / 32.0f;
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		aoKernel.push_back(sample);
	}

	std::vector<glm::vec3> aoNoise;
	for(int i{0}; i < 16; ++i)
	{
		glm::vec3 noise(randomFloats(generator) * 2.0f - 1.0f, randomFloats(generator) * 2.0f - 1.0f, 0.0f);
		aoNoise.push_back(noise);
	}

	glGenTextures(1, &aoNoiseTexture);
	glBindTexture(GL_TEXTURE_2D, aoNoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 4, 4, 0, GL_RGB, GL_FLOAT, &aoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// BLOOM FBOs
	for(int i{1}; i <= 6; ++i)
	{
		int factor = std::pow(2, i);
		int up = std::pow(2, 6-i);
		int w = width / factor;
		int h = height / factor;
		downSampling[i-1]->addAttachment(
				ATTACHMENT_TYPE::TEXTURE,
				ATTACHMENT_TARGET::COLOR,
				w, h);
		ping_pong[(i-1)*2]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, w, h);
		ping_pong[(i-1)*2+1]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, w, h);
		upSampling[(i-1)*2]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width/up, height/up);
		upSampling[(i-1)*2+1]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width/up, height/up);
	}

	// quad mesh for rendering final image
	glm::vec3 normal(0.0f, 0.0f, 1.0f);
	std::vector<Vertex> vertices{{
		Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), normal, glm::vec2(0.0f, 0.0f)),
		Vertex(glm::vec3(1.0f, -1.0f, 0.0f), normal, glm::vec2(1.0f, 0.0f)),
		Vertex(glm::vec3(1.0f, 1.0f, 0.0f), normal, glm::vec2(1.0f, 1.0f)),
		Vertex(glm::vec3(-1.0f, 1.0f, 0.0f), normal, glm::vec2(0.0f, 1.0f))
	}};

	std::vector<int> indices{{
		0, 1, 2,
		0, 2, 3
	}};

	quadMaterial.opacity = 1.0f;
	quad = std::make_unique<Mesh>(vertices, indices, quadMaterial, "final image");
}

void Graphics::setNearPlane(float nearPlane)
{
	near = nearPlane;
}

void Graphics::setFarPlane(float farPlane)
{
	far = farPlane;
}

void Graphics::setShadows(bool s)
{
	shadows = s;
}

bool Graphics::shadowsOn()
{
	return shadows;
}

void Graphics::setBloomEffect(bool b)
{
	bloomEffect = b;
}

bool Graphics::bloomOn()
{
	return bloomEffect;
}

float Graphics::getBloomSigma()
{
	return bloomSigma;
}

int Graphics::getBloomSize()
{
	return bloomSize;
}

void Graphics::setBloomSigma(float sigma)
{
	bloomSigma = sigma;
}

void Graphics::setBloomSize(int size)
{
	bloomSize = size;
}

void Graphics::setSSAOEffect(bool ao)
{
	ssaoEffect = ao;
}

bool Graphics::ssaoOn()
{
	return ssaoEffect;
}

void Graphics::set_tone_mapping(TONE_MAPPING tone)
{
	tone_mapping = tone;
}

TONE_MAPPING Graphics::get_tone_mapping()
{
	return tone_mapping;
}

void Graphics::setStdShadowQuality(SHADOW_QUALITY quality, int index)
{
	if(quality != SHADOW_QUALITY::OFF)
	{
		stdDepth[index]->updateAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::DEPTH, static_cast<int>(quality), static_cast<int>(quality));
	}
}

void Graphics::setOmniShadowQuality(SHADOW_QUALITY quality, int index)
{
	if(quality != SHADOW_QUALITY::OFF)
	{
		omniDepth[index]->updateAttachment(ATTACHMENT_TYPE::TEXTURE_CUBE_MAP, ATTACHMENT_TARGET::DEPTH, static_cast<int>(quality), static_cast<int>(quality));
	}
}

glm::mat4 Graphics::getOrthoProjection(float orthoDimension)
{
	return glm::ortho(-orthoDimension, orthoDimension, -orthoDimension, orthoDimension, near, far);
}

glm::mat4 & Graphics::getOmniPerspProjection()
{
	return omniPerspProjection;
}

glm::mat4 Graphics::getSpotPerspProjection(float outerCutOff, float shadowQuality)
{
	float aspectRatio = static_cast<float>(shadowQuality) / static_cast<float>(shadowQuality);
	return glm::perspective(glm::radians(outerCutOff * 2.0f), aspectRatio, near, far);
}

Shader & Graphics::getBlinnPhongShader()
{
	return blinnPhong;
}

Shader & Graphics::getPBRShader()
{
	return pbr;
}

Shader & Graphics::getShadowMappingShader()
{
	return shadowMapping;
}

Shader & Graphics::getGBufferShader()
{
	return gBuffer;
}

Shader & Graphics::getAOShader()
{
	return ao;
}

Shader & Graphics::getAOBlurShader()
{
	return aoBlur;
}

Shader & Graphics::getGaussianBlurShader()
{
	return gaussianBlur;
}

Shader & Graphics::getTentBlurShader()
{
	return tentBlur;
}

Shader & Graphics::getDownSamplingShader()
{
	return downSample;
}

Shader & Graphics::getUpSamplingShader()
{
	return upSample;
}

Shader & Graphics::getFinalShader()
{
	return end;
}

std::unique_ptr<Framebuffer> & Graphics::getMultisampleFBO()
{
	return multisample;
}

std::unique_ptr<Framebuffer> & Graphics::getNormalFBO(int index)
{
	return normal[index];
}

std::unique_ptr<Framebuffer> & Graphics::getOmniDepthFBO(int index)
{
	return omniDepth[index];
}

std::unique_ptr<Framebuffer> & Graphics::getStdDepthFBO(int index)
{
	return stdDepth[index];
}

std::unique_ptr<Framebuffer> & Graphics::getGBufferFBO()
{
	return GBuffer;
}

std::unique_ptr<Framebuffer> & Graphics::getAOFBO(int index)
{
	return AOBuffer[index];
}

std::unique_ptr<Framebuffer> & Graphics::getDownSamplingFBO(int index)
{
	return downSampling[index];
}

std::unique_ptr<Framebuffer> & Graphics::getPingPongFBO(int index)
{
	return ping_pong[index];
}

std::unique_ptr<Framebuffer> & Graphics::getUpSamplingFBO(int index)
{
	return upSampling[index];
}

std::unique_ptr<Mesh> & Graphics::getQuadMesh()
{
	return quad;
}

void Graphics::resizeScreen(int width, int height)
{
	multisample = std::make_unique<Framebuffer>(true, true, true);
	normal = std::array<std::unique_ptr<Framebuffer>, 2>{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
	};
	GBuffer = std::make_unique<Framebuffer>(true, false, true);
	AOBuffer = std::array<std::unique_ptr<Framebuffer>, 2>{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
	};
	downSampling = std::array<std::unique_ptr<Framebuffer>, 6>{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
	};
	ping_pong = std::array<std::unique_ptr<Framebuffer>, 12>{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
	};
	upSampling = std::array<std::unique_ptr<Framebuffer>, 12>{
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true),
		std::make_unique<Framebuffer>(true, false, true)
	};
	
	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::RENDER_BUFFER, ATTACHMENT_TARGET::DEPTH_STENCIL, width, height);

	for(int i{0}; i < 2; ++i)
		normal[i]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);

	// SSAO G-BUFFER FBO
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height, GL_NEAREST);
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	GBuffer->addAttachment(ATTACHMENT_TYPE::RENDER_BUFFER, ATTACHMENT_TARGET::DEPTH, width, height);

	// AMBIENT OCCLUSION BUFFER
	for(int i{0}; i < 2; ++i)
		AOBuffer[i]->addSingleColorTextureAttachment(GL_RED, GL_NEAREST, width, height);

	// bloom ping pong FBOs
	for(int i{1}; i <= 6; ++i)
	{
		int factor = std::pow(2, i);
		int up = std::pow(2, 6-i);
		int w = width / factor;
		int h = height / factor;
		downSampling[i-1]->addAttachment(
				ATTACHMENT_TYPE::TEXTURE,
				ATTACHMENT_TARGET::COLOR,
				w, h);
		ping_pong[(i-1)*2]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, w, h);
		ping_pong[(i-1)*2+1]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, w, h);
		upSampling[(i-1)*2]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width/up, height/up);
		upSampling[(i-1)*2+1]->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width/up, height/up);
	}
}

std::vector<glm::vec3> & Graphics::getAOKernel()
{
	return aoKernel;
}

GLuint Graphics::getAONoiseTexture()
{
	return aoNoiseTexture;
}
