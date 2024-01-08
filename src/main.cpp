#include <iostream>
#include "preferences.hpp"
#include <gelf.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

int main()
{
  std::cout << "Hello world!" << std::endl;
  
  return 0;
}

/*
int main()
{
  GLFWwindow* window;

  if (!glfwInit())
  {
    return -1;
  }

  window = glfwCreateWindow(window::height, window::width, window::title, NULL, NULL);
  glfwMakeContextCurrent(window);


  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Couldn't load opengl" << std::endl;
    glfwTerminate();
    return -1;
  }

  glClearColor(window::c.r,window::c.g,window::c.b,window::c.a);

  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();

    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(window);
  }

  return 0;
}
*/