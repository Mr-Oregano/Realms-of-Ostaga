#pragma once

#include <IAudioReader.h>
#include <IAudioPlayer.h>

#include <AudioStream.h>
#include <AL/al.h>

#include <Ref.h>
#include <atomic>

namespace Ostaga { namespace Audio {

	class AudioStreamPlayer : public IAudioPlayer
	{
	public:
		AudioStreamPlayer(const Ref<AudioStream> &stream, const AudioProps& props);
		virtual ~AudioStreamPlayer();

		virtual void Play() override;
		virtual void Stop() override;
		virtual void Pause() override;

		// Required for streaming audio data
		//
		void UpdateStream();
		void ResetStream();

	private:
		Ref<AudioStream> m_Stream;
		unsigned char *m_StreamCache;
		std::atomic_bool m_Streaming = false;
	};
} }