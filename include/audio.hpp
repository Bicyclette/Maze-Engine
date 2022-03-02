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

		Source(glm::vec3 position, float volume = 1.0f, bool loop = false);
		~Source();
		void set_volume(float volume);
		float get_volume();
		void set_looping(bool loop);
		bool get_looping();
		void set_position(glm::vec3 position);
		glm::vec3 get_position();
		void stop_sound();
		void play_sound(ALuint sound_buffer);
		bool is_playing() const;
		float get_elapsed_time() const;

	private:

		ALuint source_id;
		glm::vec3 m_position;
		// for volume, 1.0f means unchanged volume
		// x2 volume means +6db
		// /2 volume means -6db
		// 0.0f means no sound
		float m_volume;
		bool m_loop;
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
