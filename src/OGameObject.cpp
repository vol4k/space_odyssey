#include "OGameObject.hpp"

OGameObject::OGameObject(OShaderUnit* shader, std::string modelfile, std::string texturefile, std::string normalfile, glm::vec3 pos, glm::vec3 dir) 
:shader(shader) ,context(new OResourceUnit::RenderContext()) ,pos(pos) ,dir(dir) 
{
  OResourceUnit::loadModelToContext(modelfile, *context);
  texture = OResourceUnit::LoadTexture(texturefile);
  normal = OResourceUnit::LoadTexture(normalfile);
}

OGameObject::OGameObject(OShaderUnit* shader, std::string modelfile, std::string texturefolder, glm::vec3 pos, glm::vec3 dir) 
:shader(shader) ,context(new OResourceUnit::RenderContext()) ,pos(pos) ,dir(dir) 
{
  OResourceUnit::loadModelToContext(modelfile, *context);
  texture = OResourceUnit::LoadCubeTexture(texturefolder);
}

OGameObject::~OGameObject() 
{
  delete context;
}

void OGameObject::updateShader(OShaderUnit* shader)
{
  this->shader = shader;
}

OResourceUnit::RenderContext& OGameObject::getContext()
{
  return *context;
}

OShaderUnit& OGameObject::getShader()
{
  return *shader;
}