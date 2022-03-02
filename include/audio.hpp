#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <memory>
#include <utility>
#include <al.h>
#include <alc.h>
#include <sndfile.h>
#include <glm/glm.hpp>
#include "shader_light.hpp"

class Audio
{
	public:

		Audio();
		~Audio();
		void load_sound(std::string file_path);
		std::vector<ALuint> sounds;

	private:

		ALCdevice * device;
		ALCcontext * context;
};

class Source
{
	public:

		Source(glm::vec3 position, glm::vec3 direction = glm::vec3(0.0f), float inner_angle = 0.0f, float outer_angle = 0.0f,  float volume = 1.0f, bool loop = false);
		~Source();
		void set_volume(float volume);
		float get_volume();
		void set_looping(bool loop);
		bool get_looping();
		void set_position(glm::vec3 position);
		glm::vec3 get_position();
		void set_direction(glm::vec3 direction);
		glm::vec3 get_direction();
		void set_direction_inner_angle(float inner_angle);
		float get_direction_inner_angle();
		void set_direction_outer_angle(float outer_angle);
		float get_direction_outer_angle();
		void stop_sound();
		void play_sound(ALuint sound_buffer);
		bool is_playing() const;
		float get_elapsed_time() const;
		void setModelMatrix(glm::mat4 model);
		void setViewMatrix(glm::mat4 view);
		void setProjMatrix(glm::mat4 proj);
		void draw();

	private:

		ALuint source_id;
		glm::vec3 m_position;
		// for volume, 1.0f means unchanged volume
		// x2 volume means +6db
		// /2 volume means -6db
		// 0.0f means no sound
		glm::vec3 m_direction;
		float m_inner_angle;
		float m_outer_angle;
		float m_volume;
		bool m_loop;

		GLuint vao;
		GLuint vbo;

		glm::mat4 m_model;
		glm::mat4 m_view;
		glm::mat4 m_proj;

		struct Texture icon_on;
		struct Texture icon_off;
		Shader shaderIcon;
		Shader shaderSoundArea;
};

class Listener
{
	public:

		Listener(glm::vec3 position, std::array<float, 6> orientation);
		glm::vec3 get_position();
		void set_position(glm::vec3 position);
		std::array<float, 6> get_orientation();
		void set_orientation(std::array<float, 6> orientation);
		glm::vec3 get_velocity();
		void set_velocity(glm::vec3 velocity);

	private:

		glm::vec3 m_position;
		// 3 floats for at vector, and 3 others for up vector
		std::array<float, 6> m_orientation;
		glm::vec3 m_velocity;
};

#endif
