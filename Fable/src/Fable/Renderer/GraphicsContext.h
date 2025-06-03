#pragma once

#include "Fable/Core.h"

namespace Fable
{
	struct RendererSettings
	{
		std::string applicationName;
	};

	class GraphicsContext
	{
	public:
		virtual void Init(RendererSettings settings) = 0;
		virtual void Shutdown() = 0;
		virtual void SwapBuffers() = 0;
		virtual void Draw() = 0;
	};
}