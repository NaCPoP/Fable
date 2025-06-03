#include "fbpch.h"
#include "VulkanPipelineBuilder.h"

namespace Fable
{
	/*
	* GRAPHICS PIPELINE
	*/
	VkPipeline VulkanPipelineBuilder::createGraphicsPipeline(VkDevice device, VkRenderPass renderPass)
	{
		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType			= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount	= 1;
		viewportInfo.pViewports		= &m_Viewport;
		viewportInfo.scissorCount	= 1;
		viewportInfo.pScissors		= &m_Scissor;

		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		colorBlendInfo.sType				= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.logicOpEnable		= VK_FALSE;
		colorBlendInfo.logicOp				= VK_LOGIC_OP_COPY;
		colorBlendInfo.attachmentCount		= 1;
		colorBlendInfo.pAttachments			= &m_ColorBlendAttachment;
		colorBlendInfo.blendConstants[0]	= 0.0f;		// optional
		colorBlendInfo.blendConstants[1]	= 0.0f;		// optional
		colorBlendInfo.blendConstants[2]	= 0.0f;		// optional
		colorBlendInfo.blendConstants[3]	= 0.0f;		// optional

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType					= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount				= m_ShaderStages.size();
		pipelineInfo.pStages				= m_ShaderStages.data();
		pipelineInfo.pVertexInputState		= &m_VertexInputInfo;
		pipelineInfo.pInputAssemblyState	= &m_InputAssemblyInfo;
		pipelineInfo.pViewportState			= &viewportInfo;
		pipelineInfo.pRasterizationState	= &m_RasterizationInfo;
		pipelineInfo.pMultisampleState		= &m_MultisampleInfo;
		//pipelineInfo.pDepthStencilState	= &m_DepthStencilInfo;
		pipelineInfo.pColorBlendState		= &colorBlendInfo;
		pipelineInfo.pDynamicState			= nullptr;
		pipelineInfo.layout					= m_PipelineLayout;
		pipelineInfo.renderPass				= renderPass;
		pipelineInfo.subpass				= 0;
		pipelineInfo.basePipelineHandle		= VK_NULL_HANDLE;
		//pipelineInfo.basePipelineIndex	= -1;

		VkPipeline newPipeline;
		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline!");
		}

		return newPipeline;
	}
}