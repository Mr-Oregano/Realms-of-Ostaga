
// Precompiled Header
#include "ospch.h"
//

#include <Ostaga.h>
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

		LOG_LINE();

		glm::vec2 myVector = { 0.0f, 1.5f };
		LOG_TRACE("Here is myVector: {0}", myVector);

		glm::mat4 myMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), { 0, 0, 1 });
		LOG_TRACE("Here is myMatrix: {0}", myMatrix);

		LOG_SHUTDOWN();
		return 0;
	}
}