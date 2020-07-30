
// Precompiled
#include "ospch.h"
//

#include "AudioDevice.h"

#include <AL/alc.h>
#include <AL/al.h>

#include <vector>
#include <algorithm>
#include <thread>
#include <atomic>
#include <mutex>

#include <Ostaga.h>

namespace Ostaga { namespace Audio {
	
	struct AudioDeviceData
	{
		ALCdevice *device = nullptr;
		ALCcontext *context = nullptr;

		// Shared data:
		//  - audio->streams
		//  - audio->running
		//  - AudioStreamPlayer::streaming
		//
		std::thread runner;
		std::atomic_bool running;
		std::mutex mutex;

		std::vector<AudioStreamPlayer *> streams;
	};

	static AudioDeviceData *audio = nullptr;

	void AudioDevice::Init()
	{
		audio = new AudioDeviceData;

		audio->device = alcOpenDevice(nullptr); // Select the default device
		if (!audio->device)
		{
			LOG_ERROR("Could not load the default audio device!");
			return;
		}

		audio->context = alcCreateContext(audio->device, nullptr);
		if (!audio->context)
		{
			LOG_ERROR("Could not create audio context!");
			return;
		}

		if (alcMakeContextCurrent(audio->context) != ALC_TRUE)
		{
			LOG_ERROR("Could not make the audio context current!");
			return;
		}

		audio->running = true;
		audio->runner = std::thread([]()
		{
			while (audio->running)
			{
				// TODO: test speed on different devices, may need to be determined at runtime
				//
				std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Harsh implementation of thread priority
				UpdateStreams();
			}
		});
	}

	void AudioDevice::Shutdown()
	{
		audio->running = false;
		audio->runner.join();

		alcMakeContextCurrent(nullptr);
		alcDestroyContext(audio->context);
		alcCloseDevice(audio->device);

		delete audio; audio = nullptr;
	}

	void AudioDevice::SetGain(float gain)
	{
		AL_CALL(alListenerf(AL_GAIN, gain));
	}

	// TODO: Not really the best to stall the main thread when unregistering/registering streams
	//		 will need to figure out how to deal with resource deallocation
	//
	// When called, method will lock until the stream can be removed
	//      the stream should not be removed until it is certain that
	//      that it has finished its last update.
	//
	void AudioDevice::RegisterStream(AudioStreamPlayer *streamer)
	{
		std::lock_guard lock{ audio->mutex };
		audio->streams.push_back(streamer);
	}

	void AudioDevice::UnregisterStream(AudioStreamPlayer *streamer)
	{
		std::lock_guard lock{ audio->mutex };
		auto &streams = audio->streams;
		streams.erase(std::remove(streams.begin(), streams.end(), streamer), streams.end());
	}

	void AudioDevice::UpdateStreams()
	{
		std::lock_guard lock{ audio->mutex }; // stream list will not be updated by other threads
											  // while it is being updated.
		auto &streams = audio->streams;
		std::for_each(streams.begin(), streams.end(), [&](AudioStreamPlayer *stream)
		{
			stream->UpdateStream();

			if (stream->IsStopped())
				streams.erase(std::remove(streams.begin(), streams.end(), stream), streams.end());
		});
	}
} }

