#pragma once

#include <cstdint>
#include <array>

// The width and height we are going to work at.  ESCAPI automatically scales
// larger images down to this.
enum { WIDTH = 1024, HEIGHT = 768 };

// Represent a pixel as a quad in rgba order.  Note that this is not the layout
// of the video input data
struct rgb_byte { uint8_t r, g, b, a; };

class frame {
	std::array<uint32_t, WIDTH * HEIGHT> buffer;

public:
	// Returns a pointer to the beginning of a pixel in memory.
	// Pixels are laid out in BGRA format.  A is not defined.
	uint8_t* get_pixel(int x, int y) {
		return reinterpret_cast<uint8_t*>(&buffer[WIDTH * (HEIGHT - y) + x]);
	}

	// Allow const methods to get a read-only copy of the data
	const uint8_t* get_pixel(int x, int y) const {
		return const_cast<frame*>(this)->get_pixel(x, y);
	}

	// Return a pointer to the underlying data.  Be careful with this!
	void* get_buffer_data() {
		return &buffer[0];
	}
};