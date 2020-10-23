#pragma once

#include <AL/al.h>

#include <IAudioReader.h>

#include <Ref.h>

#include <string>

namespace Ostaga { namespace Audio {

	enum class AudioBufferType
	{
		Buffer = 0,
		Stream
	};

	struct IAudio
	{
	public:
		IAudio() = default;
		virtual ~IAudio() = default;
		virtual AudioBufferType GetBufferType() const = 0;

	public:
		static Ref<IAudio> LoadFromFile(const std::string &path);
	};

} }