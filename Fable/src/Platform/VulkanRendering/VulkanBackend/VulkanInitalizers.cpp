#include "fbpch.h"
#include "VulkanInitalizers.h"

namespace Fable
{
	/*
	* GRAPHICS PIPELINE
	*/
	VkPipelineShaderStageCreateInfo VulkanInitalizers::createShaderStageInfo(VkShaderStageFlagBits stage, VkShaderModule module)
	{
		VkPipelineShaderStageCreateInfo shaderStageInfo{};
		shaderStageInfo.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.stage	= stage;
		shaderStageInfo.module	= module;
		shaderStageInfo.pName	= "main";

		return shaderStageInfo;
	}

	VkPipelineVertexInputStateCreateInfo VulkanInitalizers::createVertexInputInfo()
	{
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType							= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount	= 0;
		vertexInputInfo.pVertexBindingDescriptions		= nullptr;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions	= nullptr;

		return vertexInputInfo;
	}

	VkPipelineInputAssemblyStateCreateInfo VulkanInitalizers::createInputAssemblyInfo(VkPrimitiveTopology topology)
	{
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		inputAssemblyInfo.sType						= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology					= topology;
		inputAssemblyInfo.primitiveRestartEnable	= VK_FALSE;

		return inputAssemblyInfo;
	}

	VkPipelineRasterizationStateCreateInfo VulkanInitalizers::createRasterizationInfo(VkPolygonMode polygonMode)
	{
		VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
		rasterizationInfo.sType						= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationInfo.depthClampEnable			= VK_FALSE;
		rasterizationInfo.rasterizerDiscardEnable	= VK_FALSE;
		rasterizationInfo.polygonMode				= polygonMode;
		rasterizationInfo.lineWidth					= 1.0f;
		rasterizationInfo.cullMode					= VK_CULL_MODE_BACK_BIT;
		rasterizationInfo.frontFace					= VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizationInfo.depthBiasEnable			= VK_FALSE;
		rasterizationInfo.depthBiasConstantFactor	= 0.0f;		// optional
		rasterizationInfo.depthBiasClamp			= 0.0f;		// optional
		rasterizationInfo.depthBiasSlopeFactor		= 0.0f;		// optional

		return rasterizationInfo;
	}

	VkPipelineMultisampleStateCreateInfo VulkanInitalizers::createMultisampleInfo()
	{
		VkPipelineMultisampleStateCreateInfo multisampleCreateinfo{};
		multisampleCreateinfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleCreateinfo.sampleShadingEnable	= VK_FALSE;
		multisampleCreateinfo.rasterizationSamples	= VK_SAMPLE_COUNT_1_BIT;
		multisampleCreateinfo.minSampleShading		= 1.0f;
		multisampleCreateinfo.pSampleMask			= nullptr;
		multisampleCreateinfo.alphaToCoverageEnable = VK_FALSE;
		multisampleCreateinfo.alphaToOneEnable		= VK_FALSE;

		return multisampleCreateinfo;
	}

	VkPipelineColorBlendAttachmentState VulkanInitalizers::createColorBlendAttachment()
	{
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.blendEnable			= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendAttachment.colorWriteMask			= VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
													  VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable			= VK_FALSE;
		colorBlendAttachment.srcColorBlendFactor	= VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstColorBlendFactor	= VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.colorBlendOp			= VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor	= VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor	= VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp			= VK_BLEND_OP_ADD;

		return colorBlendAttachment;
	}

	VkPipelineLayoutCreateInfo VulkanInitalizers::createPipelineLayout()
	{
		VkPipelineLayoutCreateInfo pipelineLayout{};
		pipelineLayout.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayout.flags					= 0;
		pipelineLayout.setLayoutCount			= 0;
		pipelineLayout.pSetLayouts				= nullptr;
		pipelineLayout.pushConstantRangeCount	= 0;
		pipelineLayout.pPushConstantRanges		= nullptr;

		return pipelineLayout;
	}
}