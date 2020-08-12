#pragma once

#define OSTAGA_OPENGL_VERSION_MAJOR 4
#define OSTAGA_OPENGL_VERSION_MINOR 6

#if defined(OSTAGA_DEBUG) || defined(OSTAGA_RELEASE)
	#define OSTAGA_IF_DEBUG(x, ...) x

	// The logger will only be included in the program for debug builds
	#include <Profiler.h>
	#include <Logger.h>

	#define LOG_INIT() ::Ostaga::Logger::Init()
	#define LOG_SHUTDOWN() ::Ostaga::Logger::Shutdown()

	#define LOG_LINE() ::Ostaga::Logger::Get()->trace("")

	#define	   LOG_TRACE(x, ...) ::Ostaga::Logger::Get()->trace(x, __VA_ARGS__)
	#define		LOG_INFO(x, ...) ::Ostaga::Logger::Get()->info(x, __VA_ARGS__)
	#define		LOG_WARN(x, ...) ::Ostaga::Logger::Get()->warn(x, __VA_ARGS__)
	#define	   LOG_ERROR(x, ...) ::Ostaga::Logger::Get()->error(x, __VA_ARGS__)
	#define LOG_CRITICAL(x, ...) ::Ostaga::Logger::Get()->critical(x, __VA_ARGS__)

	#define    ASSERT_TRACE(x, y, ...) if (!(x)) { ::Ostaga::Logger::Get()->trace(y, __VA_ARGS__); }
	#define		ASSERT_INFO(x, y, ...) if (!(x)) { ::Ostaga::Logger::Get()->info(y, __VA_ARGS__); }
	#define		ASSERT_WARN(x, y, ...) if (!(x)) { ::Ostaga::Logger::Get()->warn(y, __VA_ARGS__); }
	#define    ASSERT_ERROR(x, y, ...) if (!(x)) { ::Ostaga::Logger::Get()->error(y, __VA_ARGS__); }
	#define ASSERT_CRITICAL(x, y, ...) if (!(x)) { ::Ostaga::Logger::Get()->critical(y, __VA_ARGS__); exit(-1); }

	#ifdef OSTAGA_BUILD_WINDOWS
		#define OSTAGA_DEBUG_BREAK() __debugbreak()
	#else
		#define OSTAGA_DEBUG_BREAK() // TODO: figure out debug break for other platforms
	#endif

	#ifdef OSTAGA_RELEASE
		#define PROFILING 1
	#else
		#define PROFILING 0
	#endif

	#define AL_CALL(func) \
		do { func; ALenum error = alGetError(); if (error != AL_NO_ERROR) { LOG_ERROR("OpenAL error {0} @ {1}:{2}", error, __FILE__, __LINE__); } } while(false)
	
#else
	#define OSTAGA_IF_DEBUG(x, ...) __VA_ARGS__

	#define LOG_INIT()
	#define LOG_SHUTDOWN()

	#define LOG_LINE()

	#define	   LOG_TRACE(x, ...)
	#define		LOG_INFO(x, ...)
	#define		LOG_WARN(x, ...)
	#define	   LOG_ERROR(x, ...)
	#define LOG_CRITICAL(x, ...)

	#define    ASSERT_TRACE(x, y, ...)
	#define		ASSERT_INFO(x, y, ...)
	#define		ASSERT_WARN(x, y, ...)
	#define    ASSERT_ERROR(x, y, ...)
	#define ASSERT_CRITICAL(x, y, ...)

	#define OSTAGA_DEBUG_BREAK()
	#define PROFILING 0

	#define AL_CALL(func) func
#endif

#ifndef PROFILE_RESULTS_DIR
	#define PROFILE_RESULTS_DIR "."
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

#ifndef OSTAGA_BUILD_WINDOWS
	#if __GNUC__
		#warning("Warning: This project has currently only been tested on windows. It may fail to run or even build.")
	#endif
#endif
