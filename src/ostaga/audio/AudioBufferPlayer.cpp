
// Precompiled
#include "ospch.h"
//

#include "AudioBufferPlayer.h"

#include <Ostaga.h>

namespace Ostaga { namespace Audio {

	AudioBufferPlayer::AudioBufferPlayer(const Ref<AudioBuffer> &buffer, const AudioProps &props)
		: IAudioPlayer(props), m_Buffer(buffer)
	{
		AL_CALL(alGenSources(1, &m_SourceID));
		AL_CALL(alSourcei(m_SourceID, AL_BUFFER, m_Buffer->bufferID));
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

