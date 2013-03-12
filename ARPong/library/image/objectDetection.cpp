#include "objectDetection.h"

glm::ivec2 calculate_median(const frame& img) {
	double x_sum = 0.0, y_sum = 0.0;
	int pixels = 0;
	for(int x = 0; x < WIDTH; ++x) {
		for(int y = 0; y < HEIGHT; ++y) {
			const uint8_t* pix = img.get_pixel(x, y);
			if(!is_black(pix)) {
				x_sum += x;
				y_sum += y;
				++pixels;
			}
		}
	}

	return glm::ivec2(x_sum / pixels, y_sum / pixels);
}