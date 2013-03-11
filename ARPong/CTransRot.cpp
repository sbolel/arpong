//==============================================================================
//
//  CTransRot function bodies:
//
//==============================================================================
#include "CTransRot.h"

CTransRot::~CTransRot(void)
//------------------------------------------------------------------------------
// Default destructor
{
}

CTransRot::CTransRot(void)
//------------------------------------------------------------------------------
// Default constructor
{
	reset();						// set all values to zero.
}

CTransRot::CTransRot(int player)
//------------------------------------------------------------------------------
// Default constructor
{
	reset();						// set all values to zero.
	switch (player)
	{
		case 1:
			playerid = 1;
			x_pos_max = 5.0;
			x_pos_min = -5.0;
			y_pos_max = 0.0;
			y_pos_min = 0.0;
			z_pos_max = 0.0;
			z_pos_min = 0.0;
			break;
		case 2:
			playerid = 2;
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

void CTransRot::reset(void)
//------------------------------------------------------------------------------
// Set all values to zero.
{
	x_pos = 0.0; y_pos = 0.0; z_pos = 0.0;
	x_rot = 0.0; y_rot = 0.0; z_rot = 0.0;
}

void CTransRot::apply_TR_Matrix(void)
//------------------------------------------------------------------------------
// Apply translations, then rotations to current coordinate axes.
{
	glTranslated(x_pos, y_pos, z_pos);	// trans. current coord system axes and
										// origin to make new coord system.
								// d suffix to specify 'doubles' as arguments.
	glRotated(x_rot, 1.0, 0.0, 0.0);
	glRotated(y_rot, 0.0, 1.0, 0.0);
  glRotated(z_rot, 0.0, 0.0, 1.0);
}

void CTransRot::apply_RT_Matrix(void)
//------------------------------------------------------------------------------
// Apply rotations, then translations to current coordinate axes.
{
	glRotated(x_rot, 1.0, 0.0, 0.0);
	glRotated(y_rot, 0.0, 1.0, 0.0);
  glRotated(z_rot, 0.0, 0.0, 1.0);
								// d suffix to specify 'doubles' as arguments.
	glTranslated(x_pos, y_pos, z_pos);	// trans. current coord system axes and
										// origin to make new coord system.
}

void CTransRot::apply_R_Matrix(void)
//------------------------------------------------------------------------------
// Apply rotations, then translations to current coordinate axes.
{
	glRotated(x_rot, 1.0, 0.0, 0.0);
	glRotated(y_rot, 0.0, 1.0, 0.0);
  glRotated(z_rot, 0.0, 0.0, 1.0);

}

void CTransRot::apply_T_Matrix(void)
//------------------------------------------------------------------------------
// Apply rotations, then translations to current coordinate axes.
{
	glTranslated(x_pos, y_pos, z_pos);	// trans. current coord system axes and
										// origin to make new coord system.
}

// void CTransRot::drawPaddle(void)
// {

// }