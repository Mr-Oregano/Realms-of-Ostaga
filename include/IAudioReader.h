#pragma once

#include <string>

// TODO: Create generic interface for file loader
//		 add support for different file formats
#include <dr_wav.h>
//

#include <AL/al.h>

namespace Ostaga { namespace Audio {

	class IAudioReader
	{
	public:
		IAudioReader(const std::string &path);
		~IAudioReader();

		inline const std::string &GetFilePath() const { return m_FilePath; }

		inline int GetChannels() const { return m_Channels; }
		inline int GetSampleRate() const { return m_SampleRate; }

		inline size_t GetTotalFrames() const { return m_TotalFrames; }
		inline size_t GetTotalSize() const { return m_TotalFrames * GetFrameSize(); }
		inline size_t GetFrameSize() const { return m_Channels * sizeof(drwav_int16); }
		inline ALenum GetFormat() const { return m_Format; }

		inline operator bool() { return m_Loaded; }

		// Returns the total frames read
		//
		size_t ReadFrames(size_t framesToRead, unsigned char *data);
		void SeekToFrame(size_t frame);

	private:
		std::string m_FilePath;
		drwav m_FileHandle; // Only supporting wav files for now

		int m_Channels = 0;
		int m_SampleRate = 0;
		size_t m_TotalFrames = 0;
		ALenum m_Format;

		bool m_Loaded = false;
	};
} }
