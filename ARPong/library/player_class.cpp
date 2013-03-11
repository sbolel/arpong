#include "player_class.h"

player_class::~player_class(void) {}

player_class::player_class(void)  {
	reset();
}

player_class::player_class(int player_id)
{
	reset();
	switch (player_id)
	{
		case 1:
			id = 1;
			x_pos_max = 5.0;
			x_pos_min = -5.0;
			y_pos_max = 0.0;
			y_pos_min = 0.0;
			z_pos_max = 0.0;
			z_pos_min = 0.0;
			break;
		case 2:
			id = 2;
			x_pos_max = 5.0;
			x_pos_min = -5.0;
			y_pos_max = 0.0;
			y_pos_min = 0.0;
			z_pos_max = 0.0;
			z_pos_min = 0.0;
			break;
		default:
			break;
	}
}

void player_class::reset(void)
// Set all values to zero.
{
	x_pos = 0.0; y_pos = 0.0; z_pos = 0.0;
	x_rot = 0.0; y_rot = 0.0; z_rot = 0.0;
}

void player_class::apply_TR_Matrix(void)
// Apply translations, then rotations to current coordinate axes.
{
	glTranslated(x_pos, y_pos, z_pos);
	glRotated(x_rot, 1.0, 0.0, 0.0);
	glRotated(y_rot, 0.0, 1.0, 0.0);
  glRotated(z_rot, 0.0, 0.0, 1.0);
}

void player_class::apply_RT_Matrix(void)
// Apply rotations, then translations to current coordinate axes.
{
	glRotated(x_rot, 1.0, 0.0, 0.0);
	glRotated(y_rot, 0.0, 1.0, 0.0);
  glRotated(z_rot, 0.0, 0.0, 1.0);
	glTranslated(x_pos, y_pos, z_pos);
}

void player_class::apply_R_Matrix(void)
// Apply rotations, then translations to current coordinate axes.
{
	glRotated(x_rot, 1.0, 0.0, 0.0);
	glRotated(y_rot, 0.0, 1.0, 0.0);
  glRotated(z_rot, 0.0, 0.0, 1.0);

}

void player_class::apply_T_Matrix(void)
// Apply rotations, then translations to current coordinate axes.
{
	glTranslated(x_pos, y_pos, z_pos);
}

// void player_class::drawPaddle(void)
// {

// }