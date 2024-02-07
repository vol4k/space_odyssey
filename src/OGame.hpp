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

  OShaderUnit* skyboxShader;
  OShaderUnit* spaceshipShader;
  OShaderUnit* sphereShader;
  
  OGameObject* skybox;
  OGameObject* spaceship;
  OGameObject* sphere;

  void processInput();
  void reloadShaders();
  void renderScene();
  void initResources();
};