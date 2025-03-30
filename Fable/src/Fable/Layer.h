#pragma once

#include "Events/Events.h"

namespace Fable
{
	class Layer
	{
	public:
		Layer(const std::string& layerName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(Event& event) {};
		virtual void OnImGuiRender() {};

		inline std::string GetName() { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}