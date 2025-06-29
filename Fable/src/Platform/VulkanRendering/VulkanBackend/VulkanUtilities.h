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

		static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, 
			VkMemoryPropertyFlags properties);
		
		static VkCommandBuffer beginSingleTimeCommands(VkCommandPool commandPool, VkDevice device);
		static void endSingleTimeCommands(VkCommandBuffer commandBuffer, VkCommandPool commandPool, VkDevice device,
			VkQueue queue);
		
		/*
		*  BUFFERS
		*/
		static VkBuffer createBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage,
			VkMemoryPropertyFlags properties, VkDeviceMemory& bufferMemory);
		static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkCommandPool commandPool,
			VkDevice device, VkQueue queue);
	};
}

