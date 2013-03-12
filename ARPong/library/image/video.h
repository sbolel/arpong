#pragma once

#include "../../ARPong.h"

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