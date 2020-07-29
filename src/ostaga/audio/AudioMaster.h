#pragma once

#include <memory>

#include <AL/al.h>

#include <audio/Audio.h>
#include <util/Ref.h>

namespace Ostaga { namespace Audio {

    class AudioMaster
    {
    public:
        static void Init();
        static void Shudown();

        static void Play(const Ref<AudioSource>& audio);
        static void Pause(const Ref<AudioSource> &audio);
        static void Stop(const Ref<AudioSource> &audio);

        static void Update();
    };
} }