
// Precompiled
#include "ospch.h"
//

#include "AudioStreamPlayer.h"
#include <AudioDevice.h>

#include <utility>

#include <Ostaga.h>

namespace Ostaga { namespace Audio {

	AudioStreamPlayer::AudioStreamPlayer(const Ref<AudioStream> &stream, const AudioProps &props)
		: IAudioPlayer(props), m_Stream(stream)
	{
		buffers = new ALuint[m_Stream->bufferCount]{ 0 };
		AL_CALL(alGenBuffers((ALsizei) m_Stream->bufferCount, buffers));
		InitializeStream();

		m_StreamCache = new unsigned char[m_Stream->bufferSize];
		AL_CALL(alGenSources(1, &m_SourceID));
		AL_CALL(alSourceQueueBuffers(m_SourceID, (ALsizei)m_Stream->bufferCount, buffers));
	}

	AudioStreamPlayer::~AudioStreamPlayer()
	{
		Stop(); // Stop the audio if still playing.
			    // Stop() will stall until the stream has been unregistered
			    // Only then is it safe to free resources

		ALint buffersProcessed = 0;
		AL_CALL(alGetSourcei(m_SourceID, AL_BUFFERS_PROCESSED, &buffersProcessed));
		AL_CALL(alSourceUnqueueBuffers(m_SourceID, (ALsizei)buffersProcessed, buffers));

		AL_CALL(alDeleteSources(1, &m_SourceID));
		delete[] m_StreamCache;

		LOG_INFO("Destroying Audiostream for \"{0}\"", m_Stream->reader->GetFilePath());
		AL_CALL(alDeleteBuffers((ALsizei) m_Stream->bufferCount, buffers));
		delete[] buffers;
	}

	void AudioStreamPlayer::Play()
	{
		switch (PollState())
		{
		case AudioState::Stopped:
			AudioDevice::RegisterStream(this); // Will wait until the stream can be registered
			[[fallthrough]]; // Yes, msvc, I want this to fall through
		case AudioState::Paused:
			m_Streaming = true;
			AL_CALL(alSourcei(m_SourceID, AL_LOOPING, AL_FALSE)); // Looping is handled by the stream
			AL_CALL(alSourcePlay(m_SourceID));
			break;
		}
	}

	void AudioStreamPlayer::Stop()
	{
		switch (PollState())
		{
		case AudioState::Playing:
		case AudioState::Paused:
			m_Streaming = false;
			AL_CALL(alSourceStop(m_SourceID));
			AudioDevice::UnregisterStream(this); // Will wait until the stream can be unregistered
			ResetStream(); // It is safe to do this as stream is no longer being updated
		}
	}

	void AudioStreamPlayer::Pause()
	{
		switch (PollState())
		{
		case AudioState::Playing:
			m_Streaming = false;
			AL_CALL(alSourcePause(m_SourceID));
		}
	}

	void AudioStreamPlayer::UpdateStream()
	{
		if (!m_Streaming)
			return;

		// If the queue was starved of buffers then
		// restart the audio and move forward by one buffer.
		if (IsStopped())
		{
			// TODO: Sometimes the audio is not restarted, investigate why.
			AL_CALL(alSourcei(m_SourceID, AL_BYTE_OFFSET, (ALint)m_Stream->bufferSize));
			AL_CALL(alSourcePlay(m_SourceID));
		}
		//

		ALint buffersProcessed = 0;
		AL_CALL(alGetSourcei(m_SourceID, AL_BUFFERS_PROCESSED, &buffersProcessed));
		for (int i = 0; i < buffersProcessed; ++i)
		{
			IAudioReader &reader = *m_Stream->reader;
			size_t framesToRead = m_Stream->bufferSize / reader.GetFrameSize();
			size_t framesRead = reader.ReadFrames(m_CurrentFrame, framesToRead, m_StreamCache);
			
			m_CurrentFrame = (m_CurrentFrame + framesRead) % reader.GetTotalFrames();

			ALuint buffer = 0;
			AL_CALL(alSourceUnqueueBuffers(m_SourceID, 1, &buffer));
			AL_CALL(alBufferData(buffer, m_Stream->format, m_StreamCache, (ALsizei)(framesRead * reader.GetFrameSize()), (ALsizei)reader.GetSampleRate()));
			AL_CALL(alSourceQueueBuffers(m_SourceID, 1, &buffer));

			if (framesRead < framesToRead)
				m_Streaming = m_Props.mode == AudioMode::Loop;
		}
	}

	// Sharing data with UpdateStream(). This method should ONLY be invoked when
	// it is certain that UpdateStream() is NOT running.
	//
	void AudioStreamPlayer::ResetStream()
	{
		ALint buffersProcessed = 0;
		AL_CALL(alGetSourcei(m_SourceID, AL_BUFFERS_PROCESSED, &buffersProcessed));
		AL_CALL(alSourceUnqueueBuffers(m_SourceID, (ALsizei)buffersProcessed, buffers));

		m_CurrentFrame = 0;
		InitializeStream();

		AL_CALL(alSourceQueueBuffers(m_SourceID, (ALsizei) m_Stream->bufferCount, buffers));
	}

	void AudioStreamPlayer::InitializeStream()
	{
		IAudioReader &reader = *m_Stream->reader;
		unsigned char *data = new unsigned char[m_Stream->bufferSize] {0};
		for (int i = 0; i < m_Stream->bufferCount; ++i)
		{
			size_t framesToRead = m_Stream->bufferSize / m_Stream->reader->GetFrameSize();
			size_t framesRead = m_Stream->reader->ReadFrames(m_CurrentFrame, framesToRead, data);
			AL_CALL(alBufferData(buffers[i], m_Stream->format, data, (ALsizei)(framesRead * reader.GetFrameSize()), (ALsizei) reader.GetSampleRate()));
			m_CurrentFrame = (m_CurrentFrame + framesRead) % reader.GetTotalFrames();
		}
		delete[] data;
	}

} }


