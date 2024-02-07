#pragma once
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OCamera
{
  public:
  OCamera() = default;

  OCamera(glm::vec3 eye, glm::vec3 lookat, glm::vec3 upVector);

  glm::mat4x4 GetViewMatrix() const;
  glm::vec3 GetEye() const;
  glm::vec3 GetUpVector() const;
  glm::vec3 GetLookAt() const;

  // Camera forward is -z
  glm::vec3 GetViewDir() const;
  glm::vec3 GetRightVector() const;

  void SetCameraView(glm::vec3 eye, glm::vec3 lookat, glm::vec3 up);
  void UpdateViewMatrix();

  private:
  glm::mat4x4 m_viewMatrix;
  glm::vec3 m_eye; // Camera position in 3D
  glm::vec3 m_lookAt; // Point that the camera is looking at
  glm::vec3 m_upVector; // Orientation of the camera
};