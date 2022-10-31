#include <graphics/Camera.h>
//-------------------------------------------------------------------------------------------------
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

void Camera::setPosition(glm::vec3& p)
{
	m_position = p;

	updateViewMatrix();
}

//-------------------------------------------------------------------------------------------------

void Camera::setRotation(glm::vec3& r)
{
	m_rotation = r;

	updateViewMatrix();
}

//-------------------------------------------------------------------------------------------------

const glm::vec3& Camera::position() const
{
	return m_position;
}

//-------------------------------------------------------------------------------------------------

const glm::vec3& Camera::rotation() const
{
	return m_rotation;
}

//-------------------------------------------------------------------------------------------------

void Camera::makePerspectiveProjection(float fov, float aspect, float n, float f)
{
	m_projectionMatrix = glm::perspective(fov, aspect, n, f);
}

//-------------------------------------------------------------------------------------------------

void Camera::makeOrthogonalProjection(float l, float r, float t, float b, float n, float f)
{
	m_projectionMatrix = glm::ortho(l, r, t, b, n, f);
}

//-------------------------------------------------------------------------------------------------

const glm::mat4& Camera::projectionMatrix() const
{
	return m_projectionMatrix;
}

//-------------------------------------------------------------------------------------------------

const glm::mat4& Camera::viewMatrix() const
{
	return m_viewMatrix;
}

//-------------------------------------------------------------------------------------------------

void Camera::updateViewMatrix()
{
	m_viewMatrix = glm::mat4();
	glm::rotate(m_viewMatrix, -m_rotation.x, glm::vec3(1.0, 0.0, 0.0));
	glm::rotate(m_viewMatrix, -m_rotation.y, glm::vec3(0.0, 1.0, 0.0));
	glm::rotate(m_viewMatrix, -m_rotation.z, glm::vec3(0.0, 0.0, 1.0));
	glm::translate(m_viewMatrix, -m_position);
}

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
