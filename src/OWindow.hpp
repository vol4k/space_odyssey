#pragma once
#include <stdexcept>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define WINDOW_TITLE "Space Odysey"
#define DEFAULT_WINDOW_WIDTH 1280
#define DEFAULT_WINDOW_HEIGHT 720

class OWindow
{
  protected:
  int viewportWidth;
  int viewportHeight;

  float aspectRatio = 1.f;
  GLFWwindow* window;
  
  public:
  OWindow(int width, int height);
  ~OWindow();
  bool windowShouldClose();
};