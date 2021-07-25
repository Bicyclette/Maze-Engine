#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <GL/glew.h>
#include <vector>
#include <functional>
#include <memory>
#include <utility>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <random>
#include "shader_light.hpp"

constexpr int MAX_PARTICLES{1'000};

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	float lifetime;

	Particle(glm::vec3 p = glm::vec3(0.0f),
			glm::vec3 v = glm::vec3(0.0f),
			float life = 0.0f)
	{
		position = p;
		velocity = v;
		lifetime = life;
	}
};

// [-0.5:0.5] particle generation range around position
class ParticleEmitter
{
	public:
		enum class DIRECTION
		{
			VECTOR,
			RADIAL
		};

	public:

		ParticleEmitter(glm::vec3 pos, int emitRate, float aMaxLifetime, ParticleEmitter::DIRECTION aDirectionType, float aSpeed, glm::vec3 aDirectionVector = glm::vec3(0.0f));
		~ParticleEmitter();
		glm::vec3 getPosition();
		void setPosition(glm::vec3 pos);
		int getEmissionRate();
		void setEmissionRate(int rate);
		float getMaxLifetime();
		void setMaxLifetime(float value);
		void drawEmitter(glm::mat4 view, glm::mat4 proj);
		void drawParticles(glm::mat4 view, glm::mat4 proj, glm::vec3 camRight, glm::vec3 camUp);
		void emit(glm::vec3 camPos, float delta);

	private:

		glm::vec3 position;
		int emissionRate; // number of particles per sec
		float emitDelta;
		float maxLifetime;
		DIRECTION direction_type;
		glm::vec3 direction_vector;
		float speed;
		std::random_device rd;
		std::mt19937 mt;
		std::uniform_real_distribution<float> distribution;
		std::vector<Particle> particles;
		GLuint emitter_vao;
		GLuint emitter_vbo;
		GLuint particles_vao;
		GLuint particles_vbo;
		Shader emitter_shader;
		Shader particles_shader;

	private:

		GLuint fireAtlas;
};

#endif
