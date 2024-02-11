#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "OWindow.hpp"
#include "OResourceUnit.hpp"
#include "OShaderUnit.hpp"
#include "OGameObject.hpp"
#include "OCamera.hpp"
#include <glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <math.h>

union spheres {
    struct {
      OGameObject* mercury;
      OGameObject* venus;
      OGameObject* earth;
      OGameObject* mars;
      OGameObject* jupiter;
      OGameObject* saturn;
      OGameObject* uranus;
      OGameObject* neptune;
      OGameObject* pluto;
    } planet;
    OGameObject* planets[9];
  };

template <typename t> int sgn(t val) {
    return (t(0) < val) - (val < t(0));
}

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
  
  glm::vec3 reflectorColor = glm::vec3(0.3f, 0.f, 0.f);
  float reflectorAngle = glm::radians(25.f);
  float reflectorLightExp = 500;

  glm::vec2 lastMousePos = glm::vec2();
  glm::vec3 upVector; // Orientation of the camera

  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

  //glm::mat4 createCameraMatrix();
  glm::mat4 createPerspectiveMatrix();
  void updateCamera();
  void drawObjectColor(OGameObject& obj, glm::mat4 modelMatrix, glm::vec3 color);
  void drawObjectProc(OGameObject& obj, glm::mat4 modelMatrix);
  void drawObjectSkyBox(OGameObject& obj, glm::mat4 modelMatrix);

  public:
  OCamera camera;
  
  OGraphicsEngine();
  ~OGraphicsEngine();

  GLFWwindow* getWindow();

  void render(OGameObject& spaceship, spheres planetStore, OGameObject& sun, OGameObject& skybox);
};