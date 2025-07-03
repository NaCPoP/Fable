#include "fbpch.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace Fable
{
	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float nearClip, float farClip)
		: m_ProjectionMatrix(glm::perspective(fov, aspect, nearClip, farClip)), m_ViewMatrix(1.0f), m_Fov(fov), m_Aspect(aspect)
	{

	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		// eye, center, up
		m_ViewMatrix = glm::lookAt(m_Position, m_Center, glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void PerspectiveCamera::RecalculateProjectionMatrix()
	{
		//fov, aspect ratio, near clipping, far clipping
		m_ProjectionMatrix = glm::perspective(m_Fov, m_Aspect, -1.0f, 1.0f);
	}
}