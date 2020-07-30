#pragma once

#include <audio/AudioStreamPlayer.h>

#include <AL/al.h>

namespace Ostaga { namespace Audio {

	class AudioDevice
	{
	public:
		static void Init();
		static void Shutdown();

		static void SetGain(float gain);

		static void RegisterStream(AudioStreamPlayer *streamer);
		static void UnregisterStream(AudioStreamPlayer *streamer);
		static void UpdateStreams();
	};
} }