#pragma once

#include "Fable/Core.h"
#include <Fable/Window.h>

namespace Fable
{
	struct RendererSettings
	{
		std::string applicationName;
	};

	class GraphicsContext
	{
	public:
		static GraphicsContext* Create(Window* window, RendererSettings settings);
		virtual void Shutdown() = 0;
		virtual void SwapBuffers() = 0;
	};
}