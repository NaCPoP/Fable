#pragma once

#include "RendererAPI.h"

#include "Buffer.h"

namespace Fable
{
	class RenderCommand
	{
	public:
		inline static void SetContext(GraphicsContext* context)
		{
			s_RendererAPI->SetContext(context);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		inline static void WindowResize(uint32_t width, uint32_t height)
		{
			s_RendererAPI->WindowResize(width, height);
		}

		inline static void BeginRender()
		{
			s_RendererAPI->BeginRender();
		}

		inline static void EndRender()
		{
			s_RendererAPI->EndRender();
		}

		inline static void DrawIndexed(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
		{
			s_RendererAPI->DrawIndexed(vertexBuffer, indexBuffer);
		}
		
	private:
		static RendererAPI* s_RendererAPI;
	};
}