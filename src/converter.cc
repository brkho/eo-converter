#include "converter.h"
#include "exceptions.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>

converter::MeshConverter::MeshConverter() : vertices{std::vector<converter::Vertex>()},
    indices{std::vector<unsigned int>()} {}

void converter::MeshConverter::Convert(std::string path) {
  // Clear vertices and indices.
  vertices = std::vector<converter::Vertex>();
  indices = std::vector<unsigned int>();

  // Do the importy!
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
      aiProcess_GenNormals | aiProcess_CalcTangentSpace);
  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    throw converter::CannotLoadFileException();
  }

  for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
    const aiMesh* assimp_mesh = scene->mMeshes[i];
    // The mesh must have texture coordinates. Maybe I can investigate removing this constraint
    // later?
    if (!assimp_mesh->HasTextureCoords(0)) {
      throw converter::NoTextureCoordinatesException();
    }
    for (unsigned int j = 0; j < assimp_mesh->mNumVertices; ++j) {
      converter::Vertex vertex;
      vertex.position[0] = assimp_mesh->mVertices[j].x;
      vertex.position[1] = assimp_mesh->mVertices[j].y;
      vertex.position[2] = assimp_mesh->mVertices[j].z;
      vertex.normal[0] = assimp_mesh->mNormals[j].x;
      vertex.normal[1] = assimp_mesh->mNormals[j].y;
      vertex.normal[2] = assimp_mesh->mNormals[j].z;
      vertex.tangent[0] = assimp_mesh->mTangents[j].x;
      vertex.tangent[1] = assimp_mesh->mTangents[j].y;
      vertex.tangent[2] = assimp_mesh->mTangents[j].z;
      vertex.uv[0] = assimp_mesh->mTextureCoords[0][j].x;
      vertex.uv[1] = assimp_mesh->mTextureCoords[0][j].y;
      vertices.push_back(vertex);
    }

    for (unsigned int j = 0; j < assimp_mesh->mNumFaces; ++j) {
      aiFace assimp_face = assimp_mesh->mFaces[j];
      for (unsigned int k = 0; k < assimp_face.mNumIndices; k++) {
        indices.push_back(assimp_face.mIndices[k]);
      }
    }
  }
}
