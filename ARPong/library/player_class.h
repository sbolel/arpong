#pragma once
#include <GL/freeglut.h>

class player_class
{
public:
  int     id;
  double	x_pos, x_pos_max, x_pos_min,
          y_pos, y_pos_max, y_pos_min,
          z_pos,  z_pos_max, z_pos_min,
          x_rot, y_rot, z_rot,
          x_bnd, y_bnd, z_bnd;
  int		isDragging;				// TRUE if user is holding down the mouse button
  								// that affects our value(s); else FALSE.
  int m_x,m_y;  // last mouse-dragging position.

  ~player_class(void);
   player_class(void);
   player_class(int id);

  // Coordinate matrix manipulation functions
  void reset(void);
  void apply_TR_Matrix(void);
  void apply_RT_Matrix(void);
  void apply_R_Matrix(void);
  void apply_T_Matrix(void);
};