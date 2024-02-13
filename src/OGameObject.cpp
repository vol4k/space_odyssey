#include "OGameObject.hpp"

OGameObject::OGameObject(OShaderUnit* shader, std::string modelfile, std::string texturefile, std::string normalfile, glm::vec3 pos, glm::vec3 dir, float deceleration, float scale) 
:shader(shader) ,context(new OResourceUnit::RenderContext()) ,pos(pos) ,dir(dir), deceleration(deceleration), scale(scale), up(glm::vec3(0.f, 1.f, 0.f)), side(glm::vec3(0.f, 1.f, 0.f))
{
  OResourceUnit::loadModelToContext(modelfile, *context);
  texture = OResourceUnit::LoadTexture(texturefile);
  normal = OResourceUnit::LoadTexture(normalfile);
}

OGameObject::OGameObject(OShaderUnit* shader, std::string modelfile, std::string texturefolder, glm::vec3 pos, glm::vec3 dir) 
:shader(shader) ,context(new OResourceUnit::RenderContext()) ,pos(pos) ,dir(dir), up(glm::vec3(0.f, 1.f, 0.f)), side(glm::vec3(0.f, 1.f, 0.f))
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

glm::mat4 OGameObject::getModelMatrix(float time)
{
  return glm::eulerAngleY(time / deceleration) * 
        glm::translate(glm::mat4(1.f), pos) * 
				glm::scale(glm::mat4(1.f), glm::vec3(scale));
}