#pragma once

#include <AL/al.h>
#include <dr_wav.h>

#include <util/Maths.h>
#include <util/Ref.h>

namespace Ostaga { namespace Audio {

	enum class AudioMode
	{
		Normal = 0,
		Loop
	};
	struct AudioProps
	{
		AudioMode mode;
	};

	class AudioSource
	{
	public:
		AudioSource(ALuint buffer, const AudioProps& props);
		~AudioSource();

		inline ALuint GetContextID() { return m_ContextID; }

		bool IsPlaying();
		void SetGain(float gain);
		void SetSourcePosition(const glm::vec3 &pos);

	private:
		ALuint m_BufferID;
		ALuint m_ContextID;

		AudioProps m_Props;

	public:
		static Ref<AudioSource> LoadFromFile(const std::string &path, const AudioProps &props = { AudioMode::Normal });
		static std::unordered_map<std::string, ALuint> s_Buffers;

	};
} }