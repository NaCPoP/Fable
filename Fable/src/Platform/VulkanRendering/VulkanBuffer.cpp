#include "fbpch.h"
#include "VulkanBuffer.h"
#include "VulkanBackend/VulkanUtilities.h"
#include "VulkanBackend/VulkanPipelineBuilder.h"

namespace Fable
{
	// ----------------------------------------------------------------------------------------------
	//											VERTEX BUFFER
	// ----------------------------------------------------------------------------------------------

	VulkanVertexBuffer::VulkanVertexBuffer(GraphicsContext* context, glm::vec3* vertices, uint32_t size)
		: m_Context(static_cast<VulkanContext*>(context)), m_Vertices(vertices), m_Size(size)
	{
		VkDeviceSize bufferSize = sizeof(m_Vertices[0]) * m_Size;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		stagingBuffer = VulkanUtilities::createBuffer(m_Context->m_Device, m_Context->m_PhysicalDevice, bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBufferMemory);

		void* data;
		vkMapMemory(m_Context->m_Device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, m_Vertices, (size_t)bufferSize);
		vkUnmapMemory(m_Context->m_Device, stagingBufferMemory);

		m_VertexBuffers.emplace_back(VulkanUtilities::createBuffer(m_Context->m_Device, m_Context->m_PhysicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_Context->m_VertexBufferMemory));

		VulkanUtilities::copyBuffer(stagingBuffer, m_VertexBuffers.back(), bufferSize, m_Context->m_CommandPool,
			m_Context->m_Device, m_Context->m_GraphicsQueue);

		vkDestroyBuffer(m_Context->m_Device, stagingBuffer, nullptr);
		vkFreeMemory(m_Context->m_Device, stagingBufferMemory, nullptr);
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		for (VkBuffer buffer : m_VertexBuffers)
		{
			vkDestroyBuffer(m_Context->m_Device, buffer, nullptr);
		}
	}

	void VulkanVertexBuffer::Bind() const
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_Context->m_CommandBuffers[m_Context->m_CurrentFrame], 0, 1, m_VertexBuffers.data(), offsets);
	}

	void VulkanVertexBuffer::Unbind() const
	{
		VulkanVertexBuffer::~VulkanVertexBuffer();
	}

	// ----------------------------------------------------------------------------------------------
	//											INDEX BUFFER
	// ----------------------------------------------------------------------------------------------

	VulkanIndexBuffer::VulkanIndexBuffer(GraphicsContext* context, uint32_t* indices, uint32_t count)
		: m_Context(static_cast<VulkanContext*>(context)), m_Indices(indices), m_Count(count)
	{
		m_Context->m_IndicesCount = m_Count;

		VkDeviceSize bufferSize = sizeof(m_Indices[0]) * m_Count;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		stagingBuffer = VulkanUtilities::createBuffer(m_Context->m_Device, m_Context->m_PhysicalDevice, bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBufferMemory);

		void* data;

		vkMapMemory(m_Context->m_Device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, m_Indices, (size_t)bufferSize);
		vkUnmapMemory(m_Context->m_Device, stagingBufferMemory);

		m_IndexBuffer = VulkanUtilities::createBuffer(m_Context->m_Device, m_Context->m_PhysicalDevice,
			bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_Context->m_IndexBufferMemory);

		VulkanUtilities::copyBuffer(stagingBuffer, m_IndexBuffer, bufferSize, m_Context->m_CommandPool,
			m_Context->m_Device, m_Context->m_GraphicsQueue);

		vkDestroyBuffer(m_Context->m_Device, stagingBuffer, nullptr);
		vkFreeMemory(m_Context->m_Device, stagingBufferMemory, nullptr);
	}

	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		vkDestroyBuffer(m_Context->m_Device, m_IndexBuffer, nullptr);
	}

	void VulkanIndexBuffer::Bind() const
	{
		vkCmdBindIndexBuffer(m_Context->m_CommandBuffers[m_Context->m_CurrentFrame], m_IndexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void VulkanIndexBuffer::Unbind() const
	{
		VulkanIndexBuffer::~VulkanIndexBuffer();
	}
}
