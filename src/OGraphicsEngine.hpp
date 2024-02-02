#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "OWindow.hpp"
#include "ORenderUnit.hpp"
#include "OShaderUnit.hpp"
#include "OGameObject.hpp"
#include <glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <assimp/Importer.hpp>

class OGraphicsEngine: private OWindow
{
  private:

  float exposition = 1.f;
  
  glm::vec3 lightPos = glm::vec3(-8, 4, 2);
  glm::vec3 lightColor = glm::vec3(0.9, 0.7, 0.8)*100.f;

  glm::vec3 spotlightPos = glm::vec3(0, 0, 0);
  glm::vec3 spotlightConeDir = glm::vec3(0, 0, 0);
  glm::vec3 spotlightColor = glm::vec3(0.5, 0.9, 0.8)*10.f;
  float spotlightPhi = 3.14 / 3;

  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

  glm::mat4 createCameraMatrix();
  glm::mat4 createPerspectiveMatrix();
  void drawObjectColor(OGameObject& obj, glm::mat4 modelMatrix, glm::vec3 color);

  public:
  glm::vec3 cameraPos = glm::vec3(-4.f, 0, 0);
  glm::vec3 cameraDir = glm::vec3(1.f, 0.f, 0.f);
  
  OGraphicsEngine();
  ~OGraphicsEngine();

  GLFWwindow* getWindow();

  void render(OGameObject& spaceship);
  static void loadModelToContext(std::string path, ORenderUnit::RenderContext& context);
};