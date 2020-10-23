
// Precompiled
#include "ospch.h"
//

#include "AudioBuffer.h"

#include <utility>
#include <Ostaga.h>

namespace Ostaga { namespace Audio {

	// The AudioBuffer should not need to store/maintain a file handle
	// to the audio file. Once the data buffer has been read, the file
	// handle can be released.
	//
	AudioBuffer::AudioBuffer(Scope<IAudioReader> reader)
	{
		unsigned char *data = new unsigned char[reader->GetTotalSize()]{ 0 };
		reader->ReadFrames(reader->GetTotalFrames(), data);
		OSTAGA_IF_DEBUG(name = reader->GetFilePath();) // Debug only

		AL_CALL(alGenBuffers(1, &bufferID));
		AL_CALL(alBufferData(bufferID, reader->GetFormat(), (ALvoid *)data, (ALsizei) reader->GetTotalSize(), reader->GetSampleRate()));
		delete[] data;
	}

	AudioBuffer::~AudioBuffer()
	{
		LOG_INFO("Destroying AudioBuffer for \"{0}\"", name);
		AL_CALL(alDeleteBuffers(1, &bufferID));
	}
} }


