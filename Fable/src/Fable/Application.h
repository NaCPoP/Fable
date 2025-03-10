#pragma once
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/VulkanRendering/VulkanApp.h"
#include <memory>

namespace Fable
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:

		bool m_Running = true;

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<VulkanApp> m_Vulkan;
	};

	Application* CreateApplication();
}

