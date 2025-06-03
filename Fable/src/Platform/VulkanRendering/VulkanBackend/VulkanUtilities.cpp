#include "fbpch.h"
#include "VulkanUtilities.h"
#include <fstream>

namespace Fable
{
	void VulkanUtilities::loadShaderModule(const std::string& filepath, VkDevice device, VkShaderModule* shaderModule)
	{
		std::vector<char> buffer = readFile(filepath);

		VkShaderModuleCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = buffer.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

		if (vkCreateShaderModule(device, &createInfo, nullptr, shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}
	}

	std::vector<char> VulkanUtilities::readFile(const std::string& filepath)
	{
		std::ifstream file(filepath.c_str(), std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("failed to open file!: " + filepath);
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}
}