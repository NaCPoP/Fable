#pragma once
#include "Fable/Renderer/Shader.h"

#include "VulkanMain.h"
#include "VulkanContext.h"
#include "VulkanBackend/VulkanPipelineBuilder.h"

namespace Fable
{
	class VulkanShader : public Shader
	{
	public:
		VulkanShader(GraphicsContext* context);
		~VulkanShader();
		 
		void Bind() const;
		void Unbind() const;
		void Load(const std::string&& vertexFilePath, const std::string&& fragmentFilePath) override;

	private:
		VulkanContext* m_Context{ nullptr };

		VkShaderModule m_VertShader;
		VkShaderModule m_FragShader;

		VkPipelineLayout m_PipelineLayout{ VK_NULL_HANDLE };
		VkPipeline m_Pipeline{ VK_NULL_HANDLE };
	};
}

