#include "fbpch.h"
#include "RenderCommand.h"
#include "Platform/VulkanRendering/VulkanRendererAPI.h"

namespace Fable
{
	RendererAPI* RenderCommand::s_RendererAPI = new VulkanRendererAPI;
}