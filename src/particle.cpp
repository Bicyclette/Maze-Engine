#include "particle.hpp"
#include "stb_image.h"

ParticleEmitter::ParticleEmitter(glm::vec3 pos, int emitRate, float aMaxLifetime, DIRECTION aDirectionType, float aSpeed, glm::vec3 aDirectionVector) :
	position{pos},
	emissionRate{emitRate},
	emitDelta{1.0f / static_cast<float>(emissionRate)},
	maxLifetime{aMaxLifetime},
	direction_type(aDirectionType),
	direction_vector(glm::normalize(aDirectionVector)),
	speed(aSpeed),
	mt(rd()),
	distribution(-0.5f, 0.5f),
	emitter_shader("shaders/particles/emitter/vertex.glsl", "shaders/particles/emitter/geometry.glsl", "shaders/particles/emitter/fragment.glsl"),
	particles_shader("shaders/particles/vertex.glsl", "shaders/particles/geometry.glsl", "shaders/particles/fragment.glsl")
{
	// emitter VAO
	glGenVertexArrays(1, &emitter_vao);
	glBindVertexArray(emitter_vao);

	glGenBuffers(1, &emitter_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, emitter_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float), glm::value_ptr(position), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	// particles VAO
	glGenVertexArrays(1, &particles_vao);
	glBindVertexArray(particles_vao);

	glGenBuffers(1, &particles_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, particles_vbo);
	glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(Particle), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(offsetof(Particle, velocity)));
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)(offsetof(Particle, lifetime)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	// load fire texture
	stbi_set_flip_vertically_on_load(true);

	int width, height, channels;
	unsigned char * data = stbi_load("assets/particles_atlas/fire/fire2.png", &width, &height, &channels, 0);

	glGenTextures(1, &fireAtlas);
	glBindTexture(GL_TEXTURE_2D, fireAtlas);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
}

ParticleEmitter::~ParticleEmitter()
{
	glBindVertexArray(emitter_vao);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &emitter_vbo);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &emitter_vao);
}

glm::vec3 ParticleEmitter::getPosition()
{
	return position;
}

void ParticleEmitter::setPosition(glm::vec3 pos)
{
	position = pos;
}

int ParticleEmitter::getEmissionRate()
{
	return emissionRate;
}

void ParticleEmitter::setEmissionRate(int rate)
{
	emissionRate = rate;
	emitDelta = 1.0f / static_cast<float>(emissionRate);
}

float ParticleEmitter::getMaxLifetime()
{
	return maxLifetime;
}

void ParticleEmitter::setMaxLifetime(float value)
{
	maxLifetime = value;
}

void ParticleEmitter::drawEmitter(glm::mat4 view, glm::mat4 proj)
{
	emitter_shader.use();
	emitter_shader.setMatrix("model", glm::mat4(1.0f));
	emitter_shader.setMatrix("view", view);
	emitter_shader.setMatrix("proj", proj);
	glBindVertexArray(emitter_vao);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ParticleEmitter::drawParticles(glm::mat4 view, glm::mat4 proj, glm::vec3 camRight, glm::vec3 camUp)
{
	particles_shader.use();
	particles_shader.setMatrix("model", glm::mat4(1.0f));
	particles_shader.setMatrix("view", view);
	particles_shader.setMatrix("proj", proj);
	particles_shader.setVec3f("camRight", camRight);
	particles_shader.setVec3f("camUp", camUp);
	particles_shader.setFloat("maxLifetime", maxLifetime);
	particles_shader.setFloat("numImagesX", 8.0f);
	particles_shader.setFloat("numImagesY", 6.0f);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fireAtlas);
	particles_shader.setInt("particle", 0);

	glBindVertexArray(particles_vao);
	glDrawArrays(GL_POINTS, 0, particles.size());

	glBindVertexArray(0);
}

void ParticleEmitter::emit(glm::vec3 camPos, float delta)
{
	static float accum{0.0f};
	accum += delta;
	bool emit{false};
	if(accum >= emitDelta)
	{
		emit = true;
		accum = 0.0f;
	}

	// update particles
	for(int i{0}; i < particles.size(); ++i)
	{
		particles[i].lifetime += delta;
		if(particles[i].lifetime >= maxLifetime)
		{
			particles.erase(particles.begin() + i);
			i--;
		}
		else
		{
			particles[i].position += particles[i].velocity * expf(particles[i].lifetime - maxLifetime - 0.5f) * delta;
		}
	}

	if(emit && particles.size() < MAX_PARTICLES)
	{
		float px = distribution(mt);
		float py = distribution(mt);
		float pz = distribution(mt);
		glm::vec3 particle_pos = (direction_type == ParticleEmitter::DIRECTION::VECTOR) ? position + glm::vec3(px, py, pz) : position;
		glm::vec3 particle_velocity = (direction_type == ParticleEmitter::DIRECTION::VECTOR) ? direction_vector * speed : glm::vec3(px, py, pz) * speed;
		struct Particle p(particle_pos, particle_velocity);
		particles.push_back(p);
	}

	// sort particles from the furthest to the nearest
	std::function sortParticles = [&camPos] (Particle & a, Particle & b) -> bool
	{
		glm::vec3 v1 = a.position - camPos;
		glm::vec3 v2 = b.position - camPos;
		float d1 = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
		float d2 = sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);
		return d1 < d2;
	};
	std::sort(particles.begin(), particles.end(), sortParticles);
	std::reverse(particles.begin(), particles.end());

	// update VBO
	if(particles.size() >= 3)
	{
		glBindBuffer(GL_ARRAY_BUFFER, particles_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particles.size() * sizeof(Particle), particles.data());
	}
}
