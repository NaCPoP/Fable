#include "fbpch.h"

#include "Application.h"
#include "Fable/Core.h"
#include "Fable/Renderer/Renderer.h"
#include "Platform/VulkanRendering/VulkanContext.h"

#include "glm/glm.hpp"

#include "Input.h"

namespace Fable
{
	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Camera(45.0f, 1280/(float)720, 0.1f, 1000.0f)
	{
		s_Instance = this;

		// TODO: Move into Sandbox.cpp
		RendererSettings settings
		{

		};

		m_Window = std::unique_ptr<Window>(Window::WindowCreate());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_Context.reset(m_Context->Create(m_Window.get(), settings));

		RenderCommand::SetContext(m_Context.get());

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

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

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		m_Camera.SetPostition({ 0.0f,  0.0f, -10.0f });
		m_Camera.SetRotation(0.0f);
		m_Camera.SetCenter({ 0.0f,  0.0f, 0.0f });

		float z = 0;
		float x = 0;

		while (m_Running)
		{
			Renderer::BeginScene(m_Context.get());

			m_Shader->Bind();
			m_Shader->LoadUniformBuffer(m_Camera.GetProjectionMatrix(), m_Camera.GetViewMatrix(), model);
			Renderer::Submit(m_VertexBuffer.get(), m_IndexBuffer.get());
			
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			Renderer::EndScene();

			if (Input::IsKeyPressed(GLFW_KEY_W))
			{
				z += 0.001;
				m_Camera.SetPostition({ x, 0.0f, -10.0f + z });
			}
			if (Input::IsKeyPressed(GLFW_KEY_S))
			{
				z -= 0.001;
				m_Camera.SetPostition({ x, 0.0f, -10.0f + z });
			}
			if (Input::IsKeyPressed(GLFW_KEY_A))
			{
				x -= 0.001;
				m_Camera.SetPostition({ x, 0.0f, -10.0f + z });
			}
			if (Input::IsKeyPressed(GLFW_KEY_D))
			{
				x += 0.001;
				m_Camera.SetPostition({ x, 0.0f, -10.0f + z });
			}

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

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
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
		m_Camera.SetAspect(event.GetWidth()/ (float)event.GetHeight());

		return false;
	}
}