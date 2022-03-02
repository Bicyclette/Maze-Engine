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

Source::Source(glm::vec3 position, glm::vec3 direction, float inner_angle, float outer_angle, float volume, bool loop) :
	m_model(glm::mat4(1.0f)),
	m_position(position),
	m_direction(direction),
	m_inner_angle(inner_angle),
	m_outer_angle(outer_angle),
	m_volume(volume),
	m_loop(loop),
	shaderIcon("shaders/light/spot/vertex.glsl", "shaders/light/spot/geometry.glsl", "shaders/light/spot/fragment.glsl"),
	shaderSoundArea("shaders/light/spot/vertex_cutoff.glsl", "shaders/light/spot/geometry_cutoff.glsl", "shaders/light/spot/fragment_cutoff.glsl")
{
	alGenSources(1, &source_id);
	set_position(m_position);
	set_direction(m_direction);
	set_direction_inner_angle(m_inner_angle);
	set_direction_outer_angle(m_outer_angle);
	set_volume(m_volume);
	set_looping(loop);
	
	// draw
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float data[3] = {m_position.x, m_position.y, m_position.z};
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);

	shaderIcon.use();
	shaderIcon.setInt("icon", 0);
	shaderSoundArea.use();
	shaderSoundArea.setVec3f("direction", m_direction);
	shaderSoundArea.setFloat("cutOff", m_inner_angle);
	
	icon_on = createTexture("assets/audio_icons/source_on.png", TEXTURE_TYPE::DIFFUSE, true);
	icon_off = createTexture("assets/audio_icons/source_off.png", TEXTURE_TYPE::DIFFUSE, true);
}

void Source::setModelMatrix(glm::mat4 model)
{
	m_model = model;
	m_position = glm::vec3(m_model * glm::vec4(m_position, 1.0f));
}

void Source::setViewMatrix(glm::mat4 view)
{
	m_view = view;
}

void Source::setProjMatrix(glm::mat4 proj)
{
	m_proj = proj;
}

void Source::draw()
{
	glBindVertexArray(vao);
	shaderIcon.use();
	shaderIcon.setMatrix("model", m_model);
	shaderIcon.setMatrix("view", m_view);
	shaderIcon.setMatrix("proj", m_proj);
	glActiveTexture(GL_TEXTURE0);
	if(is_playing())
		glBindTexture(GL_TEXTURE_2D, icon_on.id);
	else
		glBindTexture(GL_TEXTURE_2D, icon_off.id);
	glDrawArrays(GL_POINTS, 0, 1);

	if(m_direction != glm::vec3(0.0f))
	{
		shaderSoundArea.use();
		shaderSoundArea.setMatrix("model", m_model);
		shaderSoundArea.setMatrix("view", m_view);
		shaderSoundArea.setMatrix("proj", m_proj);
		shaderSoundArea.setVec3f("direction", glm::normalize(m_direction));
		shaderSoundArea.setVec3f("right", glm::normalize(glm::cross(m_direction, glm::vec3(0.0f, 1.0f, 0.0f))));
		shaderSoundArea.setFloat("cutOff", m_inner_angle);
		glDrawArrays(GL_POINTS, 0, 1);
	}
	glBindVertexArray(0);
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

void Source::set_direction(glm::vec3 direction)
{
	m_direction = direction;
	alSource3f(source_id, AL_DIRECTION, direction.x, direction.y, direction.z);
}

void Source::set_direction_inner_angle(float inner_angle)
{
	m_inner_angle = inner_angle;
	alSourcef(source_id, AL_CONE_INNER_ANGLE, m_inner_angle);
}

float Source::get_direction_inner_angle()
{
	return m_inner_angle;
}

void Source::set_direction_outer_angle(float outer_angle)
{
	m_outer_angle = outer_angle;
	alSourcef(source_id, AL_CONE_OUTER_ANGLE, m_outer_angle);
}

float Source::get_direction_outer_angle()
{
	return m_outer_angle;
}

glm::vec3 Source::get_direction()
{
	return m_direction;
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
