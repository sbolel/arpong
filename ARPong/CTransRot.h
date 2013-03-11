#ifndef CTRANSROT_H_
#define CTRANSROT_H_
#include <GL/freeglut.h>    // open-source GLUT functions for windows, keybd, mouse

class CTransRot
{
//==============================================================================
// Declare a new class that records/accumulates translate and rotate amounts,
// usually adjusted by mouse & keyboard inputs.  I use an instance of this class
// for each user-adjustable joint in a jointed object.Its 'applyMatrix()' member
// then makes openGL calls to apply these translate/rotate amounts to the openGL
// matrix currently selected(e.g. glMatrixMode(GL_PROJECTION or GL_MODELVIEW)).

public:
int playerid;
double	x_pos, y_pos, z_pos;	// cumulative position offset
double	x_rot, y_rot, z_rot;	// cumulative rotation on x,y,z axes
double  x_bnd, y_bnd, z_bnd;  // cumulative position offset
double  x_pos_max, x_pos_min, y_pos_max, y_pos_min, z_pos_max, z_pos_min;
int		isDragging;				// TRUE if user is holding down the mouse button
								// that affects our value(s); else FALSE.
int m_x,m_y;					// last mouse-dragging position.

~CTransRot(void);				// default destructor
 CTransRot(void);				// default constructor
 CTransRot(int player);       // default constructor
void reset(void);				// reset everything to zero.
void apply_TR_Matrix(void);		// apply to openGL's current coordinate system;
								// --translate, then rotate to make a new one.
void apply_RT_Matrix(void);		// --rotate, then translate to make a new one.
void apply_R_Matrix(void);		// --rotate
void apply_T_Matrix(void);		// --translate
};

#endif
