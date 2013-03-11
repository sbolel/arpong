#pragma once

#include "ARPong.h"

class video_stream {
public:
	uint32_t buffer[WIDTH * HEIGHT];
	rgb_byte get_pix(int x, int y) const;
	void set_pix(int x, int y, const rgb_byte& pix);
	bool next_frame();

	video_stream();
	~video_stream();

private:
	// whether the library has first been initialized (once per application)
	static bool escapi_initialized;

	// use first device by default
	enum { DEVICE = 0 };
};

inline uint32_t* get_pix_ptr(uint32_t* base, int x, int y) {
	return &base[(WIDTH * (HEIGHT - y) + x)];
}