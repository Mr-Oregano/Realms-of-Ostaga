
// Precompiled
#include "ospch.h"
//

#include "Audio.h"

#include <Ostaga.h>
#include <engine/AudioMaster.h>

namespace Ostaga { namespace Assets {

	Audio::Audio(ALuint buffer, const AudioProps &props)
		: m_BufferID(buffer), m_Props(props)
	{
		alGenSources(1, &m_ContextID);
		alSourcef(m_ContextID, AL_PITCH, 1);
		alSourcef(m_ContextID, AL_GAIN, 1.0f);
		alSource3f(m_ContextID, AL_POSITION, 0, 0, 0);
		alSource3f(m_ContextID, AL_VELOCITY, 0, 0, 0);
		alSourcei(m_ContextID, AL_BUFFER, buffer);
		alSourcei(m_ContextID, AL_LOOPING, props.mode == AudioMode::Loop ? AL_TRUE : AL_FALSE);
	}

	Audio::~Audio()
	{
		alDeleteSources(1, &m_ContextID);
	}

	bool Audio::IsPlaying()
	{
		ALenum state;
		alGetSourcei(m_ContextID, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}

	void Audio::SetGain(float gain)
	{
		alSourcef(m_ContextID, AL_GAIN, gain);
	}

	void Audio::SetSourcePosition(const glm::vec3 &pos)
	{
		alSource3f(m_ContextID, AL_POSITION, pos.x, pos.y, pos.z);
	}

	Ref<Audio> Audio::LoadFromFile(const std::string &path, const AudioProps &props)
	{
		drwav wav;
		if (!drwav_init_file(&wav, path.c_str(), nullptr))
		{
			LOG_ERROR("Failed to load audio file: {0}", path);
			return nullptr;
		}

		ALuint buffer = AudioMaster::GetBuffer(path);
		ALenum format = AL_FORMAT_MONO16;
		
		switch (wav.channels)
		{
			case 1: format = AL_FORMAT_MONO16; break;
			case 2: format = AL_FORMAT_STEREO16; break;
			default: LOG_WARN("Unknown audio format, using AL_FORMAT_MONO16");
		}

		drwav_int16 *data = new drwav_int16[wav.totalPCMFrameCount * wav.channels]{ 0 };
		size_t size = wav.totalPCMFrameCount * wav.channels * sizeof(drwav_uint16);
		
		drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, data);
		alBufferData(buffer, format, (ALvoid *)data, (ALsizei)size, wav.sampleRate);
		
		delete[] data;
		return CreateRef<Audio>(buffer, props);
	}
} }
