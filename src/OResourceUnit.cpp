#include "OResourceUnit.hpp"

void OResourceUnit::RenderContext::initFromAssimpMesh(aiMesh *mesh)
{
    vertexArray = 0;
    vertexBuffer = 0;
    vertexIndexBuffer = 0;

    std::vector<float> textureCoord;
    std::vector<unsigned int> indices;
    // tex coord must be converted to 2d vecs
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        if (mesh->mTextureCoords[0] != nullptr)
        {
            textureCoord.push_back(mesh->mTextureCoords[0][i].x);
            textureCoord.push_back(mesh->mTextureCoords[0][i].y);
        }
        else
        {
            textureCoord.push_back(0.0f);
            textureCoord.push_back(0.0f);
        }
    }
    if (mesh->mTextureCoords[0] == nullptr)
    {
        std::cerr << "no uv coords" << std::endl;
    }
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    unsigned int vertexDataBufferSize = sizeof(float) * mesh->mNumVertices * 3;
    unsigned int vertexNormalBufferSize = sizeof(float) * mesh->mNumVertices * 3;
    unsigned int vertexTexBufferSize = sizeof(float) * mesh->mNumVertices * 2;
    unsigned int vertexTangentBufferSize = sizeof(float) * mesh->mNumVertices * 3;
    unsigned int vertexBiTangentBufferSize = sizeof(float) * mesh->mNumVertices * 3;

    unsigned int vertexElementBufferSize = sizeof(unsigned int) * indices.size();
    size = indices.size();

    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glGenBuffers(1, &vertexIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexElementBufferSize, &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // std::cout << vertexBuffer;
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glBufferData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize + vertexBiTangentBufferSize, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexDataBufferSize, mesh->mVertices);

    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize, vertexNormalBufferSize, mesh->mNormals);

    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize, vertexTexBufferSize, &textureCoord[0]);

    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize, vertexTangentBufferSize, mesh->mTangents);

    glBufferSubData(GL_ARRAY_BUFFER, vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize, vertexBiTangentBufferSize, mesh->mBitangents);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(static_cast<GLintptr>(0)));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(static_cast<GLintptr>(vertexDataBufferSize)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(static_cast<GLintptr>(vertexNormalBufferSize + vertexDataBufferSize)));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(static_cast<GLintptr>(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize)));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<void *>(static_cast<GLintptr>(vertexDataBufferSize + vertexNormalBufferSize + vertexTexBufferSize + vertexTangentBufferSize)));
}

void OResourceUnit::DrawContext(OResourceUnit::RenderContext &context)
{

    glBindVertexArray(context.vertexArray);
    glDrawElements(
        GL_TRIANGLES,    // mode
        context.size,    // count
        GL_UNSIGNED_INT, // type
        (void *)0        // element array buffer offset
    );
    glBindVertexArray(0);
}

void OResourceUnit::CalculateBoundingBox(const aiScene *scene, aiVector3D &min, aiVector3D &max)
{
    // Initialize min and max vectors
    min = aiVector3D(std::numeric_limits<float>::max());
    max = aiVector3D(std::numeric_limits<float>::lowest());

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[i];
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
        {
            aiVector3D vertex = mesh->mVertices[j];

            // Update the minimum values
            min.x = std::min(min.x, vertex.x);
            min.y = std::min(min.y, vertex.y);
            min.z = std::min(min.z, vertex.z);

            // Update the maximum values
            max.x = std::max(max.x, vertex.x);
            max.y = std::max(max.y, vertex.y);
            max.z = std::max(max.z, vertex.z);
        }
    }
}

void OResourceUnit::loadModelToContext(std::string filename, OResourceUnit::RenderContext &context)
{
    auto isExist = [filename](RenderContext m)
    {
        return m.filename == filename;
    };

    auto it = find_if(models.begin(), models.end(), isExist);

    if (it != models.end())
    {

        context = *it;
        return;
    }

    Assimp::Importer import;
    std::string filepath = "res/models/" + filename;
    const aiScene *scene = import.ReadFile(filepath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }

    CalculateBoundingBox(scene, context._TAABB.min, context._TAABB.max);

    aiNode *rootNode = scene->mRootNode;
    aiMatrix4x4 scalingMatrix;
    aiMatrix4x4::Scaling(aiVector3D(1.f, 1.f, 1.f), scalingMatrix);

    // Recursively traverse each node and apply scaling to vertices
    applyScalingToNode(scene, rootNode, scalingMatrix);

    context.initFromAssimpMesh(scene->mMeshes[0]);

    models.push_back({filename,
                      context.vertexArray,
                      context.vertexBuffer,
                      context.vertexIndexBuffer,
                      context.size,
                      context._TAABB});
}

// Recursively apply scaling to all vertices of the model
void OResourceUnit::applyScalingToNode(const aiScene *scene, aiNode *node, const aiMatrix4x4 &scalingMatrix)
{
    // Apply scaling to vertices of the current node
    for (unsigned int i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
        {
            mesh->mVertices[j] *= scalingMatrix;
        }
    }
}

GLuint OResourceUnit::LoadTexture(std::string filename)
{
    auto isExist = [filename](TextureContext t)
    {
        return t.filename == filename;
    };

    auto it = find_if(textures.begin(), textures.end(), isExist);

    if (it != textures.end())
        return it->id;

    std::string filepath = "res/textures/" + filename;

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    int w, h;
    unsigned char *image = SOIL_load_image(filepath.c_str(), &w, &h, 0, SOIL_LOAD_RGBA);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);

    textures.push_back({id, filename});

    return id;
}

GLuint OResourceUnit::LoadCubeTexture(std::string foldername)
{
    std::string folderpath = "res/textures/" + foldername + '/';

    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);

    BasePairMap cubemap;

    cubemap["ft"] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
    cubemap["bk"] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
    cubemap["up"] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
    cubemap["dn"] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
    cubemap["rt"] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
    cubemap["lf"] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

    int w, h;

    for (const auto &entry : fs::directory_iterator(folderpath))
    {
        unsigned char *image = SOIL_load_image(entry.path().c_str(), &w, &h, 0, SOIL_LOAD_RGBA);

        std::string filename = entry.path().filename().string();
        int loc = filename.find_last_of('_');

        if (loc == std::string::npos)
            return -1;
        std::string key = filename.substr(loc + 1, 2);

        glTexImage2D(cubemap[key], 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        SOIL_free_image_data(image);
    }

    return id;
}

void OResourceUnit::SetActiveTexture(GLuint textureID, const char *shaderVariableName, GLuint programID, int textureUnit)
{
    glUniform1i(glGetUniformLocation(programID, shaderVariableName), textureUnit);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void OResourceUnit::SetActiveCubeTexture(GLuint textureID, const char *shaderVariableName, GLuint programID, int textureUnit)
{
    glUniform1i(glGetUniformLocation(programID, shaderVariableName), textureUnit);
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
}