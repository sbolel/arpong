#pragma once

#include <cstdint>

// The width and height we are going to work at.  ESCAPI automatically scales
// larger images down to this.
enum { WIDTH = 800, HEIGHT = 600 };

struct rgb_byte { uint8_t r, g, b, a; };