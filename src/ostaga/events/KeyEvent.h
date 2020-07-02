#pragma once

#include "Event.h"

namespace Ostaga
{
	struct KeyEvent : public Event
	{
		int keyCode;

		KeyEvent(int keyCode) : keyCode(keyCode) {}

		inline virtual EventCategory GetCategory() const { return EventCategory::KeyEvent; };

	};

	struct KeyDown : public KeyEvent
	{
		KeyDown(int keyCode) : KeyEvent(keyCode) {}

		inline virtual EventType GetType() const { return GetClassType(); }
		inline static EventType GetClassType() { return EventType::KeyDown; }

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(inline virtual std::string ToString() const
		{
			std::stringstream ss;
			ss << "KeyDown: " << keyCode << " (" << (char)keyCode << ")";
			return ss.str();
		})
	};

	struct KeyUp : public KeyEvent
	{
		KeyUp(int keyCode) : KeyEvent(keyCode) {}

		inline virtual EventType GetType() const { return GetClassType(); }
		inline static EventType GetClassType() { return EventType::KeyUp; }

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(inline virtual std::string ToString() const
		{
			std::stringstream ss;
			ss << "KeyUp: " << keyCode << " (" << (char)keyCode << ")";
			return ss.str();
		})
	};

}