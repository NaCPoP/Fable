#include "fbpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/VulkanRendering/VulkanBuffer.h"

namespace Fable
{
	VertexBuffer* VertexBuffer::Create(GraphicsContext* context, glm::vec3* verticies, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:   throw std::runtime_error("RendererAPI::None not supported!");
			case RendererAPI::API::OpenGL: throw std::runtime_error("RendererAPI::OpenGL not supported!");
			case RendererAPI::API::Vulkan: return new VulkanVertexBuffer(context, verticies, size);

			default: throw std::runtime_error("Unsupported RendererAPI type!");
		}
	}

	IndexBuffer* IndexBuffer::Create(GraphicsContext* context, uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:   throw std::runtime_error("RendererAPI::None not supported!");
			case RendererAPI::API::OpenGL: throw std::runtime_error("RendererAPI::OpenGL not supported!");
			case RendererAPI::API::Vulkan: return new VulkanIndexBuffer(context, indices, count);

			default: throw std::runtime_error("Unsupported RendererAPI type!");
		}
	}
}