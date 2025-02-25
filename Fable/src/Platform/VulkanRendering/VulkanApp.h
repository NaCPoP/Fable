#pragma once

#define GLFW_INCLUDE_VULKAN
#include "../vendor/GLFW/include/GLFW/glfw3.h"

namespace Fable
{
	class VulkanApp
	{
	public:
		VulkanApp();
		virtual ~VulkanApp();

		static VulkanApp* VulkanCreate();
	private:
		VkInstance m_Instance;
		VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;

		virtual void Init();
		virtual void Shutdown();
	};
}

