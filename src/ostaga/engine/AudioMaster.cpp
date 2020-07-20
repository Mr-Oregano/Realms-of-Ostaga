
// Precompiled
#include "ospch.h"
//

#include <Ostaga.h>

#include "AudioMaster.h"

#include <AL/alc.h>
#include <AL/al.h>

namespace Ostaga { namespace Engine {

	using namespace Assets;

	struct AudioMasterData
	{
		ALCdevice *device = nullptr;
		ALCcontext *context = nullptr;		
	};

	static AudioMasterData *audiomaster = nullptr;

	void AudioMaster::Init()
	{
		PROFILE_FUNCTION();
		audiomaster = new AudioMasterData;

		audiomaster->device = alcOpenDevice(nullptr); // Select the default audio device
		if (!audiomaster->device)
		{
			LOG_CRITICAL("Could not open default audio device");
			return;
		}

		audiomaster->context = alcCreateContext(audiomaster->device, nullptr);
		if (!audiomaster->context)
		{
			LOG_CRITICAL("Failed to create OpenAL context");
			return;
		}

		if (alcMakeContextCurrent(audiomaster->context) != ALC_TRUE)
		{
			LOG_CRITICAL("Failed to make OpenAL context current");
			return;
		}
	}
	void AudioMaster::Shudown()
	{
		alcMakeContextCurrent(audiomaster->context);
		alcDestroyContext(audiomaster->context);
		alcCloseDevice(audiomaster->device);

		delete audiomaster; audiomaster = nullptr;
	}

	void AudioMaster::Play(const Ref<Audio> &audio)
	{
		if (audio->IsPlaying())
			return;

		alSourcePlay(audio->GetContextID());
	}

	void AudioMaster::Pause(const Ref<Audio> &audio)
	{
		alSourcePause(audio->GetContextID());
	}

	void AudioMaster::Stop(const Ref<Audio> &audio)
	{
		if (!audio->IsPlaying())
			return;

		alSourceStop(audio->GetContextID());
	}
	
	void AudioMaster::Update()
	{
		PROFILE_FUNCTION();
	}
} }