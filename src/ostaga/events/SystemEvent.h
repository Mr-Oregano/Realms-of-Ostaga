#pragma once

#include "Event.h"

namespace Ostaga {

	struct SystemEvent : public Event
	{
		inline virtual EventCategory GetCategory() const { return EventCategory::SystemEvent; };
	};

	struct WindowClose : public SystemEvent
	{
		inline virtual EventType GetType() const { return GetClassType(); }
		inline static EventType GetClassType() { return EventType::WindowClose; }

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_DEBUG_WRAP(inline virtual std::string ToString() const
		{
			return "WindowClose";
		})
	};

}