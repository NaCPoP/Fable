#include "fbpch.h"
#include "Camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace Fable
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float nearClip, float farClip)
		: m_ProjectionMatrix(glm::perspective(fov, aspect, nearClip, farClip)), m_ViewMatrix(1.0f), m_Fov(fov), m_Aspect(aspect)
	{

	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		// eye, center, up
		m_ViewMatrix = glm::translate(glm::mat4{ 1.0f }, m_Position) * glm::eulerAngleXYZ(0.0f, 0.0f, 0.0f);
		
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	void PerspectiveCamera::RecalculateProjectionMatrix()
	{
		//fov, aspect ratio, near clipping, far clipping
		m_ProjectionMatrix = glm::perspective(m_Fov, m_Aspect, 0.1f, 1000.0f);
	}
}