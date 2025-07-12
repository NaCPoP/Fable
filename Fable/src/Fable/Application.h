#pragma once
#include "Platform/Windows/WindowsWindow.h"
#include "Events/ApplicationEvents.h"
#include "LayerStack.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/GraphicsContext.h"
#include "Camera.h"

#include <memory>
#include "ImGui/ImGuiLayer.h"

namespace Fable
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }
		GraphicsContext& GetContext() { return *m_Context; }

	private:
		static Application* s_Instance;

		bool m_Running		= true;
		bool m_Minimized	= false;

		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<GraphicsContext> m_Context;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		// LAYERS
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;

		// CAMERA
		PerspectiveCamera m_Camera;
	};

	Application* CreateApplication();
}

