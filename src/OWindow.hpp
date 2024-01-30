#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class OWindow
{
  private:
  int status = 0;

  int width;
  int height;

  float aspectRatio = 1.f;
  GLFWwindow* window;
  GLuint program;
  GLuint programSun;
  //Core::Shader_Loader shaderLoader;

  static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  void shutdown(GLFWwindow* window);

  public:
  OWindow(int width, int height);
  ~OWindow();
  void init();
  void renderLoop();
};