#pragma once

#include <IAudioLoader.h>

#include <AL/al.h>

#include <Ref.h>

namespace Ostaga { namespace Audio {

	struct AudioStream
	{
		AudioStream(size_t bufferCount, size_t bufferSize, Scope<IAudioLoader> loader);
		~AudioStream();

		void InitializeStream();

		ALuint *buffers;
		size_t bufferCount;
		size_t bufferSize;
		ALenum format = AL_FORMAT_MONO16;
		Scope<IAudioLoader> loader;
	};
} }