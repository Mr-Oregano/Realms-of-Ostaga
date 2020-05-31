
// Precompiled Header
#include "ospch.h"
//

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