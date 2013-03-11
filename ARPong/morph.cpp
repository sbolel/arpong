#include "morph.h"

#include <memory>

enum { SQUARE_SIZE = 2 };

void erode_pixel(frame& img, const frame& orig, int x0, int y0) {
	uint8_t* pix = img.get_pixel(x0, y0);
	if(!is_black(pix)) { 
		for(int x = x0 - SQUARE_SIZE; x <= x0 + SQUARE_SIZE; ++x) {
			for(int y = y0 - SQUARE_SIZE; y <= y0 + SQUARE_SIZE; ++y) {
				if(is_black(orig.get_pixel(x, y))) { 
					pix[0] = pix[1] = pix[2] = 0;
					return;
				}
			}
		}
	}
}

void erode(frame& img) {
	std::unique_ptr<frame> orig(new frame(img));
	for(int y0 = SQUARE_SIZE; y0 < HEIGHT - SQUARE_SIZE; ++y0) {
		for(int x0 = SQUARE_SIZE; x0 < WIDTH - SQUARE_SIZE; ++x0) {
			erode_pixel(img, *orig, x0, y0);
		}
	}
}