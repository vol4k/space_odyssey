#include "OWindow.hpp"

OWindow::OWindow(int width=DEFAULT_WINDOW_WIDTH, int height=DEFAULT_WINDOW_HEIGHT)
{
  this->width=width;
  this->height=height;

  // glfw initialization
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


  // create window using glfw
  window = glfwCreateWindow(width, height, WINDOW_TITLE, NULL, NULL);
  if (window == NULL)
  {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }
  glfwMakeContextCurrent(window);
}

OWindow::~OWindow()
{
  glfwTerminate();
}

bool OWindow::windowShouldClose()
{
  return glfwWindowShouldClose(window);
}