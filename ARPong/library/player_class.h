#pragma once
#include <GL/freeglut.h>

class player_class
{
public:
  GLint   id;
  GLfloat x;
  GLfloat z;
  GLint   score;

  ~player_class(void);
   player_class(void);
   player_class(int id);

  // Coordinate matrix manipulation functions
  void reset(void);
  void Tx(void);
  void Txz(void);

  void xInc(GLfloat pos) {x += pos;}
  void xDec(GLfloat pos) {x -= pos;}

};