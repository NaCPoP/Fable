#pragma once
#include "glm/glm.hpp"
#include "GraphicsContext.h"
#include "Buffer.h"

namespace Fable
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None	= 0,
			OpenGL	= 1,
			Vulkan	= 2
		};

		typedef struct UniformBufferObject
		{
			glm::mat4 projection;
			glm::mat4 view;
			glm::mat4 model;
		} global_ubo;

	public:
		virtual void SetContext(GraphicsContext* context) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void WindowResize(uint32_t width, uint32_t height) = 0;

		virtual void BeginRender() = 0;
		virtual void EndRender() = 0;

		virtual void DrawIndexed(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer) = 0;

		inline static API GetAPI() { return s_RendererAPI; }

	private:
		static API s_RendererAPI;
	};
}