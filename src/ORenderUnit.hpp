#pragma once
#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace ORenderUnit
{
  struct RenderContext
    {
		GLuint vertexArray;
		GLuint vertexBuffer;
		GLuint vertexIndexBuffer;
		int size = 0;

		void initFromAssimpMesh(aiMesh* mesh);
	};

	void DrawContext(RenderContext& context);
}