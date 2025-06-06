#include "fbpch.h"
#pragma once

#include "Fable/Renderer/GraphicsContext.h"

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include "../vendor/GLFW/include/GLFW/glfw3.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <set>
#include <chrono>
#include <limits>
#include <optional>
#include <vulkan/vulkan_core.h>

struct GLFWwindow;

namespace Fable
{
	struct SwapChainSupportDetails 
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct QueueFamilyIndices 
	{
		uint32_t graphicsFamily;
		uint32_t presentFamily;
		bool graphicsFamilyHasValue = false;
		bool presentFamilyHasValue = false;
		inline bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
	};

	class VulkanContext : public GraphicsContext
	{
		friend class VulkanShader;
	public:
		VulkanContext(GLFWwindow* window);
		~VulkanContext();

		virtual void Init(RendererSettings settings) override;
		virtual void Shutdown() override;
		virtual void SwapBuffers() override;
		virtual void Draw() override;
	
	private:
		GLFWwindow* m_Window;
		RendererSettings m_Settings;
		uint32_t m_CurrentFrame = 0;
		uint32_t m_ImageIndex = 0;
		/*
		*	CORE VULKAN
		*/
		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> m_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		VkPhysicalDeviceProperties m_Properties;

		VkInstance			m_Instance;
		VkSurfaceKHR		m_Surface;
		VkDevice			m_Device;			// Logical Device
		VkPhysicalDevice	m_PhysicalDevice;	// Physical Device

		VkDebugUtilsMessengerEXT m_DebugMessenger;
		/*
		* SWAPCHAIN
		*/
		VkSwapchainKHR				m_Swapchain;
		VkFormat					m_SwapchainImageFormat;
		VkExtent2D					m_SwapchainExtent;
		std::vector<VkImage>		m_SwapchainImages;
		std::vector<VkImageView>	m_SwapchainImageViews;
		/*
		* COMMAND POOLS AND QUEUES
		*/
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		VkCommandPool	m_CommandPool;
		VkCommandBuffer m_CommandBuffer;
		/*
		* RENDER PASSES
		*/
		VkRenderPass				m_RenderPass;
		std::vector<VkFramebuffer>	m_FrameBuffers;
		VkPipeline m_GraphicsPipeline{ VK_NULL_HANDLE };
		/*
		* SYNC OBJECTS
		*/
		VkSemaphore m_PresentSemaphore, m_RenderSemaphore;
		VkFence		m_RenderFence;
		/*
		*	FUNCTIONS
		*/
		void initCore();
		void createSwapchain();
		void createCommandPools();
		void createImageViews();
		void createDefaultRenderPass();
		void createFrameBuffers();
		void createSyncStructures();
		void createCommandBuffers();
		void recordCommandBuffers();

		void createInstance();
		void createSurface();
		void pickPhysicalDevice();
		void createLogicalDevice();

		inline QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(m_PhysicalDevice); }

		// Helper Functions
		bool isDeviceSuitable(VkPhysicalDevice device);
		std::vector<const char*> getRequiredExtensions();
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);

		void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

		VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

		VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
	};
}