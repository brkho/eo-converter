# Engine Object (.eo) Converter

## About
This is a converter for a custom model format (.ro) used in [my physically based real-time renderer](https://github.com/brkho/real-time-pbr) that defines a list of vertices, their indices, paths to texture maps, and material metadata. This format is optimized for rapid consumption by the PBR renderer as the vertex/index section of the format is essentially the engine's in-memory representation of the model and can be directly read in to an array of Vertex structs in a single disk read.

The current format is as follows:

- [1 BYTE]      An enum that describes which shader to use.

  - 0: Normalized Blinn-Phong (albedo, specular, gloss)

  - 1: [UNIMPLEMENTED] Cook-Torrance w/ GGX (albedo, specular, gloss, IOR)

  - 2: [UNIMPLEMENTED] Ashikhmin-Shirley (albedo, specular gloss)

- [1 BYTE]      Number of characters in the path to the albedo map (0 means no map).

- [N BYTES]     The (null-terminated) cstring containing the path to the albedo map.

- [1 BYTE]      Number of characters in the path to the specular map (0 means no map).

- [N BYTES]     The (null-terminated) cstring containing the path to the specular map.

- [1 BYTE]      Number of characters in the path to the gloss map (0 means no map).

- [N BYTES]     The (null-terminated) cstring containing the path to the gloss map.

- [1 BYTE]      Number of characters in the path to the IOR map (0 means no map).

- [N BYTES]     The (null-terminated) cstring containing the path to the IOR map.

- [1 BYTE]      Number of characters in the path to the normal map (0 means no map).

- [N BYTES]     The (null-terminated) cstring containing the path to the normal map.

- [4 BYTES]     Number of vertices in the model.

- [N BYTES]     The data corresponding to the vertices in the model. See the engine's Vertex struct for exact internal representation.

- [4 BYTES]     Number of indices in the model.

- [N BYTES]     The data corresponding to the indices in the model. This is an array of 32 bit ints.

## Usage
./eo-converter input_file output_file

and then follow the on-screen instructions to define your material.

## Dependencies
This project depends on:

- assimp

## Contact
Brian Ho

brian@brkho.com
