#pragma once
#include "OShaderUnit.hpp"
#include "OResourceUnit.hpp"
#include <glm.hpp>

class OGameObject
{
  public:
  // for skybox object
  OGameObject(OShaderUnit* shader, std::string modelfile, std::string texturefolder, glm::vec3 pos, glm::vec3 dir);
  // for default object
  OGameObject(OShaderUnit* shader, std::string modelfile, std::string texturefile, std::string normalfile, glm::vec3 pos, glm::vec3 dir);
  ~OGameObject();

  void updateShader(OShaderUnit* shader);
  OResourceUnit::RenderContext& getContext();
  OShaderUnit& getShader();

  glm::vec3 pos;
  glm::vec3 dir;
  GLuint texture;
  GLuint normal;

  private:

  OShaderUnit* shader;
  OResourceUnit::RenderContext* context;
};