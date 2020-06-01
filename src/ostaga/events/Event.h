#pragma once

#include <Ostaga.h>

namespace Ostaga {

	enum class EventType
	{
		KeyDown,
		KeyUp,

		MouseDown,
		MouseUp,
		MouseMove,
		MouseScrolled,

		WindowClose
	};
	enum class EventCategory
	{
		KeyEvent,
		MouseEvent,
		SystemEvent
	};

	struct Event
	{
		inline virtual EventType GetType() const = 0;
		inline virtual EventCategory GetCategory() const = 0;
		
		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_DEBUG_WRAP(inline virtual std::string ToString() const = 0;)
		//
	};
}