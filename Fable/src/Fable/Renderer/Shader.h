#pragma once

#include "GraphicsContext.h"
#include <any>

namespace Fable
{
	class Shader
	{
	public:
		virtual ~Shader() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void Load(const std::string&& vertexFilePath, const std::string&& fragmentFilePath) = 0;

		virtual void LoadUniformBuffer() = 0;

		static Shader* Create(GraphicsContext* context);
	};
}

