#include <cmath>
#include <limits>
#include <memory>

// Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants

const double infinity = FLT_MAX;
const double pi = 3.141592;

// Utility Functions

inline double degrees_to_radians(double degrees) { return degrees * pi / 180.0; }
