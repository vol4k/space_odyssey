#pragma once
#include "OShaderUnit.hpp"
#include "ORenderUnit.hpp"
#include <glm.hpp>

class OGameObject
{
  public:
  OGameObject(OShaderUnit* shader, glm::vec3 pos, glm::vec3 dir);
  ~OGameObject();

  ORenderUnit::RenderContext& getContext();
  OShaderUnit& getShader();

  glm::vec3 pos;
  glm::vec3 dir;

  private:
  OShaderUnit* shader;
  ORenderUnit::RenderContext* context;
};