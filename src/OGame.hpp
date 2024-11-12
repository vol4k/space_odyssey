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
  OShaderUnit* defaultShader;

  OGameObject* sun;

  OGameObject* skybox;
  OGameObject* spaceship;
  OGameObject* spaceshipEngine;
  OGameObject* goal;

  spheres planetStore;
  aster asteroidStore;

  int goalCounter = 15;

  float acceleration = 0.01f;
  glm::vec3 accelerationXYZ = {0.f, 0.f, 0.f};

  void processInput();
  void reloadShaders();
  void renderScene();
  void initResources();
  void gameOver();
  void gameWin();
  void collisionTest();
};