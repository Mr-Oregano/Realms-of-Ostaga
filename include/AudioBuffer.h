#pragma once

#include <IAudioReader.h>
#include <IAudio.h>

#include <AL/al.h>

#include <Ref.h>

namespace Ostaga { namespace Audio {

	struct AudioBuffer : public IAudio
	{
		AudioBuffer(Scope<IAudioReader> loader);
		virtual ~AudioBuffer();

		inline virtual AudioBufferType GetBufferType() const override { return AudioBufferType::Buffer; };

		ALuint bufferID;
	};
} }