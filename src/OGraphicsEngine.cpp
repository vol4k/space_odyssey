#include "OGraphicsEngine.hpp"
#include <random>

OGraphicsEngine::OGraphicsEngine()
:OWindow(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
{
  glewInit();
  glViewport(0, 0, width, height);
  aspectRatio = width / float(height);

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  glEnable(GL_DEPTH_TEST);
}

OGraphicsEngine::~OGraphicsEngine()
{
  // TODO
}

void OGraphicsEngine::render()
{
  struct color {
    float r = (std::rand() % 100)/100.f;
    float g = (std::rand() % 100)/100.f;
    float b = (std::rand() % 100)/100.f;
  }clr;

  glClearColor(clr.r, clr.g, clr.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(0);
	glfwSwapBuffers(window);
}

void OGraphicsEngine::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}