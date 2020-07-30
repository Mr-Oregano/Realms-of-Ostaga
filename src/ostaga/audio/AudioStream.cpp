
// Precompiled
#include "ospch.h"
//

#include "AudioStream.h"

#include <utility>
#include <Ostaga.h>

namespace Ostaga { namespace Audio {

	AudioStream::AudioStream(size_t bufferCount, size_t bufferSize, Scope<IAudioLoader> loader)
		: bufferCount(bufferCount), bufferSize(bufferSize), loader(std::move(loader))
	{
		switch (this->loader->GetChannels())
		{
			case 1: format = AL_FORMAT_MONO16; break;
			case 2: format = AL_FORMAT_STEREO16; break;
			default: LOG_WARN("Unknown audio format for \"{0}\"", loader->GetFilePath()); break;
		}

		buffers = new ALuint[bufferCount]{0};
		AL_CALL(alGenBuffers((ALsizei) bufferCount, buffers));

		InitializeStream();
	}

	AudioStream::~AudioStream()
	{
		AL_CALL(alDeleteBuffers((ALsizei)bufferCount, buffers));
		delete[] buffers;
	}

	void AudioStream::InitializeStream()
	{
		unsigned char *data = new unsigned char[bufferSize]{0};
		for (int i = 0; i < bufferCount; ++i)
		{
			size_t framesToRead = bufferSize / loader->GetFrameSize();
			size_t framesRead = loader->ReadFrames(framesToRead, data);
			AL_CALL(alBufferData(buffers[i], format, data, (ALsizei)(framesRead * loader->GetFrameSize()), (ALsizei)loader->GetSampleRate()));
		}
		delete[] data;
	}
} }

