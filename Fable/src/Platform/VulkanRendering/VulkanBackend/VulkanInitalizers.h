#pragma once
#include "vulkan/vulkan.h"


namespace Fable
{
	class VulkanInitalizers
	{
	public:
		/*
		* GRAPHICS PIPELINE
		*/
		static VkPipelineShaderStageCreateInfo createShaderStageInfo(VkShaderStageFlagBits stage, VkShaderModule module);
		static VkPipelineVertexInputStateCreateInfo createVertexInputInfo();
		static VkPipelineInputAssemblyStateCreateInfo createInputAssemblyInfo(VkPrimitiveTopology topology);
		static VkPipelineRasterizationStateCreateInfo createRasterizationInfo(VkPolygonMode polygonMode);
		static VkPipelineMultisampleStateCreateInfo createMultisampleInfo();
		static VkPipelineColorBlendAttachmentState createColorBlendAttachment();
		static VkPipelineLayoutCreateInfo createPipelineLayout();
	private:
	};
}
