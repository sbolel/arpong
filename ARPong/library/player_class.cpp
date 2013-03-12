#include "player_class.h"

player_class::~player_class(void) {}

player_class::player_class(void) { reset(); }

player_class::player_class(int player_id)
{
	reset();
	switch (player_id)
	{
		case 1:
			id = 1;
			x_max = ARENA_WIDTH;
			x_min = -ARENA_WIDTH;
			break;
		case 2:
			id = 2;
			x_max = -ARENA_WIDTH;
			x_min = ARENA_WIDTH;
			break;
		default:
			break;
	}
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

void player_class::xInc(GLfloat pos)
{
	GLfloat x_next = x + pos;
	if ((x_next) < x_max)
		x = x_next;
}

void player_class::xDec(GLfloat pos)
{
	GLfloat x_next = x - pos;
	if (x_next > x_min)
		x = x_next;
}

// void player_class::setPos(float x) {

// }

// void player_class::drawPaddle(void)
// {

// }