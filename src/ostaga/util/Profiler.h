#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <deque>

#include <thread>
#include <mutex>

#ifndef PROFILE_RESULTS_DIR
	#define PROFILE_RESULTS_DIR "."
#endif

#ifdef OSTAGA_RELEASE
	#define PROFILING 1
#else
	#define PROFILING 0
#endif

#if PROFILING
	#define PROFILER_INIT() ::Ostaga::Profiler::Init()
	#define PROFILER_SHUTDOWN() ::Ostaga::Profiler::Shutdown()

	#define PROFILE_SESSION_BEGIN(name) ::Ostaga::Profiler::BeginSession(name, name##".json")
	#define PROFILE_SESSION_END() ::Ostaga::Profiler::EndSession()

	#define PROFILE_SCOPE(name) ::Ostaga::ProfileTimer timer##__LINE__(name)
	#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
	#define PROFILER_SHUTDOWN()
	#define PROFILER_INIT()

	#define PROFILE_SESSION_BEGIN(name)
	#define PROFILE_SESSION_END()

	#define PROFILE_SCOPE(name)
	#define PROFILE_FUNCTION()
#endif

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
		static void WriteHeader();
		static void WriteFooter();

	private:
		struct Data
		{
			std::string name;
			std::string filepath;

			ProfileSession *currentSession = nullptr;
			std::ofstream outputStream;
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