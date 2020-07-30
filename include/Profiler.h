#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <deque>

#include <thread>
#include <mutex>

namespace Ostaga {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult
	{
		std::string name;

		FloatingPointMicroseconds start;
		std::chrono::microseconds elapsedTime;

		std::thread::id threadID;
	};
	struct ProfileSession
	{
		std::string name;
	};

	class Profiler
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginSession(const std::string &name, const std::string &filepath);
		static void EndSession();
		static void WriteProfile(const ProfileResult &result);

	private:
		struct Data
		{
			std::ofstream outputStream;
			ProfileSession *currentSession = nullptr;
		};

		static Data *s_Data;

	};

	class ProfileTimer
	{
	public:
		ProfileTimer(const std::string &name);
		~ProfileTimer();

		void Stop();

	private:
		std::string m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

}