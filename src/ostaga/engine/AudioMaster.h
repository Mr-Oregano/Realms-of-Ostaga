#pragma once

#include <memory>

#include <AL/al.h>

#include <assets/audio/Audio.h>
#include <util/Ref.h>

namespace Ostaga { namespace Assets {

    class AudioMaster
    {
    public:
        static void Init();
        static void Shudown();

        static void Play(const Ref<Audio>& audio);
        static void Pause(const Ref<Audio> &audio);
        static void Stop(const Ref<Audio> &audio);

        static void Update();
    };
} }