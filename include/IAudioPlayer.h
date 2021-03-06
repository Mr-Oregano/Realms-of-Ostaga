#pragma once

#include <AL/al.h>
#include <string>

#include <IAudio.h>

#include <Ref.h>
#include <Maths.h>

namespace Ostaga { namespace Audio {

	enum class AudioMode
	{
		Normal = 0,
		Loop
	};
	enum class AudioState
	{
		Playing = 0,
		Paused,
		Stopped
	};
	struct AudioProps
	{
		AudioMode mode;
	};

	class IAudioPlayer
	{
	public:
		IAudioPlayer(const AudioProps &props) : m_Props(props) {}
		virtual ~IAudioPlayer() = default;

		virtual void Play() = 0;
		virtual void Stop() = 0;
		virtual void Pause() = 0;

		virtual void SetGain(float gain) { alSourcef(m_SourceID, AL_GAIN, gain); }
		virtual void SetPosition(float x, float y, float z) { alSource3f(m_SourceID, AL_POSITION, x, y, z); }
		virtual void SetPosition(const glm::vec3 pos) { alSourcefv(m_SourceID, AL_POSITION, glm::value_ptr(pos)); }
		virtual void SetPitch(float pitch) { alSourcef(m_SourceID, AL_PITCH, pitch); }

		virtual AudioState PollState() const;
		virtual bool IsPlaying() const { return PollState() == AudioState::Playing; }
		virtual bool IsStopped() const { return PollState() == AudioState::Stopped; }

	protected:
		AudioProps m_Props;
		ALuint m_SourceID = 0;

	public:
		static Ref<IAudioPlayer> Create(const Ref<IAudio> audio, const AudioProps& props = { AudioMode::Normal });

	};
} }
