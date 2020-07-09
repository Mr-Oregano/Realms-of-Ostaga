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

		WindowClose,
		WindowIconified
	};
	enum class EventCategory
	{
		KeyEvent,
		MouseEvent,
		SystemEvent
	};

	struct Event
	{
		bool Handled = false;

		inline virtual EventType GetType() const = 0;
		inline virtual EventCategory GetCategory() const = 0;
		
		template<typename T>
		inline void Dispatch(std::function<bool(T &)> func)
		{
			if (GetType() == T::GetClassType())
			{
				Handled = func(static_cast<T&>(*this));
			}
		};

		// Debugging functionality - 
		//	This will be stripped in non-debug builds
		OSTAGA_IF_DEBUG(inline virtual std::string ToString() const = 0;)
		//
	};
}