// This header defines exceptions used in the converter submodule.

#ifndef CONVERTER_EXCEPTIONS_H
#define CONVERTER_EXCEPTIONS_H

#include <exception>

namespace converter {

// When the converter cannot load a file via assimp.
class CannotLoadFileException : public std::exception {
  public:
    const char* what () const throw () {
      return "Cannot load file for converting.";
    }
};

// When an invalid material input map was given to the main program.
class InvalidMaterialInputMapException : public std::exception {
  public:
    const char* what () const throw () {
      return "Invalid input map wildcard format.";
    }
};

// When an invalid material map type is specified to the main program.
class InvalidMaterialTypeException : public std::exception {
  public:
    const char* what () const throw () {
      return "Invalid map type.";
    }
};

}
#endif // CONVERTER_EXCEPTIONS_H
