#pragma once

#include "Event.h"

namespace Ostaga {

	struct MouseEvent : public Event
	{
		double x;
		double y;

		MouseEvent(double x, double y) : x(x), y(y) {}

		inline virtual EventCategory GetCategory() const { return EventCategory::MouseEvent; };
	};

	struct MouseDown : public MouseEvent
	{
		int button;

		MouseDown(int button, double x, double y) : MouseEvent(x, y), button(button) {}

		inline virtual EventType GetType() const { return GetClassType(); }
		inline static EventType GetClassType() { return EventType::MouseDown; }

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(inline virtual std::string ToString() const
		{
			std::stringstream ss;
			ss << "MouseDown: " << button << " @ (" << x << ", " << y << ")";
			return ss.str();
		})
	};

	struct MouseUp : public MouseEvent
	{
		int button;

		MouseUp(int button, double x, double y) : MouseEvent(x, y), button(button) {}

		inline virtual EventType GetType() const { return GetClassType(); }
		inline static EventType GetClassType() { return EventType::MouseUp; }

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(inline virtual std::string ToString() const
		{
			std::stringstream ss;
			ss << "MouseUp: " << button << " @ (" << x << ", " << y << ")";
			return ss.str();
		})
	};

	struct MouseMove : public MouseEvent
	{
		MouseMove(double x, double y) : MouseEvent(x, y) {}

		inline virtual EventType GetType() const { return GetClassType(); }
		inline static EventType GetClassType() { return EventType::MouseMove; }

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(inline virtual std::string ToString() const
		{
			std::stringstream ss;
			ss << "MouseMove: (" << x << ", " << y << ")";
			return ss.str();
		})
	};

	struct MouseScrolled : public MouseEvent
	{
		double xOffset;
		double yOffset;

		MouseScrolled(double xOffset, double yOffset, double x, double y)
			: MouseEvent(x, y), xOffset(xOffset), yOffset(yOffset) {}

		inline virtual EventType GetType() const { return GetClassType(); }
		inline static EventType GetClassType() { return EventType::MouseScrolled; }

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(inline virtual std::string ToString() const
		{
			std::stringstream ss;
			ss << "MouseScrolled: (" << xOffset << ", " << yOffset << ") @ (" << x << ", " << y << ")";
			return ss.str();
		})
	};

}