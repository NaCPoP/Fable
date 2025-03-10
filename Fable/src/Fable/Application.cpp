#include "fbpch.h"

#include "Application.h"

namespace Fable
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::WindowCreate());
		m_Vulkan = std::unique_ptr<VulkanApp>(VulkanApp::CreateVulkanApp((GLFWwindow*)m_Window->GetNativeWindow()));
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
			m_Vulkan->drawFrame();
		}
	}
}