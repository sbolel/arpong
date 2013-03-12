#pragma once
#include <GL/freeglut.h>
#include "arena.h"
// font strings for drawText2D();
//-------------------------------
#define rom10 GLUT_BITMAP_TIMES_ROMAN_10
#define rom24 GLUT_BITMAP_TIMES_ROMAN_24
#define helv10 GLUT_BITMAP_HELVETICA_10
#define helv12 GLUT_BITMAP_HELVETICA_12
#define helv18 GLUT_BITMAP_HELVETICA_18


void drawScene(void);
void drawPlayer(void);
void drawEnemy(void);
void drawAxes(int colr);