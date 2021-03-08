
// Precompiled
#include "ospch.h"
//

#include <Ostaga.h>
#include <Logger.h>
#include <Event.h>
#include <Maths.h>

std::ostream &operator<<(std::ostream &os, const glm::vec2 &v)
{
	return os << "(" << v.x << ", " << v.y << ")";
}
std::ostream &operator<<(std::ostream &os, const glm::vec3 &v)
{
	return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}
std::ostream &operator<<(std::ostream &os, const glm::vec4 &v)
{
	return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
}
std::ostream &operator<<(std::ostream &os, const glm::mat4 &v)
{
	char buf[1024] = { 0 };
	sprintf(buf,
		"\t\t\t[%9.3f  %9.3f  %9.3f  %9.3f]\n"
		"\t\t\t[%9.3f  %9.3f  %9.3f  %9.3f]\n"
		"\t\t\t[%9.3f  %9.3f  %9.3f  %9.3f]\n"
		"\t\t\t[%9.3f  %9.3f  %9.3f  %9.3f]\n",
		v[0][0], v[0][1], v[0][2], v[0][3],
		v[1][0], v[1][1], v[1][2], v[1][3],
		v[2][0], v[2][1], v[2][2], v[2][3],
		v[3][0], v[3][1], v[3][2], v[3][3]);

	return os << "\n" << std::string(buf);
}
std::ostream &operator<<(std::ostream &os, const Ostaga::Event &e)
{
	return os OSTAGA_IF_DEBUG(<< e.ToString());
}

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