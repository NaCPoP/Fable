#include "fbpch.h"

#include "Application.h"
#include "Fable/Core.h"

namespace Fable
{
	Application::Application()
	{
		RendererSettings settings
		{

		};

		m_Window = std::unique_ptr<Window>(Window::WindowCreate());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_Context.reset(new VulkanContext(any_cast<GLFWwindow*>(m_Window->GetNativeWindow())));
		m_Context->Init(settings);

		m_Shader.reset(m_Shader->Create(m_Context.get()));
		m_Shader->Load("C:/dev/Fable/Fable/src/Shaders/vert.spv", "C:/dev/Fable/Fable/src/Shaders/frag.spv");
		m_Shader->Bind();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Context->Draw();
			m_Window->OnUpdate();
			m_Context->SwapBuffers();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(event);
			if (event.m_Handled)
				break;
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& event)
	{
		return true;
	}
}