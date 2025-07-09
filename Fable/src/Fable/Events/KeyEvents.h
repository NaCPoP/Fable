#pragma once

#include "Events.h"

namespace Fable
{
	class KeyEvent : public Event
	{
	public:
		inline int GetKeycode() { return m_Keycode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyBoard | EventCategoryInput);
	protected:
		KeyEvent(int keycode) : m_Keycode(keycode) {}

		int m_Keycode;
	};

	class KeyPressEvent : public KeyEvent
	{
	public:
		KeyPressEvent(int keycode, int repeatCount) : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

		inline int GetRepeatCount() { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << m_Keycode << " (" << m_RepeatCount << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPress);
	private:
		int m_RepeatCount;
	};

	class KeyReleaseEvent : public KeyEvent
	{
	public:
		KeyReleaseEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleaseEvent: " << m_Keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyRelease);
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_Keycode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped);
	};
}