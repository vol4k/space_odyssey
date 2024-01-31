#pragma once
#include "OWindow.hpp"

class OGraphicsEngine: private OWindow
{
  private:
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

  public:
  OGraphicsEngine();
  ~OGraphicsEngine();

  void render();
};