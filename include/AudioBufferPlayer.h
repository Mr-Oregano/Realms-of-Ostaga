#pragma once

#include <IAudioReader.h>
#include <IAudioPlayer.h>

#include <AudioBuffer.h>
#include <AL/al.h>

#include <Ref.h>

#include <unordered_map>
#include <string>

namespace Ostaga { namespace Audio {

	class AudioBufferPlayer : public IAudioPlayer
	{
	public:
		AudioBufferPlayer(const Ref<AudioBuffer> &buffer, const AudioProps &props);
		virtual ~AudioBufferPlayer();

		virtual void Play() override;
		virtual void Stop() override;
		virtual void Pause() override;

	private:
		Ref<AudioBuffer> m_Buffer;
	};
} }