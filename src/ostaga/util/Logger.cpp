
// Precompiled
#include "ospch.h"
//

#include "Logger.h"

namespace Ostaga {

	std::shared_ptr<spdlog::logger> Logger::s_Logger;

	void Logger::Init()
	{
		spdlog::set_pattern("[%T] %n: %^%v%$");

		s_Logger = spdlog::stdout_color_mt("OSTAGA-LOG");
		s_Logger->set_level(spdlog::level::trace);

		auto color_sink = static_cast<spdlog::sinks::stdout_color_sink_mt*>(s_Logger->sinks()[0].get());
		color_sink->set_color(spdlog::level::info, color_sink->CYAN);
	}

	void Logger::Shutdown()
	{
	}
}