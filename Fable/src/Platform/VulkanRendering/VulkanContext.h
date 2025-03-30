#pragma once

#include "Fable/Renderer/GraphicsContext.h"
#include "VulkanMain.h"

struct GLFWwindow;

namespace Fable
{
	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext(GLFWwindow* window);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	
	private:
		GLFWwindow* m_WindowHandle;
		VulkanMain m_VulkanInstance;
	};
}