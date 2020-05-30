#pragma once

#include <string>
#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "spdlog/fmt/ostr.h"

inline std::ostream &operator<<(std::ostream &os, const glm::vec2 &v)
{
	return os << "(" << v.x << ", " << v.y << ")";
}
inline std::ostream &operator<<(std::ostream &os, const glm::vec3 &v)
{
	return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}
inline std::ostream &operator<<(std::ostream &os, const glm::vec4 &v)
{
	return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
}
inline std::ostream &operator<<(std::ostream &os, const glm::mat4 &v)
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

namespace Ostaga  { 

	class Logger
	{
	public:
		static void Init();
		static void Shutdown();

		inline static std::shared_ptr<spdlog::logger> Get() { return s_Logger; }

	private:
		static std::shared_ptr<spdlog::logger> s_Logger;
	};
}