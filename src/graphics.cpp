#include "graphics.hpp"

Graphics::Graphics(int width, int height) :
	bloomEffect{true},
	ssaoEffect(true),
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
	ping{std::make_unique<Framebuffer>(true, false, true)},
	pong{std::make_unique<Framebuffer>(true, false, true)},
	shadowQuality(SHADOW_QUALITY::ULTRA),
	orthoDimension(10.0f),
	orthoProjection(glm::ortho(-orthoDimension, orthoDimension, -orthoDimension, orthoDimension, 0.1f, 100.0f)),
	omniPerspProjection(glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f)),
	blinnPhong("../shaders/blinn_phong/vertex.glsl", "../shaders/blinn_phong/fragment.glsl", SHADER_TYPE::BLINN_PHONG),
	pbr("../shaders/PBR/vertex.glsl", "../shaders/PBR/fragment.glsl", SHADER_TYPE::PBR),
	shadowMapping("../shaders/shadowMapping/vertex.glsl", "../shaders/shadowMapping/geometry.glsl", "../shaders/shadowMapping/fragment.glsl", SHADER_TYPE::SHADOWS),
	gBuffer("../shaders/GBuffer/vertex.glsl", "../shaders/GBuffer/fragment.glsl", SHADER_TYPE::GBUFFER),
	ao("../shaders/AO/vertex.glsl", "../shaders/AO/fragment.glsl", SHADER_TYPE::AO),
	aoBlur("../shaders/AO/blur/vertex.glsl", "../shaders/AO/blur/fragment.glsl", SHADER_TYPE::AO),
	bloom("../shaders/bloom/vertex.glsl", "../shaders/bloom/fragment.glsl", SHADER_TYPE::BLOOM),
	end("../shaders/final/vertex.glsl", "../shaders/final/fragment.glsl", SHADER_TYPE::FINAL)
{
	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::RENDER_BUFFER, ATTACHMENT_TARGET::DEPTH_STENCIL, width, height);

	for(int i{0}; i < 2; ++i)
		normal.at(i)->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);

	if(shadowQuality != SHADOW_QUALITY::OFF)
	{
		for(int i{0}; i < 10; ++i)
		{
			omniDepth.at(i)->addAttachment(ATTACHMENT_TYPE::TEXTURE_CUBE_MAP, ATTACHMENT_TARGET::DEPTH, static_cast<int>(shadowQuality), static_cast<int>(shadowQuality));
			stdDepth.at(i)->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::DEPTH, static_cast<int>(shadowQuality), static_cast<int>(shadowQuality));
		}
	}

	// SSAO G-BUFFER FBO
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height, GL_NEAREST);
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	GBuffer->addAttachment(ATTACHMENT_TYPE::RENDER_BUFFER, ATTACHMENT_TARGET::DEPTH, width, height);

	// AMBIENT OCCLUSION
	for(int i{0}; i < 2; ++i)
		AOBuffer.at(i)->addSingleColorTextureAttachment(GL_RED, GL_NEAREST, width, height);

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

	// BLOOM ping pong FBOs
	ping->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	pong->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);

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

	quad = std::make_unique<Mesh>(vertices, indices, quadMaterial, "final image");
}

void Graphics::setBloomEffect(bool b)
{
	bloomEffect = b;
}

bool Graphics::bloomOn()
{
	return bloomEffect;
}

void Graphics::setSSAOEffect(bool ao)
{
	ssaoEffect = ao;
}

bool Graphics::ssaoOn()
{
	return ssaoEffect;
}

void Graphics::setShadowQuality(SHADOW_QUALITY quality)
{
	shadowQuality = quality;
	if(shadowQuality != SHADOW_QUALITY::OFF)
	{
		for(int i{0}; i < 10; ++i)
		{
			omniDepth.at(i)->updateAttachment(ATTACHMENT_TYPE::TEXTURE_CUBE_MAP, ATTACHMENT_TARGET::DEPTH, static_cast<int>(shadowQuality), static_cast<int>(shadowQuality));
			stdDepth.at(i)->updateAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::DEPTH, static_cast<int>(shadowQuality), static_cast<int>(shadowQuality));
		}
	}
}

SHADOW_QUALITY Graphics::getShadowQuality()
{
	return shadowQuality;
}

glm::mat4 & Graphics::getOrthoProjection()
{
	return orthoProjection;
}

float Graphics::getOrthoDimension()
{
	return orthoDimension;
}

glm::mat4 & Graphics::getOmniPerspProjection()
{
	return omniPerspProjection;
}

glm::mat4 Graphics::getSpotPerspProjection(float outerCutOff)
{
	float aspectRatio = static_cast<float>(shadowQuality) / static_cast<float>(shadowQuality);
	return glm::perspective(glm::radians(outerCutOff * 2.0f), aspectRatio, 0.1f, 100.0f);
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

Shader & Graphics::getBloomShader()
{
	return bloom;
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
	return normal.at(index);
}

std::unique_ptr<Framebuffer> & Graphics::getOmniDepthFBO(int index)
{
	return omniDepth.at(index);
}

std::unique_ptr<Framebuffer> & Graphics::getStdDepthFBO(int index)
{
	return stdDepth.at(index);
}

std::unique_ptr<Framebuffer> & Graphics::getGBufferFBO()
{
	return GBuffer;
}

std::unique_ptr<Framebuffer> & Graphics::getAOFBO(int index)
{
	return AOBuffer.at(index);
}

std::unique_ptr<Framebuffer> & Graphics::getPingFBO()
{
	return ping;
}

std::unique_ptr<Framebuffer> & Graphics::getPongFBO()
{
	return pong;
}

std::unique_ptr<Mesh> & Graphics::getQuadMesh()
{
	return quad;
}

void Graphics::resizeScreen(int width, int height)
{
	/*
	multisample->updateAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->updateAttachment(ATTACHMENT_TYPE::RENDER_BUFFER, ATTACHMENT_TARGET::DEPTH_STENCIL, width, height);
	for(int i{0}; i < 2; ++i)
		normal.at(i)->updateAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	ping->updateAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	pong->updateAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	GBuffer->updateAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	GBuffer->updateAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::DEPTH, width, height);
	*/

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
	ping = std::make_unique<Framebuffer>(true, false, true);
	pong = std::make_unique<Framebuffer>(true, false, true);
	
	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	multisample->addAttachment(ATTACHMENT_TYPE::RENDER_BUFFER, ATTACHMENT_TARGET::DEPTH_STENCIL, width, height);

	for(int i{0}; i < 2; ++i)
		normal.at(i)->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);

	// SSAO G-BUFFER FBO
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height, GL_NEAREST);
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	GBuffer->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	GBuffer->addAttachment(ATTACHMENT_TYPE::RENDER_BUFFER, ATTACHMENT_TARGET::DEPTH, width, height);

	// AMBIENT OCCLUSION BUFFER
	for(int i{0}; i < 2; ++i)
		AOBuffer.at(i)->addSingleColorTextureAttachment(GL_RED, GL_NEAREST, width, height);

	// bloom ping pong FBOs
	ping->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
	pong->addAttachment(ATTACHMENT_TYPE::TEXTURE, ATTACHMENT_TARGET::COLOR, width, height);
}

std::vector<glm::vec3> & Graphics::getAOKernel()
{
	return aoKernel;
}

GLuint Graphics::getAONoiseTexture()
{
	return aoNoiseTexture;
}
