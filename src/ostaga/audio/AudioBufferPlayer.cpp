
// Precompiled
#include "ospch.h"
//

#include "AudioBufferPlayer.h"

#include <Ostaga.h>

namespace Ostaga { namespace Audio {

	std::unordered_map<std::string, ALuint> AudioBufferPlayer::s_Buffers;

	AudioBufferPlayer::AudioBufferPlayer(const AudioProps &props, IAudioLoader &loader)
		: IAudioPlayer(props)
	{
		auto it = s_Buffers.find(loader.GetFilePath());
		if (it == s_Buffers.end())
		{
			ALenum format = AL_FORMAT_MONO16;
			switch (loader.GetChannels())
			{
				case 1: format = AL_FORMAT_MONO16; break;
				case 2: format = AL_FORMAT_STEREO16; break;
				default: LOG_WARN("Unknown audio format for \"{0}\"", loader.GetFilePath());
			}

			unsigned char *data = new unsigned char[loader.GetTotalSize()]{ 0 };
			loader.ReadFrames(loader.GetTotalFrames(), data);

			AL_CALL(alGenBuffers(1, &m_BufferID));
			AL_CALL(alBufferData(m_BufferID, format, (ALvoid *)data, (ALsizei)loader.GetTotalSize(), loader.GetSampleRate()));
			s_Buffers.insert({ loader.GetFilePath(), m_BufferID });
			delete[] data;
		}
		else m_BufferID = it->second;

		AL_CALL(alGenSources(1, &m_SourceID));
		AL_CALL(alSourcei(m_SourceID, AL_BUFFER, m_BufferID));
	}

	AudioBufferPlayer::~AudioBufferPlayer()
	{
		Stop(); // Stop the audio if still playing

		AL_CALL(alSourcei(m_SourceID, AL_BUFFER, 0));
		AL_CALL(alDeleteSources(1, &m_SourceID));
	}

	void AudioBufferPlayer::Play()
	{
		switch (PollState())
		{
		case AudioState::Paused:
		case AudioState::Stopped:
			AL_CALL(alSourcei(m_SourceID, AL_LOOPING, m_Props.mode == AudioMode::Loop ? AL_TRUE : AL_FALSE));
			AL_CALL(alSourcePlay(m_SourceID));
		}
	}

	void AudioBufferPlayer::Stop()
	{
		switch (PollState())
		{
		case AudioState::Playing:
		case AudioState::Paused:
			AL_CALL(alSourceStop(m_SourceID));
		}
	}

	void AudioBufferPlayer::Pause()
	{
		switch (PollState())
		{
		case AudioState::Playing:
			AL_CALL(alSourcePause(m_SourceID));
		}
	}
} }

