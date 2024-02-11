#pragma once
#include <iostream>
#include "OGraphicsEngine.hpp"
#include "OGameObject.hpp"

class OGame
{
  public:
  OGame(int width, int height);
  ~OGame();
  
  int run();

  private:
  bool is_ok;
  OGraphicsEngine* engine;

  OShaderUnit* sunShader;
  OShaderUnit* skyboxShader;
  OShaderUnit* spaceshipShader;
  OShaderUnit* sphereShader;

  OGameObject* sun;
  OGameObject* skybox;
  OGameObject* spaceship;

  spheres planetStore;

  float acceleration = 0.01f;

  void processInput();
  void reloadShaders();
  void renderScene();
  void initResources();
};