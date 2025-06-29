#include "fbpch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/VulkanRendering/VulkanShader.h"

namespace Fable
{
	Shader* Shader::Create(GraphicsContext* context)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: throw std::runtime_error("RendererAPI::None not supported!");
			case RendererAPI::API::OpenGL: throw std::runtime_error("RendererAPI::OpenGL not supported!");
			case RendererAPI::API::Vulkan: return new VulkanShader(context);

			default: throw std::runtime_error("Unsupported RendererAPI type!");
		}
	}
}