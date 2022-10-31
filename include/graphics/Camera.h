//-------------------------------------------------------------------------------------------------
#ifndef DKE_Graphics_Camera_h
#define DKE_Graphics_Camera_h
//-------------------------------------------------------------------------------------------------
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
//-------------------------------------------------------------------------------------------------
namespace dke {
namespace graphics {
//-------------------------------------------------------------------------------------------------

class Camera 
{
public:
	Camera() = default;
	~Camera() = default;

	void makePerspectiveProjection(float fov, float aspect, float n, float f);
	void makeOrthogonalProjection(float l, float r, float t, float b, float n, float f);

	void setPosition(glm::vec3& p);
	void setRotation(glm::vec3& r);
	const glm::vec3& position() const;
	const glm::vec3& rotation() const;

	const glm::mat4& projectionMatrix() const;
	const glm::mat4& viewMatrix() const;

private:
	void updateViewMatrix();

private:
	glm::vec3 m_position;
	glm::vec3 m_rotation;

	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
};

//-------------------------------------------------------------------------------------------------
} // namespace graphics
} // namespace dke
//-------------------------------------------------------------------------------------------------
#endif // DKE_Graphics_Camera_h
