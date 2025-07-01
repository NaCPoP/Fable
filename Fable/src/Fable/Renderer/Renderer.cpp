#include "fbpch.h"
#include "Renderer.h"

namespace Fable
{
	void Renderer::WindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::WindowResize(width, height);
	}

	void Renderer::BeginScene(GraphicsContext* context)
	{
		RenderCommand::BeginRender();
	}

	void Renderer::EndScene()
	{
		RenderCommand::EndRender();
	}

	void Renderer::Submit(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
	{
		RenderCommand::DrawIndexed(vertexBuffer, indexBuffer);
	}
}