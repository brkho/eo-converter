// This is the entry point of the Engine Object converter.
// Brian Ho (brian@brkho.com)

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>

// Main point of entry for the code.
int main(int argc, char* argv[]) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile("placeholder", aiProcess_Triangulate | aiProcess_FlipUVs |
      aiProcess_GenNormals | aiProcess_CalcTangentSpace);
  if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    std::cerr << "ERROR: " << importer.GetErrorString() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Finished!" << std::endl;
  return EXIT_SUCCESS;
}
