#pragma once
#include <GL/freeglut.h>

// font strings for drawText2D();
//-------------------------------
#define rom10 GLUT_BITMAP_TIMES_ROMAN_10
#define rom24 GLUT_BITMAP_TIMES_ROMAN_24
#define helv10 GLUT_BITMAP_HELVETICA_10
#define helv12 GLUT_BITMAP_HELVETICA_12
#define helv18 GLUT_BITMAP_HELVETICA_18

const GLfloat ARENA_LENGTH = 2.0f;
const GLfloat ARENA_WIDTH = 1.5f;
const GLfloat WALL_THICK = 0.1f;

void drawScene(void);
void drawAxes(int colr);