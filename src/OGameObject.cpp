#include "OGameObject.hpp"

OGameObject::OGameObject(std::string name, OShaderUnit *shader, std::string modelfile, std::string texturefile, std::string normalfile, glm::vec3 pos, glm::vec3 dir, float deceleration, float scale, int shift, bool isBall, float exposition)
    : name(name), shader(shader), context(new OResourceUnit::RenderContext()), pos(pos), currentPos(pos), dir(dir), deceleration(deceleration), scale(scale), up(glm::vec3(0.f, 1.f, 0.f)), side(glm::vec3(0.f, 1.f, 0.f)), shift(shift), isBall(isBall), exposition(exposition)
{
  OResourceUnit::loadModelToContext(modelfile, *context);
  texture = OResourceUnit::LoadTexture(texturefile);
  normal = OResourceUnit::LoadTexture(normalfile);
}

OGameObject::OGameObject(std::string name, OShaderUnit *shader, std::string modelfile, std::string texturefolder, glm::vec3 pos, glm::vec3 dir)
    : name(name), shader(shader), context(new OResourceUnit::RenderContext()), pos(pos), currentPos(pos), dir(dir), up(glm::vec3(0.f, 1.f, 0.f)), side(glm::vec3(0.f, 1.f, 0.f)), shift(1), isBall(false)
{
  OResourceUnit::loadModelToContext(modelfile, *context);
  texture = OResourceUnit::LoadCubeTexture(texturefolder);
}

OGameObject::~OGameObject()
{
  delete context;
}

void OGameObject::updatePos(const glm::vec3 &pos)
{
  this->pos = pos;
  this->currentPos = pos;
}
void OGameObject::updateDir(const glm::vec3 &dir)
{
  this->dir = dir;
}
void OGameObject::updateUp(const glm::vec3 &up)
{
  this->up = up;
}
void OGameObject::updateSide(const glm::vec3 &side)
{
  this->side = side;
}

aiVector3D OGameObject::Add(const aiVector3D &a, const glm::vec3 &b)
{
  return aiVector3D(a.x + b.x, a.y + b.y, a.z + b.z);
}

aiVector3D OGameObject::Mul(const aiVector3D &a, const glm::vec3 &b)
{
  return aiVector3D(a.x * b.x, a.y * b.y, a.z * b.z);
}

bool OGameObject::collisionTest(OGameObject &obj)
{
  auto tBox1_min = Add(Mul(this->context->_TAABB.min, this->scale), this->currentPos);
  auto tBox1_max = Add(Mul(this->context->_TAABB.max, this->scale), this->currentPos);

  if (obj.isBall)
  {
    auto radius = Mul(obj.context->_TAABB.min, obj.scale);
    auto diff = obj.currentPos - this->currentPos;

    auto clamped = glm::clamp(diff, glm::vec3(tBox1_min.x, tBox1_min.y, tBox1_min.z), glm::vec3(tBox1_max.x, tBox1_max.y, tBox1_max.z));
    auto closest = this->currentPos + clamped;
    diff = closest - obj.currentPos;
    return glm::length(diff) < glm::length(glm::vec3(radius.x, radius.y, radius.z));
  }
  else
  {
    auto tBox2_min = Add(Mul(obj.context->_TAABB.min, obj.scale), obj.currentPos);
    auto tBox2_max = Add(Mul(obj.context->_TAABB.max, obj.scale), obj.currentPos);

    return (tBox1_max.x > tBox2_min.x &&
            tBox1_min.x < tBox2_max.x &&
            tBox1_max.y > tBox2_min.y &&
            tBox1_min.y < tBox2_max.y &&
            tBox1_max.z > tBox2_min.z &&
            tBox1_min.z < tBox2_max.z);
  }
}

void OGameObject::updateShader(OShaderUnit *shader)
{
  this->shader = shader;
}

OResourceUnit::RenderContext &OGameObject::getContext()
{
  return *context;
}

OShaderUnit &OGameObject::getShader()
{
  return *shader;
}

glm::mat4 OGameObject::getModelMatrix(float time)
{
  glm::mat4 matrix = glm::eulerAngleY((time + shift) / this->deceleration) *
                     glm::translate(glm::mat4(1.f), this->pos) *
                     glm::scale(glm::mat4(1.f), glm::vec3(this->scale));

  this->currentPos = glm::vec3(matrix[3]);

  return matrix;
}