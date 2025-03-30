#pragma once

#include "fbpch.h"

#include "Fable/Core.h"

namespace Fable
{
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
							   virtual EventType GetEventType() const override { return GetStaticType(); }\
							   virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	enum class EventType
	{
		None = 0,
		WindowResize, WindowMove, WindowClose,
		KeyPress, KeyRelease, KeyTyped,
		MousePress, MouseRelease, MouseMove, MouseScroll,
		AppTick, AppUpdate, AppRender
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryMouse = BIT(2),
		EventCategoryMouseButton = BIT(3),
		EventCategoryKeyBoard = BIT(4)
	};

	class Event
	{
	public:
		bool m_Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;

		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory& category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:

		EventDispatcher(Event& event) : m_Event(event) {}

		// Checks type matches input type
		//		- if true calls input function then returns true
		//		- else returns false
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}