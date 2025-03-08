#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include "../vendor/GLFW/include/GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "../vendor/GLFW/include/GLFW/glfw3native.h"

#include <limits>
#include <optional>

namespace Fable
{
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	class VulkanApp
	{
	public:
		VulkanApp(GLFWwindow* window);
		virtual ~VulkanApp();

		static VulkanApp* CreateVulkanApp(GLFWwindow* window);
	private:
		GLFWwindow* m_Window;

		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;
		VkSurfaceKHR m_Surface;

		VkExtent2D m_ActualExtent;
		VkSwapchainKHR m_Swapchain;
		std::vector<VkImage> m_SwapchainImages;
		VkFormat m_SwapchainImageFormat;
		VkExtent2D m_SwapchainExtent;

		virtual void Init();
		virtual void Shutdown();
	};
}

