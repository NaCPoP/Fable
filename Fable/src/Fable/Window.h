#pragma once

#include "fbpch.h"
#include "Events/Events.h"

#include <any>

namespace Fable
{
	struct WindowProps
	{
		std::string Name;
		unsigned int Width, Height;

		WindowProps(const std::string& name = "Fable", unsigned int width = 1280, unsigned int height = 720)
			: Name(name), Width(width), Height(height) {}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool GetVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* WindowCreate(const WindowProps& props = WindowProps());
	};
}