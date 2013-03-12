#pragma once
#include <GL/freeglut.h>
#include "arena.h"

class player_class
{
public:
  GLint   id;
  GLfloat x;
  GLfloat x_max;
  GLfloat x_min;
  GLfloat z;
  GLfloat z_max;
  GLfloat z_min;
  GLint   score;
  GLfloat x_vel;
  GLfloat z_vel;
  GLint x_cnt;
  GLfloat x_last;

  ~player_class(void);
   player_class(void);
   player_class(int id);

  // Coordinate matrix manipulation functions
  void reset(void);
  void Tx(void);
  void Txz(void);

  void xInc(GLfloat pos);
  void xDec(GLfloat pos);
  void zInc(GLfloat pos);
  void zDec(GLfloat pos);
  void draw(void);
};