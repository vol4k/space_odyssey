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

  std::string ReadShader(std::string filename);
  GLuint CreateShader(GLenum shaderType, std::string source, std::string shaderName); // contain throws

	public:
  OShaderUnit(std::string vertexShaderFilename, std::string fragmentShaderFilename); // contain throws
  ~OShaderUnit();

  GLuint& getProgram();
	};