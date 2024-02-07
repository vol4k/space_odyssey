#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <SOIL2.h>
#include <GL/glew.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <map>
#include <filesystem>
namespace fs = std::filesystem;
typedef std::map<std::string, int> BasePairMap;

namespace OResourceUnit
{
	typedef unsigned char byte;

  struct RenderContext
    {
		GLuint vertexArray;
		GLuint vertexBuffer;
		GLuint vertexIndexBuffer;
		int size = 0;

		void initFromAssimpMesh(aiMesh* mesh);
	};

	void DrawContext(RenderContext& context);
	void loadModelToContext(std::string filename, OResourceUnit::RenderContext& context);

	GLuint LoadTexture(std::string filename);
	GLuint LoadCubeTexture(std::string foldername);

	// textureID - texture ID obtained from the LoadTexture function
	// shaderVariableName - name of the 'sampler2D' type variable in the shader to which the texture is to be associated
	// programID - identifier of the current graphics card program
	// textureUnit - index of the texturing unit - a number from 0 to 7. If you use multiple textures in one shader, each of them should be associated with a different unit.
	void SetActiveTexture(GLuint textureID, const char * shaderVariableName, GLuint programID, int textureUnit);
	void SetActiveCubeTexture(GLuint textureID, const char * shaderVariableName, GLuint programID, int textureUnit);
}