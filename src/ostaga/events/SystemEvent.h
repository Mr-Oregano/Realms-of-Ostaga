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
		OSTAGA_IF_DEBUG(inline virtual std::string ToString() const
		{
			return "WindowClose";
		})
	};

	struct WindowIconified : public SystemEvent
	{
		bool iconified;

		WindowIconified(bool iconified) : iconified(iconified) {}

		inline virtual EventType GetType() const { return GetClassType(); }
		inline static EventType GetClassType() { return EventType::WindowIconified; }

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(inline virtual std::string ToString() const
		{
			return "WindowIconified";
		})
	};

}