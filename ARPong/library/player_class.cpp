#include "player_class.h"

player_class::~player_class(void) {}

player_class::player_class(void) { reset(); }

player_class::player_class(int player_id)
{
	reset();
	// switch (player_id)
	// {
	// 	case 1:
	// 		id = 1;
	// 		pos_max[0] = 5.0;
	// 		pos_max[1] = 0.0;
	// 		pos_max[2] = 0.0;
	// 		pos_min[0] = -5.0;
	// 		pos_min[1] = 0.0;
	// 		pos_min[2] = 0.0;
	// 		break;
	// 	case 2:
	// 		id = 2;
	// 		pos_max[0] = 5.0;
	// 		pos_max[1] = 0.0;
	// 		pos_max[2] = 0.0;
	// 		pos_min[0] = -5.0;
	// 		pos_min[1] = 0.0;
	// 		pos_min[2] = 0.0;
	// 		break;
	// 	default:
	// 		break;
	// }
}

void player_class::reset(void)
{
	x = 0.0;
	z = 0.0;
}

void player_class::Tx(void)
{
	glTranslated(x, 0.0, 0.0);
}

void player_class::Txz(void)
{
	glTranslated(x, 0.0, z);
}



// void player_class::setPos(float x) {

// }

// void player_class::drawPaddle(void)
// {

// }