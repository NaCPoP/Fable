#include "fbpch.h"
#include "VulkanShader.h"

#include "VulkanBackend/VulkanUtilities.h"
#include "VulkanBackend/VulkanInitalizers.h"
#include "VulkanBackend/VulkanPipelineBuilder.h"
#include "Fable/Renderer/Renderer.h"

namespace Fable
{
	VulkanShader::VulkanShader(GraphicsContext* context)
		: m_Context(static_cast<VulkanContext*>(context))
	{
		
	}

	VulkanShader::~VulkanShader()
	{

	}

	void VulkanShader::Bind() const
	{
		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = (float)m_Context->m_SwapchainExtent.height;
		viewport.width = (float)m_Context->m_SwapchainExtent.width;
		viewport.height = -(float)m_Context->m_SwapchainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(m_Context->m_CommandBuffers[m_Context->m_CurrentFrame], 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_Context->m_SwapchainExtent;
		vkCmdSetScissor(m_Context->m_CommandBuffers[m_Context->m_CurrentFrame], 0, 1, &scissor);

		vkCmdBindPipeline(m_Context->m_CommandBuffers[m_Context->m_CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);

		vkCmdBindDescriptorSets(m_Context->m_CommandBuffers[m_Context->m_CurrentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, &m_DescriptorSets[0], 0, nullptr);

		vkCmdPushConstants(m_Context->m_CommandBuffers[m_Context->m_CurrentFrame], m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &m_GlobalUbo.model);
	}

	void VulkanShader::Unbind() const
	{
		vkDestroyShaderModule(m_Context->m_Device, m_VertShader, nullptr);
		vkDestroyShaderModule(m_Context->m_Device, m_FragShader, nullptr);
	}

	void VulkanShader::Load(const std::string vertexFilePath, const std::string fragmentFilePath)
	{
		m_PipelineBuilder = new(VulkanPipelineBuilder);

		VulkanUtilities::loadShaderModule(vertexFilePath, m_Context->m_Device, &m_VertShader);
		VulkanUtilities::loadShaderModule(fragmentFilePath, m_Context->m_Device, &m_FragShader);

		m_PipelineBuilder->m_ShaderStages.emplace_back(
			VulkanInitalizers::createShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT, m_VertShader)
		);

		m_PipelineBuilder->m_ShaderStages.emplace_back(
			VulkanInitalizers::createShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT, m_FragShader)
		);

		m_PipelineBuilder->m_RasterizationInfo		= VulkanInitalizers::createRasterizationInfo(VK_POLYGON_MODE_FILL);
		m_PipelineBuilder->m_MultisampleInfo		= VulkanInitalizers::createMultisampleInfo();
		m_PipelineBuilder->m_ColorBlendAttachment	= VulkanInitalizers::createColorBlendAttachment();

		
		//	VERTEX AND INDEX BUFFERS
		m_PipelineBuilder->m_BindingDescription		= VulkanInitalizers::createVertexBindingDescription();
		m_PipelineBuilder->m_AttribbuteDescription	= VulkanInitalizers::createVertexAttributeDescription();
		m_PipelineBuilder->m_VertexInputInfo		= VulkanInitalizers::createVertexInputInfo(m_PipelineBuilder->m_BindingDescription, m_PipelineBuilder->m_AttribbuteDescription);
		m_PipelineBuilder->m_InputAssemblyInfo		= VulkanInitalizers::createInputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

		//	PUSH CONSTANTS
		VkPushConstantRange pushConstant			= VulkanInitalizers::createPushConstantRange();

		//	DESCRIPTOR SETS
		m_DescriptorSetLayoutBinding				= VulkanInitalizers::createDescriptorSetLayout();
		m_DescriptorSetLayoutInfo					= VulkanInitalizers::createDescriptorLayoutInfo(m_DescriptorSetLayoutBinding);

		if (vkCreateDescriptorSetLayout(m_Context->m_Device, &m_DescriptorSetLayoutInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}

		//	PIPELINE LAYOUT CREATION
		VkPipelineLayoutCreateInfo pipelineLayoutInfo = VulkanInitalizers::createPipelineLayout(m_DescriptorSetLayout, pushConstant);
		if (vkCreatePipelineLayout(m_Context->m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout))
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}

		m_PipelineBuilder->m_PipelineLayout = m_PipelineLayout;

		m_GraphicsPipeline = m_PipelineBuilder->createGraphicsPipeline(m_Context->m_Device, m_Context->m_RenderPass);

		// UNIFORM BUFFERS
		VkDeviceSize bufferSize = sizeof(m_GlobalUbo);

		m_UniformBuffer = (VulkanUtilities::createBuffer(m_Context->m_Device, m_Context->m_PhysicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_UniformBufferMemory));

		vkMapMemory(m_Context->m_Device, m_UniformBufferMemory, 0, bufferSize, 0, &m_UniformBufferMapped);

		// DESCRIPTOR POOLS
		m_DescriptorPoolInfo = VulkanInitalizers::createDescriptorPool();

		if (vkCreateDescriptorPool(m_Context->m_Device, &m_DescriptorPoolInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}

		m_DescriptorSets = VulkanInitalizers::allocDescriptorSet(m_Context->m_Device, m_DescriptorPool, m_DescriptorSetLayout, m_DescriptorSets);
	}

	void VulkanShader::LoadUniformBuffer(glm::mat4 projection, glm::mat4 view, glm::mat4 model)
	{
		m_GlobalUbo.projection	= projection;
		m_GlobalUbo.view		= view;
		m_GlobalUbo.model		= model;

		memcpy(m_UniformBufferMapped, &m_GlobalUbo, sizeof(m_GlobalUbo));

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer				= m_UniformBuffer;
		bufferInfo.offset				= 0;
		bufferInfo.range				= sizeof(m_GlobalUbo);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet			= m_DescriptorSets[0];
		descriptorWrite.dstBinding		= 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo		= &bufferInfo;

		vkUpdateDescriptorSets(m_Context->m_Device, 1, &descriptorWrite, 0, nullptr);
	}
}