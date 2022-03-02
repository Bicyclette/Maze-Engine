#include "audio.hpp"

Audio::Audio()
{
	device = alcOpenDevice(nullptr);
	if(device == nullptr)
	{
		std::cerr << "Error: failed initializing audio device." << std::endl;
	}

	context = alcCreateContext(device, nullptr);
	if(context == nullptr)
	{
		std::cerr << "Error: failed creating audio context." << std::endl;
	}

	if(!alcMakeContextCurrent(context))
	{
		std::cerr << "Error: failed activating context." << std::endl;
	}
}

Audio::~Audio()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
	alDeleteBuffers(sounds.size(), sounds.data());
}

void Audio::load_sound(std::string file_path)
{
	SF_INFO fileInfos;
	SNDFILE* file = sf_open(file_path.c_str(), SFM_READ, &fileInfos);
	if(file == nullptr)
	{
		std::cerr << "Error: failed reading sound file." << std::endl;
	}

	ALsizei samples_count = static_cast<ALsizei>(fileInfos.channels * fileInfos.frames);
	ALsizei sample_rate = static_cast<ALsizei>(fileInfos.samplerate);
	std::vector<ALshort> samples(samples_count);

	if(sf_read_short(file, samples.data(), samples_count) < samples_count)
	{
		std::cerr << "Error: samples read from file doesn't match the actual samples count." << std::endl;
	}

	sf_close(file);

	// query file format
	ALenum format;
	if(fileInfos.channels == 1)
		format = AL_FORMAT_MONO16;
	else if(fileInfos.channels == 2)
		format = AL_FORMAT_STEREO16;
	else
		std::cerr << "Error: unknown sound file format." << std::endl;

	// create sound buffer
	ALuint sound_buffer;
	alGenBuffers(1, &sound_buffer);

	alBufferData(sound_buffer, format, samples.data(), samples_count * sizeof(ALushort), sample_rate);

	if(alGetError() != AL_NO_ERROR)
		std::cerr << "Error: failed filling sound buffer." << std::endl;

	// append it to the buffers array
	sounds.push_back(sound_buffer);
}

// ##################################################
// ##################################################
// ##################################################

Source::Source(glm::vec3 position, float volume, bool loop) :
	m_position(position),
	m_volume(volume),
	m_loop(loop)
{
	alGenSources(1, &source_id);
	set_position(m_position);
	set_volume(m_volume);
	set_looping(loop);
}

Source::~Source()
{
	alSourcei(source_id, AL_BUFFER, 0);
	alDeleteSources(1, &source_id);
}

void Source::set_volume(float volume)
{
	m_volume = volume;
	alSourcef(source_id, AL_GAIN, m_volume);
}

float Source::get_volume()
{
	return m_volume;
}

void Source::set_looping(bool loop)
{
	m_loop = loop;
	if(m_loop)
		alSourcei(source_id, AL_LOOPING, AL_TRUE);
	else
		alSourcei(source_id, AL_LOOPING, AL_FALSE);
}

bool Source::get_looping()
{
	return m_loop;
}

void Source::set_position(glm::vec3 position)
{
	m_position = position;
	alSource3f(source_id, AL_POSITION, position.x, position.y, position.z);
}

glm::vec3 Source::get_position()
{
	return m_position;
}

void Source::stop_sound()
{
	alSourceStop(source_id);
}

void Source::play_sound(ALuint sound_buffer)
{
	alSourcei(source_id, AL_BUFFER, sound_buffer);
	alSourcePlay(source_id);
}

bool Source::is_playing() const
{
	ALint status;
	alGetSourcei(source_id, AL_SOURCE_STATE, &status);
	return status == AL_PLAYING;
}

float Source::get_elapsed_time() const
{
	ALfloat elapsed_time = 0.0f;
	alGetSourcef(source_id, AL_SEC_OFFSET, &elapsed_time);
	return static_cast<float>(elapsed_time);
}

// ##################################################
// ##################################################
// ##################################################

Listener::Listener(glm::vec3 position, std::array<float, 6> orientation) :
	m_position(position),
	m_orientation(orientation)
{
	alListener3f(AL_POSITION, m_position.x, m_position.y, m_position.z);
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	ALfloat data[] = {
		m_orientation[0],
		m_orientation[1],
		m_orientation[2],
		m_orientation[3],
		m_orientation[4],
		m_orientation[5]};
	alListenerfv(AL_ORIENTATION, data);
}

glm::vec3 Listener::get_position()
{
	return m_position;
}

void Listener::set_position(glm::vec3 position)
{
	m_position = position;
	alListener3f(AL_POSITION, m_position.x, m_position.y, m_position.z);
}

std::array<float, 6> Listener::get_orientation()
{
	return m_orientation;
}

void Listener::set_orientation(std::array<float, 6> orientation)
{
	m_orientation = orientation;
	ALfloat data[] = {
		m_orientation[0],
		m_orientation[1],
		m_orientation[2],
		m_orientation[3],
		m_orientation[4],
		m_orientation[5]};
	alListenerfv(AL_ORIENTATION, data);
}

glm::vec3 Listener::get_velocity()
{
	return m_velocity;
}

void Listener::set_velocity(glm::vec3 velocity)
{
	m_velocity = velocity;
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
}
