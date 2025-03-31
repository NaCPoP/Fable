#pragma once
#include "Platform/Windows/WindowsWindow.h"
#include "Events/ApplicationEvents.h"
#include "LayerStack.h"

//#include "Renderer/Buffer.h"

#include <memory>

namespace Fable
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);
	private:

		bool m_Running = true;

		std::unique_ptr<Window> m_Window;
		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);
	};

	Application* CreateApplication();
}

