
// Precompiled
#include "ospch.h"
//

#include "AudioStream.h"

#include <utility>
#include <Ostaga.h>

namespace Ostaga { namespace Audio {

	AudioStream::AudioStream(Scope<IAudioReader> reader, size_t bufferCount, size_t bufferSize)
		: IAudio(std::move(reader)), bufferCount(bufferCount), bufferSize(bufferSize), format(this->reader->GetFormat())
	{
		buffers = new ALuint[bufferCount]{0};
		AL_CALL(alGenBuffers((ALsizei) bufferCount, buffers));

		InitializeStream();
	}

	AudioStream::~AudioStream()
	{
		LOG_INFO("Destroying Audiostream for \"{0}\"", reader->GetFilePath());
		AL_CALL(alDeleteBuffers((ALsizei)bufferCount, buffers));
		delete[] buffers;
	}

	void AudioStream::InitializeStream()
	{
		unsigned char *data = new unsigned char[bufferSize]{0};
		for (int i = 0; i < bufferCount; ++i)
		{
			size_t framesToRead = bufferSize / reader->GetFrameSize();
			size_t framesRead = reader->ReadFrames(framesToRead, data);
			AL_CALL(alBufferData(buffers[i], format, data, (ALsizei)(framesRead * reader->GetFrameSize()), (ALsizei)reader->GetSampleRate()));
		}
		delete[] data;
	}
} }

