#pragma once
#include "vulkan/vulkan.h"


namespace Fable
{
	class VulkanInitalizers
	{
	public:
		//	GRAPHICS PIPELINE
		static VkPipelineShaderStageCreateInfo createShaderStageInfo(VkShaderStageFlagBits stage, VkShaderModule module);
		static VkPipelineVertexInputStateCreateInfo createVertexInputInfo(VkVertexInputBindingDescription bindingDescription,
																		  VkVertexInputAttributeDescription attributeDescription);
		static VkPipelineInputAssemblyStateCreateInfo createInputAssemblyInfo(VkPrimitiveTopology topology);
		static VkPipelineRasterizationStateCreateInfo createRasterizationInfo(VkPolygonMode polygonMode);
		static VkPipelineMultisampleStateCreateInfo createMultisampleInfo();
		static VkPipelineColorBlendAttachmentState createColorBlendAttachment();
		static VkPipelineLayoutCreateInfo createPipelineLayout(VkDescriptorSetLayout descriptorSetLayout, VkPushConstantRange pushConstant);

		//	VERTEX BUFFERS
		static VkVertexInputBindingDescription createVertexBindingDescription();
		static VkVertexInputAttributeDescription createVertexAttributeDescription();

		//	DESCRIPTOR SETS
		static VkDescriptorSetLayoutBinding createDescriptorSetLayout();
		static VkDescriptorSetLayoutCreateInfo createDescriptorLayoutInfo(VkDescriptorSetLayoutBinding layoutInfo);
		static VkDescriptorPoolCreateInfo createDescriptorPool();
		static std::vector<VkDescriptorSet> allocDescriptorSet(VkDevice device, VkDescriptorPool descriptorPool, VkDescriptorSetLayout layout, std::vector<VkDescriptorSet> descriptorSets);
		static VkWriteDescriptorSet createDescriptorSets(VkBuffer uniformBuffer, std::vector<VkDescriptorSet> descriptorSet, int imageIdx);

		//	PUSH CONSTANTS
		static VkPushConstantRange createPushConstantRange();
	};
}
