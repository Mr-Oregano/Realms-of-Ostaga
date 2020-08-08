
// Precompiled
#include "ospch.h"
//

#include "IAudio.h"

#include <IAudioReader.h>
#include <AudioStream.h>
#include <AudioBuffer.h>

#include <Ostaga.h>
#include <Ref.h>

#include <utility>

// TODO: Consider making these values dynamic, 
//       runtime decisions
//
#define STREAM_BUFFER_COUNT 4
#define STREAM_BUFFER_SIZE (64 * 1024) // 64 kb
//

namespace Ostaga { namespace Audio {

    Ref<IAudio> IAudio::LoadFromFile(const std::string &path)
    {
        Scope<IAudioReader> reader(new IAudioReader(path));

        if (!reader)
        {
            LOG_ERROR("Failed to open file: \"{0}\"", path);
            return nullptr;
        }

        // TODO:
        size_t threshold = STREAM_BUFFER_COUNT * STREAM_BUFFER_SIZE;
        //

        if (reader->GetTotalSize() > threshold) // Stream mode will be enabled
        {
            LOG_INFO("\"{0}\" will be opened in stream mode", path);
            return std::make_shared<AudioStream>(std::move(reader), STREAM_BUFFER_COUNT, STREAM_BUFFER_SIZE);
        }

        return std::make_shared<AudioBuffer>(std::move(reader));
    }
} }
