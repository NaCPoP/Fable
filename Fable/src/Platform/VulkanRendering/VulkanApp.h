#pragma once

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include "../vendor/GLFW/include/GLFW/glfw3.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#include "../vendor/GLFW/include/GLFW/glfw3native.h"

#include <limits>
#include <optional>
#include <fstream>
#include <vector>

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

		static std::vector<char> readFile(const std::string& filename);

		VkShaderModule createShaderModule(const std::vector<char>& code);

		void drawFrame();

		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
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

		std::vector<VkImageView> m_SwapchainImageViews;
		std::vector<VkFramebuffer> m_SwapchainFrameBuffer;

		VkPipelineLayout m_PipelineLayout;
		VkRenderPass m_RenderPass;
		VkPipeline m_GraphicsPipeline;

		VkCommandPool m_CommandPool;
		VkCommandBuffer m_CommandBuffer;

		VkSemaphore m_ImageAvailableSemaphore;
		VkSemaphore m_RenderFinishedSemaphore;
		VkFence m_InFlightFence;

		virtual void Init();
		virtual void Shutdown();
	};
}

