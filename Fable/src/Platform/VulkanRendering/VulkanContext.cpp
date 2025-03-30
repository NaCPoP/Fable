#include "fbpch.h"
#include "VulkanContext.h"
#include "VulkanBuffer.h"

namespace Fable
{
	VulkanContext::VulkanContext(GLFWwindow* window) : m_WindowHandle(window)
	{
		if (window == NULL)
		{
			throw std::runtime_error("Window is null!");
		}
		m_VulkanInstance.setWindow(window);
	}

	void VulkanContext::Init()
	{
		m_VulkanInstance.createInstance();
		m_VulkanInstance.createSurface();
		m_VulkanInstance.pickPhysicalDevice();
		m_VulkanInstance.createLogicalDevice();
		m_VulkanInstance.createSwapchain();
		m_VulkanInstance.createImageViews();
		m_VulkanInstance.createRenderPass();
		m_VulkanInstance.createDescriptorSetLayout();
		m_VulkanInstance.createGraphicsPipeline();
		m_VulkanInstance.createFramebuffers();
		m_VulkanInstance.createCommandPool();
		m_VulkanInstance.createVertexBuffer();
		m_VulkanInstance.createIndexBuffer();
		m_VulkanInstance.createUniformBuffers();
		m_VulkanInstance.createDescriptorPool();
		m_VulkanInstance.createDescriptorSets();
		m_VulkanInstance.createCommandBuffer();
		m_VulkanInstance.createSyncObjects();
	}

	void VulkanContext::SwapBuffers()
	{
		m_VulkanInstance.drawFrame();
	}
}