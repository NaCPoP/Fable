#pragma once

#include "Fable/Layer.h"

#include "Fable/Events/ApplicationEvents.h"
#include "Fable/Events/MouseEvents.h"
#include "Fable/Events/KeyEvents.h"

namespace Fable
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}

