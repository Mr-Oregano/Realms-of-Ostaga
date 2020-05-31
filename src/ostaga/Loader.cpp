
// Precompiled Header
#include "ospch.h"
//

#include <Ostaga.h>
#include <window/Window.h>

#include <Main.h>

namespace Ostaga {

	int Load()
	{
		LOG_INIT();

		Window window({ 
			1280, 
			720, 
			"Realms of Ostaga", 
			OSTAGA_DEBUG_WRAP(WindowMode::Windowed, WindowMode::WindowedFullscreen), 
			false });

		window.SetVisible(true);
		while (true)
			window.Update();

		LOG_SHUTDOWN();
		return 0;
	}
}