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

  OShaderUnit* shader;
  
  OGameObject* spaceship;

  void processInput();
  void renderScene();
  void initResources();
};