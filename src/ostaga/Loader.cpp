
// Precompiled Header
#include "ospch.h"
//

#if defined(OSTAGA_BUILD_WINDOWS) && defined(OSTAGA_DIST)

	// Enable optimus for hybrid graphics systems in order to 
	// use high performance graphics.

	extern "C" {
		__declspec(dllexport) unsigned long NvOptimusEnablement = 0x01;
		__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0x01;
	}

#endif

#include <Ostaga.h>
#include <window/Window.h>
#include <Application.h>

#include <Main.h>

namespace Ostaga {

	int Load()
	{
		LOG_INIT();

		Application app;
		app.Run();

		LOG_SHUTDOWN();
		return 0;
	}
}