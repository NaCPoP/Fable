#include "fbpch.h"
#include "VulkanShader.h"

#include "VulkanBackend/VulkanUtilities.h"
#include "VulkanBackend/VulkanInitalizers.h"
#include "VulkanBackend/VulkanPipelineBuilder.h"

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
		VulkanPipelineBuilder pipelineBuilder;

		pipelineBuilder.m_ShaderStages.emplace_back(
			VulkanInitalizers::createShaderStageInfo(VK_SHADER_STAGE_VERTEX_BIT, m_VertShader)
		);

		pipelineBuilder.m_ShaderStages.emplace_back(
			VulkanInitalizers::createShaderStageInfo(VK_SHADER_STAGE_FRAGMENT_BIT, m_FragShader)
		);

		pipelineBuilder.m_VertexInputInfo = VulkanInitalizers::createVertexInputInfo();
		pipelineBuilder.m_InputAssemblyInfo = VulkanInitalizers::createInputAssemblyInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

		pipelineBuilder.m_Viewport = {
			0.0f,
			0.0f,
			static_cast<float>(m_Context->m_SwapchainExtent.width),
			static_cast<float>(m_Context->m_SwapchainExtent.height),
			0.0f,
			1.0f
		};

		pipelineBuilder.m_Scissor = {
			{0, 0},
			m_Context->m_SwapchainExtent
		};

		//vkCmdSetViewport(m_Context->m_CommandBuffer, 0, 1, &pipelineBuilder.m_Viewport);
		//vkCmdSetScissor(m_Context->m_CommandBuffer, 0, 1, &pipelineBuilder.m_Scissor);

		pipelineBuilder.m_RasterizationInfo = VulkanInitalizers::createRasterizationInfo(VK_POLYGON_MODE_FILL);
		pipelineBuilder.m_MultisampleInfo = VulkanInitalizers::createMultisampleInfo();
		pipelineBuilder.m_ColorBlendAttachment = VulkanInitalizers::createColorBlendAttachment();
		pipelineBuilder.m_PipelineLayout = m_PipelineLayout;

		m_Context->m_GraphicsPipeline = pipelineBuilder.createGraphicsPipeline(m_Context->m_Device, m_Context->m_RenderPass);

		vkDestroyShaderModule(m_Context->m_Device, m_VertShader, nullptr);
		vkDestroyShaderModule(m_Context->m_Device, m_FragShader, nullptr);
	}

	void VulkanShader::Unbind() const
	{
	}

	void VulkanShader::Load(const std::string&& vertexFilePath, const std::string&& fragmentFilePath)
	{
		VulkanUtilities::loadShaderModule(vertexFilePath, m_Context->m_Device, &m_VertShader);
		VulkanUtilities::loadShaderModule(fragmentFilePath, m_Context->m_Device, &m_FragShader);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = VulkanInitalizers::createPipelineLayout();
		if (vkCreatePipelineLayout(m_Context->m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout))
		{
			throw std::runtime_error("Failed to create pipeline layout");
		}
	}
}