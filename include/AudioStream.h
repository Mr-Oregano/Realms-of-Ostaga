#pragma once

#include <IAudioReader.h>
#include <IAudio.h>

#include <AL/al.h>

#include <Ref.h>

namespace Ostaga { namespace Audio {

	struct AudioStream : public IAudio
	{
		AudioStream(Scope<IAudioReader> reader, size_t bufferCount, size_t bufferSize)
			: reader(std::move(reader)), bufferCount(bufferCount), bufferSize(bufferSize), format(this->reader->GetFormat()) {}

		virtual ~AudioStream() = default;

		inline virtual AudioBufferType GetBufferType() const override { return AudioBufferType::Stream; };

		Scope<IAudioReader> reader;
		size_t bufferCount;
		size_t bufferSize;
		ALenum format = AL_FORMAT_MONO16;
	};

} }