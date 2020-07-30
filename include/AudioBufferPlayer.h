#pragma once

#include <IAudioLoader.h>
#include <IAudioPlayer.h>

#include <AL/al.h>

#include <unordered_map>
#include <string>

namespace Ostaga { namespace Audio {

	class AudioBufferPlayer : public IAudioPlayer
	{
	public:
		AudioBufferPlayer(const AudioProps &props, IAudioLoader &loader);
		virtual ~AudioBufferPlayer();

		virtual void Play() override;
		virtual void Stop() override;
		virtual void Pause() override;

	private:
		ALuint m_BufferID;

	private:
		static std::unordered_map<std::string, ALuint> s_Buffers;
	};
} }