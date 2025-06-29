#include "fbpch.h"
#include "VulkanInitalizers.h"
#include "Fable/Renderer/RendererAPI.h"
#include "glm/glm.hpp"

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

	VkPipelineVertexInputStateCreateInfo VulkanInitalizers::createVertexInputInfo(VkVertexInputBindingDescription bindingDescription, 
																				  VkVertexInputAttributeDescription attributeDescription)
	{
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType							= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount	= 1;
		vertexInputInfo.pVertexBindingDescriptions		= &bindingDescription;
		vertexInputInfo.vertexAttributeDescriptionCount = 1;
		vertexInputInfo.pVertexAttributeDescriptions	= &attributeDescription;

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

	VkVertexInputBindingDescription VulkanInitalizers::createVertexBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescriptions{};
		bindingDescriptions.binding		= 0;
		bindingDescriptions.stride		= sizeof(glm::vec3);
		bindingDescriptions.inputRate	= VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescriptions;
	}

	VkVertexInputAttributeDescription VulkanInitalizers::createVertexAttributeDescription()
	{
		VkVertexInputAttributeDescription attributeDescriptions{};
		attributeDescriptions.binding	= 0;
		attributeDescriptions.location	= 0;
		attributeDescriptions.format	= VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions.offset	= 0;

		return attributeDescriptions;
	}

	VkDescriptorSetLayoutBinding VulkanInitalizers::createDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding globalUboDescriptorSetLayout{};
		globalUboDescriptorSetLayout.binding			= 0;
		globalUboDescriptorSetLayout.descriptorCount	= 1;
		globalUboDescriptorSetLayout.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		globalUboDescriptorSetLayout.pImmutableSamplers = 0;
		globalUboDescriptorSetLayout.stageFlags			= VK_SHADER_STAGE_VERTEX_BIT;

		return globalUboDescriptorSetLayout;
	}

	VkDescriptorSetLayoutCreateInfo VulkanInitalizers::createDescriptorLayoutInfo(VkDescriptorSetLayoutBinding layoutInfo)
	{
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
		descriptorSetLayoutInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.bindingCount	= 1;
		descriptorSetLayoutInfo.pBindings		= &layoutInfo;

		return descriptorSetLayoutInfo;
	}

	VkDescriptorPoolCreateInfo VulkanInitalizers::createDescriptorPool(uint32_t imageCount)
	{
		VkDescriptorPoolSize poolSize{};
		poolSize.type				= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount	= imageCount;

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType				= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount		= 1;
		poolInfo.pPoolSizes			= &poolSize;
		poolInfo.maxSets			= imageCount;

		return poolInfo;
	}

	VkDescriptorSetAllocateInfo VulkanInitalizers::allocDescriptorSet(VkDescriptorPool descriptorPool, std::array<VkDescriptorSetLayout, 3> layouts)
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorSetCount = 3;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.pSetLayouts = layouts.data();

		return allocInfo;
	}

	VkWriteDescriptorSet VulkanInitalizers::createDescriptorSets(VkBuffer uniformBuffer, std::vector<VkDescriptorSet> descriptorSet, int imageIdx)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = uniformBuffer;
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(Fable::RendererAPI::global_ubo);

		VkWriteDescriptorSet descriptorWrites{};
		descriptorWrites.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites.dstSet = descriptorSet[imageIdx];
		descriptorWrites.dstBinding = 0;
		descriptorWrites.dstArrayElement = 0;
		descriptorWrites.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites.descriptorCount = 1;
		descriptorWrites.pBufferInfo = &bufferInfo;

		return descriptorWrites;
	}
}