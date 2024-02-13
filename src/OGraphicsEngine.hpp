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

union aster {
    struct {
      OGameObject* _1;
      OGameObject* _2;
      OGameObject* _3;
      OGameObject* _4;
      OGameObject* _5;
      OGameObject* _6;
    } asteroid;
    OGameObject* asteroids[9];
  };

union goal {
    struct {
      OGameObject* _1;
      OGameObject* _2;
      OGameObject* _3;
      OGameObject* _4;
      OGameObject* _5;
      OGameObject* _6;
      OGameObject* _7;
      OGameObject* _8;
      OGameObject* _9;
      OGameObject* _10;
      OGameObject* _11;
      OGameObject* _12;
      OGameObject* _13;
      OGameObject* _14;
      OGameObject* _15;
    } gold;
    OGameObject* gold_arr[15];
  };

template <typename t> int sgn(t val) {
    return (t(0) < val) - (val < t(0));
}

class OGraphicsEngine: private OWindow
{
  private:
  glm::vec3 lightPos = glm::vec3(0, 0, 0);
  glm::vec3 lightColor = glm::vec3(0.9, 0.7, 0.8)*100.f;
  glm::vec3 sunDir = glm::vec3(0.9, 0.3, 0.003);
  glm::vec3 sunColor = glm::vec3(0.9f, 0.9f, 0.7f)*5;

  glm::vec3 spotlightPos = glm::vec3(0, 0, 0);
  glm::vec3 spotlightConeDir = glm::vec3(0, 0, 0);
  glm::vec3 spotlightColor = glm::vec3(0.5, 0.9, 0.8)*10.f;
  float spotlightPhi = 3.14 / 3;
  
  glm::vec3 reflectorColor = glm::vec3(0.3f, 0.f, 0.f);
  float reflectorAngle = glm::radians(25.f);
  float reflectorLightExp = 100;

  glm::vec2 lastMousePos = glm::vec2();
  glm::vec3 upVector; // Orientation of the camera

  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

  //glm::mat4 createCameraMatrix();
  glm::mat4 createPerspectiveMatrix();
  void updateCamera();
  void drawObjectPBR(OGameObject& obj, glm::mat4 modelMatrix, float roughness, float metallic, float exposition);
  void drawObjectSkyBox(OGameObject& obj, glm::mat4 modelMatrix, float exposition);

  public:
  int viewportWidth;
  int viewportHeight;

  OCamera camera;
  float fov = 45.f;
  
  OGraphicsEngine();
  ~OGraphicsEngine() = default;

  GLFWwindow* getWindow();

  void render(OGameObject& spaceship, spheres planetStore,aster asteroidStore, goal goldStore, OGameObject& sun, OGameObject& skybox);
};