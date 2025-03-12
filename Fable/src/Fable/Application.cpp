#include "fbpch.h"

#include "Application.h"

namespace Fable
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::WindowCreate());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}