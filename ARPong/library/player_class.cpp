#include "player_class.h"

player_class::~player_class(void) {}

player_class::player_class(void) { reset(); }

player_class::player_class(int player_id)
{
	reset();
	id = player_id;
	x_max = ARENA_WIDTH;
	x_min = -ARENA_WIDTH;
  x_vel = 0.0f;
  z_vel = 0.0f;
  score = 0;
	if (player_id==3) {
		z_min = 0.0;
		z_max = ARENA_LENGTH;
	}
}

void player_class::reset(void)
{
	x = 0.0;
	z = 0.0;
	x_cnt = 0;
}

void player_class::Tx(void)
{
	glTranslated(x, 0.0, 0.0);
}

void player_class::Txz(void)
{
	glTranslated(x, 0.0, z);
}

void player_class::xInc(GLfloat dx)
{
	GLfloat x_next = x + dx;
	if ((x_next) < x_max)
		x = x_next;
}

void player_class::xDec(GLfloat dx)
{
	GLfloat x_next = x - dx;
	if (x_next > x_min)
		x = x_next;
}

void player_class::zInc(GLfloat dz)
{
	GLfloat z_next = z + dz;
	if ((z_next) < z_max)
		z = z_next;
}

void player_class::zDec(GLfloat dz)
{
	GLfloat z_next = z - dz;
	if (z_next > z_min)
		z = z_next;
}