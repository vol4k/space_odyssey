#include "OCamera.hpp"

OCamera::OCamera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector)
: m_eye(std::move(eye))
, m_lookAt(std::move(lookat))
, m_upVector(std::move(upVector))
{
  UpdateViewMatrix();
}

glm::mat4 OCamera::GetViewMatrix() const { return m_viewMatrix;  }
glm::vec3 OCamera::GetEye() const { return m_eye; }
glm::vec3 OCamera::GetUpVector() const { return m_upVector; }
glm::vec3 OCamera::GetLookAt() const { return m_lookAt; }

glm::vec3 OCamera::GetViewDir() const { return -glm::transpose(m_viewMatrix)[2]; }
glm::vec3 OCamera::GetRightVector() const { return glm::transpose(m_viewMatrix)[0]; }

void OCamera::SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up)
{
    m_eye = std::move(eye);
    m_lookAt = std::move(lookat);
    m_upVector = std::move(up);
    UpdateViewMatrix();
}

void OCamera::UpdateViewMatrix()
{
    // Generate view matrix using the eye, lookAt and up vector
    m_viewMatrix = glm::lookAt(m_eye, m_lookAt, m_upVector);
}