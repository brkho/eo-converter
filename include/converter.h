// This class defines a meshconverter. A converter can be instantiated and then input a .fbx or
// .obj mesh to convert. This will in turn invoke assimp to read the mesh and then store the parsed
// output in the engine's native Vertex form.
//
// Brian Ho (brian@brkho.com)

#ifndef CONVERTER_MESH_CONVERTER_H
#define CONVERTER_MESH_CONVERTER_H

#include <vector>

namespace converter {

// Abstraction that describes a vertex. This is adapted from the Vertex definition in the PBR
//engine.
struct Vertex {
  // Location of the vertex.
  float position[3];
  // Normal of the vertex.
  float normal[3];
  // Tangent of the vertex.
  float tangent[3];
  // UV coordinate of the vertex.
  float uv[2];
};

class MeshConverter {
  public:
    // The vertices of the mesh.
    std::vector<Vertex> vertices;
    // The index elements of the mesh.
    std::vector<unsigned int> indices;

    // Create a MeshConverter that has all of its fields initialized to empty.
    MeshConverter();

    // Loads a .fbx or .obj model into the MeshConverter and extracts the vertices and indices.
    // Throws if assimp cannot load the file.
    void Convert(std::string path);
};

}
#endif // CONVERTER_MESH_CONVERTER_H
