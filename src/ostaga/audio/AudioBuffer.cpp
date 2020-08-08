
// Precompiled
#include "ospch.h"
//

#include "AudioBuffer.h"

#include <utility>
#include <Ostaga.h>

namespace Ostaga { namespace Audio {

	AudioBuffer::AudioBuffer(Scope<IAudioReader> reader)
		: IAudio(std::move(reader))
	{
		unsigned char *data = new unsigned char[this->reader->GetTotalSize()]{ 0 };
		this->reader->ReadFrames(this->reader->GetTotalFrames(), data);

		AL_CALL(alGenBuffers(1, &bufferID));
		AL_CALL(alBufferData(bufferID, this->reader->GetFormat(), (ALvoid *)data, (ALsizei) this->reader->GetTotalSize(), this->reader->GetSampleRate()));
		delete[] data;
	}

	AudioBuffer::~AudioBuffer()
	{
		LOG_INFO("Destroying AudioBuffer for \"{0}\"", reader->GetFilePath());
		AL_CALL(alDeleteBuffers(1, &bufferID));
	}
} }


