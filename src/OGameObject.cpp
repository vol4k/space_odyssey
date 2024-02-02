#include "OGameObject.hpp"

OGameObject::OGameObject(OShaderUnit* shader, glm::vec3 pos, glm::vec3 dir) 
:shader(shader) ,context(new ORenderUnit::RenderContext()) ,pos(pos) ,dir(dir) {}

OGameObject::~OGameObject() 
{
  delete context;
}

ORenderUnit::RenderContext& OGameObject::getContext()
{
  return *context;
}

OShaderUnit& OGameObject::getShader()
{
  return *shader;
}