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
  aster asteroidStore;
  goal goldStore;

  int goalCounter = 15;

  float acceleration = 0.01f;

  void processInput();
  void reloadShaders();
  void renderScene();
  void initResources();
  void gameOver();
  void gameWin();
  void checkCollision();
};