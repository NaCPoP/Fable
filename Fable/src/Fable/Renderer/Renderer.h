#pragma once
#include "glm/glm.hpp"

#include "RenderCommand.h"

namespace Fable
{
	class Renderer
	{
	public:
		static void BeginScene(GraphicsContext* context);
		static void EndScene();

		static void Submit(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}