#pragma once
#include "vulkan/vulkan.hpp"

namespace Fable
{
	class VulkanWrapper
	{
	public:
		/*
		*	RENDERING
		*/
		static void BeginRenderPass(VkCommandBuffer cmdBuffer, VkRenderPass renderPass, VkExtent2D extent, VkFramebuffer frameBuffers, VkClearValue clearValue);
		static void EndRenderPass(VkCommandBuffer cmdBuffer);

		static void BeginCommandBuffer(VkCommandBuffer commandBuffer, VkCommandBufferUsageFlags flags);
		static void EndCommandBuffer(VkCommandBuffer cmdBuffer);
	};
}

