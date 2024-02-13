#include "OGameObject.hpp"

OGameObject::OGameObject(OShaderUnit* shader, std::string modelfile, std::string texturefile, std::string normalfile, glm::vec3 pos, glm::vec3 dir, float deceleration, float scale) 
:shader(shader) ,context(new OResourceUnit::RenderContext()) ,pos(pos), realPos(pos) ,dir(dir), deceleration(deceleration), scale(scale), up(glm::vec3(0.f, 1.f, 0.f)), side(glm::vec3(0.f, 1.f, 0.f))
{
  OResourceUnit::loadModelToContext(modelfile, *context);
  texture = OResourceUnit::LoadTexture(texturefile);
  normal = OResourceUnit::LoadTexture(normalfile);
}

OGameObject::OGameObject(OShaderUnit* shader, std::string modelfile, std::string texturefolder, glm::vec3 pos, glm::vec3 dir) 
:shader(shader) ,context(new OResourceUnit::RenderContext()) ,pos(pos), realPos(pos) ,dir(dir), up(glm::vec3(0.f, 1.f, 0.f)), side(glm::vec3(0.f, 1.f, 0.f))
{
  OResourceUnit::loadModelToContext(modelfile, *context);
  texture = OResourceUnit::LoadCubeTexture(texturefolder);
}

OGameObject::~OGameObject() 
{
  delete context;
}

bool OGameObject::checkCollision(OGameObject& obj)
{
  if(obj.show)
  {
    glm::vec3 distance = this->pos - obj.realPos;
    glm::vec3 sc = (abs(scale) - abs(obj.scale))/2.f;

    return (
      abs(distance.x) < sc.x &&
      abs(distance.y) < sc.y &&
      abs(distance.z) < sc.z
      );
  }
  return false;
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
  glm::mat4 matrix = glm::eulerAngleY(time / deceleration) * 
        glm::translate(glm::mat4(1.f), pos) * 
				glm::scale(glm::mat4(1.f), glm::vec3(scale));
  realPos = glm::vec3(matrix[3]);
  return matrix;
}