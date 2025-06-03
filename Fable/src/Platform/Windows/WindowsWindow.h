#pragma once
#include "../vendor/GLFW/include/GLFW/glfw3.h"

#include "Fable/Window.h"

#include "Fable/Renderer/GraphicsContext.h"
#include "Fable/Renderer/Shader.h"

namespace Fable
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		inline void SetVSync(bool enabled) override;
		inline bool GetVSync() const override;
	
		inline virtual std::any GetNativeWindow() const { return m_Window; }		

	private: // FUNCTIONS
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private: // VARIABLES
		GLFWwindow* m_Window;
		//GraphicsContext* m_Context;
		Shader* m_Shader;

		struct WindowData
		{
			std::string Name;
			unsigned int Width = 0, Height = 0;
			bool VSync = true;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}

