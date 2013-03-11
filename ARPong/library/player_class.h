#pragma once
#include <GL/freeglut.h>

class player_class
{
public:
  GLint   id;
  GLfloat position[3];  // x, y, z
  GLfloat pos_max[3];   // x, y, z
  GLfloat pos_min[3];   // x, y, z
  GLint   score;

  ~player_class(void);
   player_class(void);
   player_class(int id);

  // Coordinate matrix manipulation functions
  void reset(void);
  void apply_T_Matrix(void);

  GLfloat x() {return position[0];}
  GLfloat y() {return position[1];}
  GLfloat z() {return position[2];}

  void xInc(GLfloat pos) {position[0] += pos;}
  void yInc(GLfloat pos) {position[1] += pos;}
  void zInc(GLfloat pos) {position[2] += pos;}

  void xDec(GLfloat pos) {position[0] -= pos;}
  void yDec(GLfloat pos) {position[1] -= pos;}
  void zDec(GLfloat pos) {position[2] -= pos;}


};