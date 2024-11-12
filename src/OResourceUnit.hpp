#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <SOIL2.h>
#include <GL/glew.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <glm/gtc/noise.hpp>
#include <map>
#include <filesystem>
#include <limits>
namespace fs = std::filesystem;
typedef std::map<std::string, int> BasePairMap;

namespace OResourceUnit
{
	typedef unsigned char byte;

	struct TAABB
	{
		aiVector3D min;
		aiVector3D max;
	};

	struct RenderContext
	{
		std::string filename;
		GLuint vertexArray;
		GLuint vertexBuffer;
		GLuint vertexIndexBuffer;
		int size = 0;
		TAABB _TAABB;

		void initFromAssimpMesh(aiMesh *mesh);
	};

	struct TextureContext
	{
		GLuint id;
		std::string filename;
	};

	static std::vector<RenderContext> models;
	static std::vector<TextureContext> textures;

	void DrawContext(RenderContext &context);
	void CalculateBoundingBox(const aiScene *scene, aiVector3D &min, aiVector3D &max);
	void loadModelToContext(std::string filename, OResourceUnit::RenderContext &context);
	void applyScalingToNode(const aiScene *scene, aiNode *node, const aiMatrix4x4 &scalingMatrix);

	GLuint LoadTexture(std::string filename);
	GLuint LoadCubeTexture(std::string foldername);

	// textureID - texture ID obtained from the LoadTexture function
	// shaderVariableName - name of the 'sampler2D' type variable in the shader to which the texture is to be associated
	// programID - identifier of the current graphics card program
	// textureUnit - index of the texturing unit - a number from 0 to 7. If you use multiple textures in one shader, each of them should be associated with a different unit.
	void SetActiveTexture(GLuint textureID, const char *shaderVariableName, GLuint programID, int textureUnit);
	void SetActiveCubeTexture(GLuint textureID, const char *shaderVariableName, GLuint programID, int textureUnit);
}