// This is the entry point of the Engine Object converter.
// Usage: ./eo-converter input_mesh output_path input_map_matcher space_separated_list_of_map_types
// where valid map types are: albedo, specular, gloss, normal, ior, and ao. For example,
// ./eo-converter lamp.fbx lamp.eo assets/lamp_*.png albedo, specular, gloss, normal
// which will create lamp.eo using the geometry found in lamp.fbx. The materials will use the
// imput map matcher, substituting strings like 'albedo', 'specular', and 'gloss' for the wildcard
// character * in the input map matcher. In this case, we will use materials corresponding to
// assets/lamp_albedo.png, assets/lamp_gloss.png, etc.
// Brian Ho (brian@brkho.com)

#include "converter.h"
#include "exceptions.h"

#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::vector<std::string> MAP_TYPES {"albedo", "specular", "gloss", "ior", "normal", "ao"};

void PrintError(const char* msg) {
  std::cerr << "ERROR: " << msg << std::endl;
}

void WriteMaterialInfo(std::ostream* output_file, std::string input_map_matcher, int argc,
    char* argv[]) {
  // Get position of wildcard character.
  size_t wildcard_pos = input_map_matcher.find("*");
  if (wildcard_pos == std::string::npos ||
      input_map_matcher.find("*", wildcard_pos + 1) != std::string::npos) {
    throw converter::InvalidMaterialInputMapException();
  }

  // Find which maps to link. O(n^2), but sue me.
  int num_linked = 0;
  for (size_t i = 0; i < MAP_TYPES.size(); ++i) {
    bool found = false;
    for (int j = 4; j < argc; ++j) {
      std::string type {argv[j]};
      if (type == MAP_TYPES[i]) {
        // Replace the wildcard in the map matcher with the map type.
        std::string map_path{input_map_matcher};
        map_path.replace(wildcard_pos, 1, type);
        // Additional byte so we can copy over the terminating null byte.
        (*output_file) << (char)(map_path.size() + 1) << map_path << (char)(0);
        found = true;
        num_linked++;
        break;
      }
    }
    if (!found) {
      (*output_file) << (char)(0);
    }
  }
  if (num_linked != argc - 4) {
    throw converter::InvalidMaterialTypeException();
  }
}

void WriteMeshInfo(std::ostream* output_file, std::string mesh_path) {
  converter::MeshConverter mesh_converter;
  mesh_converter.Convert(mesh_path);
  size_t num_vertices = mesh_converter.vertices.size();
  (*output_file).write((char*)(&num_vertices), sizeof(num_vertices));
  (*output_file).write((char*)mesh_converter.vertices.data(),
      num_vertices * sizeof(converter::Vertex));
  size_t num_indices = mesh_converter.indices.size();
  (*output_file).write((char*)(&num_indices), sizeof(num_indices));
  (*output_file).write((char*)mesh_converter.indices.data(), num_indices * sizeof(unsigned int));
}

// Main point of entry for the code.
int main(int argc, char* argv[]) {
  if (argc < 5 && argc != 3) {
    PrintError("Invalid command line arguments.");
    return EXIT_FAILURE;
  }

  // Read command line arguments.
  std::string mesh_path{argv[1]};
  std::string output_path{argv[2]};
  std::ofstream output_file {output_path, std::ios::out};

  // For now, always use normalized Blinn-Phong because other shader types are not yet implemented.
  output_file << (char)(0);

  // Short form.
  if (argc == 3) {
    for (size_t i = 0; i < MAP_TYPES.size(); ++i) {
      output_file << (char)(0);
    }
  } else {
    std::string input_map_matcher{argv[3]};
    try {
      // Dangerous passing around file handle here, but oh well.
      WriteMaterialInfo(&output_file, input_map_matcher, argc, argv);
    } catch (const std::exception& e) {
      PrintError(e.what());
      output_file.close();
      return EXIT_FAILURE;
    }
  }

  try {
    // Get vertex and index data from mesh. Again, dangerous passing around file handle...
    WriteMeshInfo(&output_file, mesh_path);
  } catch (const std::exception& e) {
    PrintError(e.what());
    output_file.close();
    return EXIT_FAILURE;
  }

  output_file.close();
  return EXIT_SUCCESS;
}
