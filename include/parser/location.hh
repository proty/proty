#ifndef PYRITE_LOCATION_HH
#define PYRITE_LOCATION_HH

#include <iostream>

namespace pyrite {

  /// Holds the location (filename, linenumber)
  /// of a token.
  class Location {
    private:
      std::string source;
      int lineno;

    public:
      Location(std::string, int);

      /// @returns the line number.
      int getLineNumber();
      /// Increases the line number by 1.
      void incLineNumber();
      /// @returns the source file name.
      std::string getSource();

      /// @returns a copy of this Location.
      Location* copy();
  };
  
}

#endif