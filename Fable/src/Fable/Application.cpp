#include "fbpch.h"

#include "Application.h"
#include "Fable/Core.h"
#include "Fable/Renderer/Renderer.h"

#include "glm/glm.hpp"

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

		RenderCommand::SetContext(m_Context.get());

		glm::vec3 vertices[4]
		{
			{-0.5f, -0.5f, 0.0f},
			{ 0.5f,  0.5f, 0.0f},
			{-0.5f,  0.5f, 0.0f},
			{ 0.5f, -0.5f, 0.0f}
		};

		m_VertexBuffer.reset(m_VertexBuffer->Create(m_Context.get(), vertices, sizeof(vertices)));

		uint32_t indices[6] = { 0, 1, 2, 0, 3, 1 };

		m_IndexBuffer.reset(m_IndexBuffer->Create(m_Context.get(), indices, sizeof(indices)));

		m_Shader.reset(m_Shader->Create(m_Context.get()));
		m_Shader->Load("../Fable/src/Shaders/vert1.spv", "../Fable/src/Shaders/frag1.spv");
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });

		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(-60.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		while (m_Running)
		{
			glm::mat4 projection = glm::perspective(glm::radians(90.0f), m_Window->GetWidth() / (float)m_Window->GetHeight(), 0.1f, 1000.0f);

			Renderer::BeginScene(m_Context.get());

			m_Shader->Bind();
			m_Shader->LoadUniformBuffer(projection, view, model);
			Renderer::Submit(m_VertexBuffer.get(), m_IndexBuffer.get());
			
			Renderer::EndScene();

			m_Window->OnUpdate();
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
		if (event.GetWidth() == 0 || event.GetHeight() == 0)
		{
			m_Minimized = true;
		}

		m_Minimized = false;

		Renderer::WindowResize(event.GetWidth(), event.GetHeight());

		return false;
	}
}