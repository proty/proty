#ifndef PYRITE_PARSEERROR_HH
#define PYRITE_PARSEERROR_HH

#include "parser/location.hh"

namespace pyrite {
  
  /// Represents a parse error.
  /// This class will be thrown
  /// if the parser find a syntax
  /// error.
  class ParseError {
    private:
      Location* loc;
      std::string message;

    public:
      ParseError(Location* loc, std::string message)
        : loc(loc), message(message) { }

      /// Prints out an error message.
      void alert();
  };

}

#endif