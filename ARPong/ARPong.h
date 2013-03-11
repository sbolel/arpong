#pragma once

#include <cstdint>

// The width and height we are going to work at.  ESCAPI automatically scales
// larger images down to this.
enum { WIDTH = 1024, HEIGHT = 768 };

// Represent a pixel as a quad in rgba order.  Note that this is not the layout
// of the video input data
struct rgb_byte { uint8_t r, g, b, a; };