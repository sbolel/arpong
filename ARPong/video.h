#pragma once

#include "ARPong.h"

#include <array>

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

class video_stream {
public:
	video_stream();
	~video_stream();

	rgb_byte get_pix(int x, int y) const;
	void set_pix(int x, int y, const rgb_byte& pix);
	bool next_frame();
	void cleanup();

	frame current_frame;

private:
	// whether the library has first been initialized (once per application)
	static bool escapi_initialized;

	// whether this video stream has been deinitialized
	bool deinitialized;

	// use first device by default
	enum { DEVICE = 0 };
};