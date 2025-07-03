#pragma once
#include "Platform/Windows/WindowsWindow.h"
#include "Events/ApplicationEvents.h"
#include "LayerStack.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"
#include "Renderer/GraphicsContext.h"
#include "Camera.h"

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

		static Application& Get() { return *s_Instance; }
		Window& GetWindow() { return *m_Window; }

	private:
		static Application* s_Instance;

		bool m_Running		= true;
		bool m_Minimized	= false;

		LayerStack m_LayerStack;

		bool OnWindowClose(WindowCloseEvent& event);
		bool OnWindowResize(WindowResizeEvent& event);

		std::unique_ptr<Window> m_Window;
		std::unique_ptr<GraphicsContext> m_Context;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		// CAMERA
		PerspectiveCamera m_Camera;
	};

	Application* CreateApplication();
}

