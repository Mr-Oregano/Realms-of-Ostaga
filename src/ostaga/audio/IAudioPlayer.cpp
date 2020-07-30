
// Precompiled
#include "ospch.h"
//

#include "IAudioPlayer.h"

#include <audio/IAudioLoader.h>
#include <audio/AudioStreamPlayer.h>
#include <audio/AudioBufferPlayer.h>

#include <Ostaga.h>
#include <util/Ref.h>

namespace Ostaga { namespace Audio {

    AudioState IAudioPlayer::PollState() const
    {
        ALenum state;
        AL_CALL(alGetSourcei(m_SourceID, AL_SOURCE_STATE, &state));

        switch (state)
        {
            case AL_PLAYING: return AudioState::Playing;
            case AL_PAUSED: return AudioState::Paused;
            case AL_STOPPED: return AudioState::Stopped;
        }

        return AudioState::Stopped;
    }

    Ref<IAudioPlayer> IAudioPlayer::LoadFromFile(const std::string &path, const AudioProps &props)
    {
        Scope<IAudioLoader> loader(new IAudioLoader(path));

        if (!loader)
        {
            LOG_ERROR("Failed to open file: \"{0}\"", path);
            return nullptr;
        }

        size_t threshold = props.streamBufferCount * props.streamBufferSize;
        if (loader->GetTotalSize() > threshold) // Stream mode will be enabled
        {
            LOG_INFO("\"{0}\" will be opened in stream mode", path);
            return std::make_shared<AudioStreamPlayer>(props, std::move(loader));
        }

        return std::make_shared<AudioBufferPlayer>(props, *loader);
    }
} }
