#include "fbpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/VulkanRendering/VulkanBuffer.h"

namespace Fable
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:		return nullptr;
			case RendererAPI::Vulkan:	return new VulkanVertexBuffer(vertices, size);
		}

		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::None:		return nullptr;
			case RendererAPI::Vulkan:	return new VulkanIndexBuffer(indices, count);
		}

		return nullptr;
	}
}