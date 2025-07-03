#pragma once
#include "glm/glm.hpp"

namespace Fable
{
	class PerspectiveCamera
	{
	public:
		PerspectiveCamera(float fov, float aspect, float nearClip = -1.0f, float farClip = 1.0f);

		void SetAspect(float aspect) { m_Aspect = aspect; RecalculateProjectionMatrix(); }

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPostition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		void SetCenter(const glm::vec3 center) { m_Center = center; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

	private:
		void RecalculateViewMatrix();
		void RecalculateProjectionMatrix();

	private:
		glm::mat4	m_ProjectionMatrix;
		glm::mat4	m_ViewMatrix;

		float		m_Fov;
		float		m_Aspect;
		glm::vec3	m_Center;

		glm::vec3	m_Position;
		float		m_Rotation = 0.0f;
	};
}

