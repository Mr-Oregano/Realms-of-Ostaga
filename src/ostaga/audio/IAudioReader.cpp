
// Precompiled
#include "ospch.h"
//

#include "IAudioReader.h"

#include <Ostaga.h>

namespace Ostaga { namespace Audio {

    IAudioReader::IAudioReader(const std::string &path)
        : m_FilePath(path)
    {
        if (!drwav_init_file(&m_FileHandle, m_FilePath.c_str(), nullptr))
            return; // Failed to open file!

        m_Loaded = true;
        m_Channels = m_FileHandle.channels;
        m_SampleRate = m_FileHandle.sampleRate;
        m_TotalFrames = m_FileHandle.totalPCMFrameCount;

        switch (m_Channels)
        {
            case 1: m_Format = AL_FORMAT_MONO16; break;
            case 2: m_Format = AL_FORMAT_STEREO16; break;
            default: 
                LOG_WARN("Unknown audio format for \"{0}\"", path); 
                m_Format = AL_FORMAT_MONO16; // Assuming mono
                break;
        }

    }

    IAudioReader::~IAudioReader()
    {
        LOG_INFO("Closing file handle for \"{0}\"", m_FilePath);
        m_Loaded = false;
        drwav_uninit(&m_FileHandle); // Close file handle
    }

    size_t IAudioReader::ReadFrames(size_t framesToRead, unsigned char *data)
    {
        return drwav_read_pcm_frames_s16(&m_FileHandle, framesToRead, (drwav_int16 *)data);
    }

    void IAudioReader::SeekToFrame(size_t frame)
    {
        drwav_seek_to_pcm_frame(&m_FileHandle, frame);
    }
} }

