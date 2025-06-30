#pragma once

#include "GraphicsContext.h"

#include "glm/glm.hpp"
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

		virtual void LoadUniformBuffer(glm::mat4 porjection, glm::mat4 view, glm::mat4 model) = 0;

		static Shader* Create(GraphicsContext* context);
	};
}

