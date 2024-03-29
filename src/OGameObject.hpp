#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "OShaderUnit.hpp"
#include "OResourceUnit.hpp"
#include <glm.hpp>
#include <glm/gtx/euler_angles.hpp>

class OGameObject
{
  public:
  // for skybox object
  OGameObject(OShaderUnit* shader, std::string modelfile, std::string texturefolder, glm::vec3 pos, glm::vec3 dir);
  // for default object
  OGameObject(OShaderUnit* shader, std::string modelfile, std::string texturefile, std::string normalfile, glm::vec3 pos, glm::vec3 dir, float deceleration=1.f, float scale=1.f);
  ~OGameObject();

  bool checkCollision(OGameObject& obj);
  void updateShader(OShaderUnit* shader);
  OResourceUnit::RenderContext& getContext();
  OShaderUnit& getShader();
  glm::mat4 getModelMatrix(float time);

  bool show = true;

  glm::vec3 realPos;
  glm::vec3 pos;
  glm::vec3 dir;
  glm::vec3 up;
  glm::vec3 side;
  GLuint texture;
  GLuint normal;

  private:

  glm::vec3 scale;
  float deceleration;
  OShaderUnit* shader;
  OResourceUnit::RenderContext* context;
};