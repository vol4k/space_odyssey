#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

class OShaderUnit
{
	private:
  GLuint program;

  std::string ReadShader(char *filename);
  GLuint CreateShader(GLenum shaderType, std::string source, char* shaderName); // contain throws

	public:
  OShaderUnit(char* vertexShaderFilename, char* fragmentShaderFilename); // contain throws
  ~OShaderUnit();

  GLuint& getProgram();
	};