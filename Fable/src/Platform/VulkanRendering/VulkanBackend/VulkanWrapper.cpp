#include "fbpch.h"
#include "VulkanWrapper.h"

namespace Fable
{
	/*
	*	RENDERING
	*/
	void VulkanWrapper::BeginRenderPass(VkCommandBuffer cmdBuffer, VkRenderPass renderPass, VkExtent2D extent, VkFramebuffer frameBuffers, VkClearValue clearValue)
	{
		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass			= renderPass;
		renderPassBeginInfo.renderArea.offset	= { 0, 0 };
		renderPassBeginInfo.renderArea.extent	= extent;
		renderPassBeginInfo.framebuffer			= frameBuffers;
		renderPassBeginInfo.clearValueCount		= 1;
		renderPassBeginInfo.pClearValues		= &clearValue;

		vkCmdBeginRenderPass(cmdBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanWrapper::EndRenderPass(VkCommandBuffer cmdBuffer)
	{
		vkCmdEndRenderPass(cmdBuffer);
	}

	void VulkanWrapper::BeginCommandBuffer(VkCommandBuffer cmdBuffer, VkCommandBufferUsageFlags flags)
	{
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = flags;

		if (vkBeginCommandBuffer(cmdBuffer, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin command buffer!");
		}
	}

	void VulkanWrapper::EndCommandBuffer(VkCommandBuffer cmdBuffer)
	{
		if (vkEndCommandBuffer(cmdBuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to end command Buffer!");
		}
	}
}