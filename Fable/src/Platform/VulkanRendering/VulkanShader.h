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
		void Load(const std::string&& vertexFilePath, const std::string&& fragmentFilePath) override;
		void LoadUniformBuffer() override;

	private:
		VulkanPipelineBuilder* m_PipelineBuilder;
		VulkanContext* m_Context{ nullptr };

		VkShaderModule m_VertShader;
		VkShaderModule m_FragShader;

		VkPipelineLayout m_PipelineLayout{ VK_NULL_HANDLE };
		VkPipeline m_Pipeline{ VK_NULL_HANDLE };

		VkPipeline m_GraphicsPipeline;
		// ---------------
		// UNIFORM BUFFERS
		// ---------------
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		std::array<VkDescriptorSetLayout, 3> m_GlobalLayouts{};
		std::vector<VkDescriptorSet> m_GlobalDescriptorSets{};

		Fable::RendererAPI::global_ubo m_GlobalUbo;

		VkBuffer m_UniformBuffer;
		VkDeviceMemory m_UniformBufferMemory;
		void* m_UniformBuffersMapped;
	};
}

