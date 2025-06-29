#include "fbpch.h"
#include "VulkanPipelineBuilder.h"
#include "VulkanUtilities.h"

namespace Fable
{
	/*
	* GRAPHICS PIPELINE
	*/
	VkPipeline VulkanPipelineBuilder::createGraphicsPipeline(VkDevice device, VkRenderPass renderPass)
	{
		std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.sType				= VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.dynamicStateCount	= static_cast<uint32_t>(dynamicStates.size());
		dynamicStateInfo.pDynamicStates		= dynamicStates.data();

		VkPipelineViewportStateCreateInfo viewportInfo{};
		viewportInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount			= 1;
		viewportInfo.scissorCount			= 1;

		VkPipelineDepthStencilStateCreateInfo depthStencil{};
		depthStencil.sType					= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable		= VK_TRUE;
		depthStencil.depthWriteEnable		= VK_TRUE;
		depthStencil.depthCompareOp			= VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable	= VK_FALSE;
		depthStencil.stencilTestEnable		= VK_FALSE;

		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		colorBlendInfo.sType				= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.logicOpEnable		= VK_FALSE;
		colorBlendInfo.logicOp				= VK_LOGIC_OP_COPY;
		colorBlendInfo.attachmentCount		= 1;
		colorBlendInfo.pAttachments			= &m_ColorBlendAttachment;

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType					= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount				= m_ShaderStages.size();
		pipelineInfo.pStages				= m_ShaderStages.data();
		pipelineInfo.pVertexInputState		= &m_VertexInputInfo;
		pipelineInfo.pInputAssemblyState	= &m_InputAssemblyInfo;
		pipelineInfo.pViewportState			= &viewportInfo;
		pipelineInfo.pRasterizationState	= &m_RasterizationInfo;
		pipelineInfo.pMultisampleState		= &m_MultisampleInfo;
		pipelineInfo.pDepthStencilState		= &depthStencil;
		pipelineInfo.pColorBlendState		= &colorBlendInfo;
		pipelineInfo.pDynamicState			= &dynamicStateInfo;
		pipelineInfo.layout					= m_PipelineLayout;
		pipelineInfo.renderPass				= renderPass;
		pipelineInfo.subpass				= 0;
		pipelineInfo.basePipelineHandle		= VK_NULL_HANDLE;

		VkPipeline newPipeline;
		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create graphics pipeline!");
		}

		return newPipeline;
	}
}