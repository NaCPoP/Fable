#pragma once

#include <string>
#include <vulkan/vulkan.h>

namespace Fable
{
	class VulkanUtilities
	{
	public:
		static void loadShaderModule(const std::string& filepath, VkDevice device, VkShaderModule* shaderModule);
		static std::vector<char> readFile(const std::string& filepath);
	};
}

