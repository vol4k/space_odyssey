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
  OGameObject(std::string name, OShaderUnit *shader, std::string modelfile, std::string texturefolder, glm::vec3 pos, glm::vec3 dir);
  // for default object
  OGameObject(std::string name, OShaderUnit *shader, std::string modelfile, std::string texturefile, std::string normalfile, glm::vec3 pos, glm::vec3 dir, float deceleration = 1.f, float scale = 1.f, int shift = 1, bool isBall = false, float exposition = 1.f);
  ~OGameObject();

  bool collisionTest(OGameObject &obj);
  void updateShader(OShaderUnit *shader);
  OResourceUnit::RenderContext &getContext();
  OShaderUnit &getShader();
  glm::mat4 getModelMatrix(float time);

  aiVector3D Add(const aiVector3D &a, const glm::vec3 &b);
  aiVector3D Mul(const aiVector3D &a, const glm::vec3 &b);
  void updatePos(const glm::vec3 &pos);
  void updateDir(const glm::vec3 &dir);
  void updateUp(const glm::vec3 &up);
  void updateSide(const glm::vec3 &side);
  std::string getName();

  bool isBall;
  int shift;
  float exposition;
  std::string name;
  glm::vec3 pos;
  glm::vec3 currentPos;
  glm::vec3 dir;
  glm::vec3 up;
  glm::vec3 side;
  GLuint texture;
  GLuint normal;
  glm::vec3 scale;

private:
  float deceleration;
  OShaderUnit *shader;
  OResourceUnit::RenderContext *context;
};