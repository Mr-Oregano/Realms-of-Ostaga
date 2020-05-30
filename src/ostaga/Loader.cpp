
// Precompiled Header
#include "ospch.h"
//

#include <Core.h>
#include <Main.h>

namespace Ostaga {

	int Load()
	{
		LOG_INIT();

		LOG_TRACE("Hello this is a trace message. My name is {0}", "Sympleque");
		LOG_INFO("Hello this is an info message. My name is {0}", "Sympleque");
		LOG_WARN("Hello this is a warn message. My name is {0}", "Sympleque");
		LOG_ERROR("Hello this is an error message. My name is {0}", "Sympleque");
		LOG_CRITICAL("Hello this is a critical message. My name is {0}", "Sympleque");

		LOG_SHUTDOWN();
		return 0;
	}

}