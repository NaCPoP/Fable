#pragma once
#include "Fable/Renderer/Shader.h"

#include "VulkanMain.h"
#include "VulkanContext.h"
#include "VulkanBackend/VulkanPipelineBuilder.h"
#include "Fable/Renderer/RendererAPI.h"

namespace Fable
{
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(GraphicsContext* context);
		~VulkanShader();
		 
		void Bind() const override;
		void Unbind() const override;
		void Load(const std::string vertexFilePath, const std::string fragmentFilePath) override;
		void LoadUniformBuffer(glm::mat4 projection, glm::mat4 view, glm::mat4 model) override;

	private:
		VulkanPipelineBuilder*			m_PipelineBuilder;
		VulkanContext*					m_Context{ nullptr };

		VkShaderModule					m_VertShader;
		VkShaderModule					m_FragShader;

		VkPipelineLayout				m_PipelineLayout{ VK_NULL_HANDLE };
		VkPipeline						m_GraphicsPipeline;

		// DESCRIPTOR SETS
		VkDescriptorPoolCreateInfo		m_DescriptorPoolInfo;
		VkDescriptorPool				m_DescriptorPool;

		VkDescriptorSetAllocateInfo		m_PoolAllocInfo;
		std::vector<VkDescriptorSet>	m_DescriptorSets;
		VkDescriptorSetLayoutBinding	m_DescriptorSetLayoutBinding;
		VkDescriptorSetLayoutCreateInfo	m_DescriptorSetLayoutInfo;
		VkDescriptorSetLayout			m_DescriptorSetLayout;

		// UNIFORM BUFFERS
		Fable::RendererAPI::global_ubo	m_GlobalUbo;

		VkBuffer						m_UniformBuffer;
		VkDeviceMemory					m_UniformBufferMemory;
		void*							m_UniformBufferMapped;
	};
}

