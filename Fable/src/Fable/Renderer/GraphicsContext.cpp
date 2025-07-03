#include <fbpch.h>
#include "GraphicsContext.h"

#include "Renderer.h"
#include "Platform/VulkanRendering/VulkanContext.h"

namespace Fable
{
	GraphicsContext* GraphicsContext::Create(Window* window, RendererSettings settings)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:   throw std::runtime_error("RendererAPI::None not supported!");
		case RendererAPI::API::OpenGL: throw std::runtime_error("RendererAPI::OpenGL not supported!");
		case RendererAPI::API::Vulkan: return new VulkanContext(window, settings);

		default: throw std::runtime_error("Unsupported RendererAPI type!");
		}
	}
}