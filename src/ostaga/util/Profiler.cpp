
// Precompiled
#include "ospch.h"
//

#include <Ostaga.h>

#include <filesystem>

#include "Profiler.h"

namespace Ostaga {

	Profiler::Data *Profiler::s_Data = nullptr;

	void Profiler::Init()
	{
		s_Data = new Data;
	}

	void Profiler::Shutdown()
	{
		delete s_Data; s_Data = nullptr;
	}

	void Profiler::BeginSession(const std::string &name, const std::string &filepath)
	{
		if (s_Data->currentSession) {
			// If there is already a current session, then close it before beginning new one.
			// Subsequent profiling output meant for the original session will end up in the
			// newly opened session instead.  That's better than having badly formatted
			// profiling output.
			EndSession();
			LOG_WARN("Instrumentor::BeginSession('{0}') when session '{1}' already open.", name, s_Data->currentSession->name);
		}

		if (!std::filesystem::exists(PROFILE_RESULTS_DIR))
			std::filesystem::create_directory(PROFILE_RESULTS_DIR);

		auto &os = s_Data->outputStream;
		os.open(PROFILE_RESULTS_DIR + std::string{"/"} + filepath);

		if (os.is_open()) {

			s_Data->currentSession = new ProfileSession({ name });

			// Write Header
			os << "{\"otherData\": {},\"traceEvents\":[{}";
			os.flush();
			
			return;

		}

		LOG_ERROR("Instrumentor could not open results file '{0}'.", filepath);
	}

	void Profiler::EndSession()
	{
		if (s_Data->currentSession)
		{
			// Write Footer
			auto &os = s_Data->outputStream;
			os << "]}";
			os.flush();

			os.close();
			delete s_Data->currentSession; s_Data->currentSession = nullptr;
		}
	}

	void Profiler::WriteProfile(const ProfileResult &result)
	{
		// If no session is currently active then exit
		if (!s_Data->currentSession)
			return;

		std::stringstream json;
		std::string name = result.name;

		std::replace(name.begin(), name.end(), '"', '\'');

		json << std::setprecision(3) << std::fixed;
		json << ",{";
		json << "\"cat\":\"function\",";
		json << "\"dur\":" << (result.elapsedTime.count()) << ',';
		json << "\"name\":\"" << name << "\",";
		json << "\"ph\":\"X\",";
		json << "\"pid\":0,";
		json << "\"tid\":" << result.threadID << ",";
		json << "\"ts\":" << result.start.count();
		json << "}";

		auto &os = s_Data->outputStream;
		if (os) {
			os << json.str();
			os.flush();
		}
	}

	ProfileTimer::ProfileTimer(const std::string &name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::steady_clock::now();
	}

	ProfileTimer::~ProfileTimer()
	{
		if (!m_Stopped)
			Stop();
	}

	void ProfileTimer::Stop()
	{
		auto endTimepoint = std::chrono::steady_clock::now();
		auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
		auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
			- std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

		Profiler::WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });

		m_Stopped = true;
	}
}

