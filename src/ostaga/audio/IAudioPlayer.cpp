
// Precompiled
#include "ospch.h"
//

#include "IAudioPlayer.h"

#include <IAudio.h>
#include <IAudioReader.h>
#include <AudioStreamPlayer.h>
#include <AudioBufferPlayer.h>

#include <Ostaga.h>
#include <Ref.h>

namespace Ostaga { namespace Audio {

    AudioState IAudioPlayer::PollState() const
    {
        ALenum state;
        AL_CALL(alGetSourcei(m_SourceID, AL_SOURCE_STATE, &state));

        switch (state)
        {
            case AL_PLAYING: return AudioState::Playing;
            case AL_PAUSED:  return AudioState::Paused;
            default:         return AudioState::Stopped;
        }
    }

    Ref<IAudioPlayer> IAudioPlayer::Create(const Ref<IAudio> audio, const AudioProps &props)
    {
        switch (audio->GetBufferType())
        {
            case AudioBufferType::Stream:
                return CreateRef<AudioStreamPlayer>(std::dynamic_pointer_cast<AudioStream>(audio), props);

            default: 
                return CreateRef<AudioBufferPlayer>(std::dynamic_pointer_cast<AudioBuffer>(audio), props);
        }
    }
} }
