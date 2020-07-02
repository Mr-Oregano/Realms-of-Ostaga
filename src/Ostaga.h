#pragma once

#define OSTAGA_OPENGL_VERSION_MAJOR 4
#define OSTAGA_OPENGL_VERSION_MINOR 6

#if defined(OSTAGA_DEBUG) || defined(OSTAGA_RELEASE)
	#define OSTAGA_IF_DEBUG(x, ...) x

	// The logger will only be included in the program for debug builds
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
#endif

#ifndef OSTAGA_BUILD_WINDOWS
	#if __GNUC__
		#warning("Warning: This project has currently only been tested on windows. It may fail to run or even build.")
	#endif
#endif
