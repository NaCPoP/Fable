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
	}

	void VulkanShader::Unbind() const
	{
		vkDestroyShaderModule(m_Context->m_Device, m_VertShader, nullptr);
		vkDestroyShaderModule(m_Context->m_Device, m_FragShader, nullptr);
	}

	void VulkanShader::Load(const std::string&& vertexFilePath, const std::string&& fragmentFilePath)
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

		m_PipelineBuilder->m_BindingDescription = VulkanInitalizers::createVertexBindingDescription();
		m_PipelineBuilder->m_AttribbuteDescription = VulkanInitalizers::createVertexAttributeDescription();
		m_PipelineBuilder->m_VertexInputInfo = VulkanInitalizers::createVertexInputInfo(m_PipelineBuilder->m_BindingDescription,
			m_PipelineBuilder->m_AttribbuteDescription);

		m_PipelineBuilder->m_InputAssemblyInfo = VulkanInitalizers::createInputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

		m_PipelineBuilder->m_RasterizationInfo = VulkanInitalizers::createRasterizationInfo(VK_POLYGON_MODE_FILL);
		m_PipelineBuilder->m_MultisampleInfo = VulkanInitalizers::createMultisampleInfo();
		m_PipelineBuilder->m_ColorBlendAttachment = VulkanInitalizers::createColorBlendAttachment();

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = VulkanInitalizers::createPipelineLayout();
		if (vkCreatePipelineLayout(m_Context->m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout))
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}

		m_PipelineBuilder->m_PipelineLayout = m_PipelineLayout;

		m_GraphicsPipeline = m_PipelineBuilder->createGraphicsPipeline(m_Context->m_Device, m_Context->m_RenderPass);
	}

	void VulkanShader::LoadUniformBuffer()
	{

	}
}