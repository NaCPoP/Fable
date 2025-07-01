#pragma once
#include "Fable/Renderer/Buffer.h"
#include "Fable/Renderer/GraphicsContext.h"
#include "VulkanContext.h"

namespace Fable
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		std::vector<VkBuffer> m_VertexBuffers;

	public:
		VulkanVertexBuffer(GraphicsContext* context, glm::vec3* vertices, uint32_t size);
		virtual ~VulkanVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		VulkanContext*	m_Context{ nullptr };
		glm::vec3*		m_Vertices;
		uint32_t		m_Size;
	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		std::vector<VkBuffer> m_IndexBuffers;
		uint32_t m_Count;
	public:
		VulkanIndexBuffer(GraphicsContext* context, uint32_t* indices, uint32_t count);
		virtual ~VulkanIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

	private:
		VulkanContext* m_Context{ nullptr };
		uint32_t* m_Indices;
	};
}

