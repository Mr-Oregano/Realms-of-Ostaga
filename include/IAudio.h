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
		IAudio(Scope<IAudioReader> reader) : reader(std::move(reader)) {}
		virtual ~IAudio() = default;

		virtual AudioBufferType GetBufferType() const = 0;

		Scope<IAudioReader> reader;

	public:
		static Ref<IAudio> LoadFromFile(const std::string &path);
	};

} }