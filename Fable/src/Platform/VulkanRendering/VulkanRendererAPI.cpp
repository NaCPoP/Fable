#include "fbpch.h"
#include "VulkanRendererAPI.h"
#include "VulkanBackend/VulkanUtilities.h"
#include "VulkanBackend/VulkanWrapper.h"
#include "VulkanBuffer.h"

namespace Fable
{
	void VulkanRendererAPI::SetContext(GraphicsContext* context)
	{
		m_Context = static_cast<VulkanContext*>(context);
	}

	void VulkanRendererAPI::SetClearColor(const glm::vec4& color)
	{
		m_ClearColor = {color.r, color.g, color.b, color.a};
	}

	void VulkanRendererAPI::WindowResize(uint32_t width, uint32_t height)
	{
		m_Context->recreateSwapchain(width, height);
	}

	void VulkanRendererAPI::BeginRender()
	{
		vkWaitForFences(m_Context->m_Device, 1, &m_Context->m_RenderFence, VK_TRUE, UINT64_MAX);

		if (vkAcquireNextImageKHR(m_Context->m_Device, m_Context->m_Swapchain, UINT64_MAX, m_Context->m_PresentSemaphore, VK_NULL_HANDLE, &m_Context->m_ImageIndex) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to aquire swapchain image!");
		}

		vkResetFences(m_Context->m_Device, 1, &m_Context->m_RenderFence);
		vkResetCommandBuffer(m_Context->m_CommandBuffers[m_CurrentFrame], 0);

		VulkanWrapper::BeginCommandBuffer(m_Context->m_CommandBuffers[m_CurrentFrame], VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		VulkanWrapper::BeginRenderPass(m_Context->m_CommandBuffers[m_CurrentFrame], m_Context->m_RenderPass, m_Context->m_SwapchainExtent,
									   m_Context->m_FrameBuffers[m_Context->m_ImageIndex], m_ClearColor);
	}

	void VulkanRendererAPI::EndRender()
	{
		VulkanWrapper::EndRenderPass(m_Context->m_CommandBuffers[m_CurrentFrame]);

		VulkanWrapper::EndCommandBuffer(m_Context->m_CommandBuffers[m_CurrentFrame]);

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo{};
		submitInfo.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount	= 1;
		submitInfo.pWaitSemaphores		= &m_Context->m_PresentSemaphore;
		submitInfo.pWaitDstStageMask	= waitStages;
		submitInfo.commandBufferCount	= 1;
		submitInfo.pCommandBuffers		= &m_Context->m_CommandBuffers[m_CurrentFrame];

		VkSemaphore signalSemaphores[]	= { m_Context->m_RenderSemaphore };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores	= signalSemaphores;

		if (vkQueueSubmit(m_Context->m_GraphicsQueue, 1, &submitInfo, m_Context->m_RenderFence) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to submit draw command to buffer!");
		}

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount	= 1;
		presentInfo.pWaitSemaphores		= signalSemaphores;

		VkSwapchainKHR swapchains[]		= { m_Context->m_Swapchain };
		presentInfo.swapchainCount		= 1;
		presentInfo.pSwapchains			= swapchains;
		presentInfo.pImageIndices		= &m_Context->m_ImageIndex;
		presentInfo.pResults			= nullptr;

		if (vkQueuePresentKHR(m_Context->m_PresentQueue, &presentInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("");
		}

		m_CurrentFrame = (m_CurrentFrame + 1) % m_MaxFramesInFlight;
		m_Context->m_CurrentFrame = m_CurrentFrame;
	}

	void VulkanRendererAPI::DrawIndexed(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
	{
		auto vertBuffers	= static_cast<VulkanVertexBuffer*>(vertexBuffer);
		auto idxBuffers		= static_cast<VulkanIndexBuffer*>(indexBuffer);

		vertBuffers->Bind();
		idxBuffers->Bind();

		vkCmdDrawIndexed(m_Context->m_CommandBuffers[m_CurrentFrame], m_Context->m_IndicesCount, 1, 0, 0, 0);
	}
}