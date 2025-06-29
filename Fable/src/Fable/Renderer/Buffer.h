#pragma once
#include "Fable/Core.h"

#include "Fable/Renderer/GraphicsContext.h"
#include "glm/glm.hpp"

namespace Fable
{
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexBuffer* Create(GraphicsContext* context, glm::vec3* verticies, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IndexBuffer* Create(GraphicsContext* context, uint32_t* indices, uint32_t count);
	};
}