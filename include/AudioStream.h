#pragma once

#include <IAudioReader.h>
#include <IAudio.h>

#include <AL/al.h>

#include <Ref.h>

namespace Ostaga { namespace Audio {

	struct AudioStream : public IAudio
	{
		AudioStream(Scope<IAudioReader> reader, size_t bufferCount, size_t bufferSize);
		virtual ~AudioStream();

		inline virtual AudioBufferType GetBufferType() const override { return AudioBufferType::Stream; };
		void InitializeStream();

		ALuint *buffers;
		size_t bufferCount;
		size_t bufferSize;
		ALenum format = AL_FORMAT_MONO16;
	};

} }