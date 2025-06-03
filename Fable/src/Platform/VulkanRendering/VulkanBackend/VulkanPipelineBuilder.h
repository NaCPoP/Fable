#pragma once
#include "vulkan/vulkan.h"

namespace Fable
{
	class VulkanPipelineBuilder
	{
	public:
		/*
		* GRAPHICS PIPELINE
		*/
		VkViewport								m_Viewport;
		VkRect2D								m_Scissor;

		VkPipelineVertexInputStateCreateInfo	m_VertexInputInfo;
		VkPipelineInputAssemblyStateCreateInfo	m_InputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo	m_RasterizationInfo;
		VkPipelineMultisampleStateCreateInfo	m_MultisampleInfo;
		VkPipelineColorBlendAttachmentState		m_ColorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo		m_ColorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo	m_DepthStencilInfo;

		VkPipelineLayout	m_PipelineLayout = nullptr;
		uint32_t			m_Subpass = 0;

		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
		VkPipeline m_GraphicsPipeline;

	public:
		VkPipeline createGraphicsPipeline(VkDevice device, VkRenderPass renderPass);
	};
}

