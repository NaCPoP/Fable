#include "fbpch.h"
#include "Shader.h"

#include "RenderAPI.h"
#include "Platform/VulkanRendering/VulkanShader.h"

namespace Fable
{
	Shader* Shader::Create(GraphicsContext* context)
	{
		switch (RenderAPI::GetAPI())
		{
			case RendererAPI::None: throw std::runtime_error("RendererAPI::None not supported!");
			case RendererAPI::OpenGL: throw std::runtime_error("RendererAPI::OpenGL not supported!");
			case RendererAPI::Vulkan: return new VulkanShader(context);

			default: throw std::runtime_error("Unsupported RendererAPI type!");
		}
	}
}