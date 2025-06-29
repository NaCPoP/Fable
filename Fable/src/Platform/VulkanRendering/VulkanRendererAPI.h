#pragma once
#include "Fable/Renderer/RendererAPI.h"
#include "VulkanContext.h"

namespace Fable
{
	class VulkanRendererAPI : public RendererAPI
	{
	public:
		void SetContext(GraphicsContext* context) override;
		void SetClearColor(const glm::vec4& color) override;

		void BeginRender() override;
		void EndRender() override;

		void DrawIndexed(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) override;

	private:
		VulkanContext* m_Context;

		VkClearValue m_ClearColor;

		// FRAME INFO
		const uint8_t m_MaxFramesInFlight = 2;
		uint32_t m_CurrentFrame = 0;
	};
}
