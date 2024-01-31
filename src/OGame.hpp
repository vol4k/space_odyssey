#pragma once

#include <iostream>
#include "OGraphicsEngine.hpp"

class OGame
{
  public:
  OGame(int width, int height);
  ~OGame();
  
  int run();

  private:
  bool is_ok;
  OGraphicsEngine* engine;

  void processInput();
  void renderScene();
};