#pragma once

#include "Fable/Window.h"

#include "Platform/VulkanRendering/VulkanApp.h"

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
		inline void SetVSync(bool enabled) override;
		inline bool GetVSync() const override;
	
		inline virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		GLFWwindow* m_Window;

		VulkanApp* m_VulkanApp;

		struct WindowData
		{
			std::string Name;
			unsigned int Width = 0, Height = 0;
			bool VSync = true;
		};

		WindowData m_Data;
	};
}

