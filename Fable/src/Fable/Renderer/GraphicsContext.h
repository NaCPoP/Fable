#pragma once

#include "Fable/Core.h"

namespace Fable
{
	class GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}