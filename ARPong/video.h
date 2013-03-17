#pragma once

#include "ARPong.h"

class video_stream {
public:
	video_stream();
	~video_stream();

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