#include "ARPong.h"
#include "render.h"
#include "game.h"

#include <GL/freeglut.h>

namespace {
	game* global_game;
}

void drawAxes(int colr) {
	// When 'colr' = 0 draws red,grn,blu coordinate system axes;
	// Otherwise draws cyan,magenta,yellow axes
	glDisable(GL_LIGHTING);//-----------------Draw axes:
	glBegin(GL_LINES); // start drawing lines:
	if(colr) glColor3f( 0.0, 1.0, 1.0); // cyan or
	else glColor3f( 1.0, 0.0, 0.0); // Red X axis
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f( 1.0, 0.0, 0.0);

	if(colr) glColor3f( 1.0, 0.0, 1.0); // Magenta or
	else glColor3f( 0.0, 1.0, 0.0); // Green Y axis
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f( 0.0, 1.0, 0.0);

	if(colr) glColor3f( 1.0, 1.0, 0.0); // Yellow or
	else glColor3f( 0.0, 0.0, 1.0); // Blue Z axis
	glVertex3f( 0.0, 0.0, 0.0);
	glVertex3f( 0.0, 0.0, 1.0);
	glEnd(); // end drawing lines
	glEnable(GL_LIGHTING);// ------------DONE drawing axes.
}

void drawScene(void) {
	glPushMatrix();
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.25f, 0.8f);
		glVertex3f( 5.0f, 2.0f, 0.0f);
		glVertex3f( 5.0f, -2.0f, 0.0f);
		glVertex3f( 5.0f, -2.0f, 30.0f);
		glVertex3f( 5.0f, 2.0f, 30.0f);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(0.0f, 0.25f, 0.8f);
		glVertex3f( -5.0f, 2.0f, 0.0f);
		glVertex3f( -5.0f, -2.0f, 0.0f);
		glVertex3f( -5.0f, -2.0f, 30.0f);
		glVertex3f( -5.0f, 2.0f, 30.0f);
	glEnd();
	glPopMatrix();
}

void display() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, -2.0f,
		0.0f, 0.0f, 30.0f,
		0.0f, 1.0f, 2.0f);

	// Movement and Rotation
	drawScene();
	glPushMatrix();
	glTranslated(global_game->p1.pos.x, global_game->p1.pos.y, 1.0);
	drawAxes(0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0,0.0,30.0);
	glRotated(180, 0.0,1.0,0.0);
	glTranslated(global_game->p2.pos.x, global_game->p2.pos.y, -1.0);
	drawAxes(0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0,0.0,13.0);
	glColor3f(0.0, 1.0, 0.25);
	glutSolidSphere(0.5, 20, 20);
	glScaled(0.7,0.7,0.7);
	drawAxes(0);
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

GLfloat sun_direction[] = { 0.0f, 2.0f, -1.0f, 1.0f };
GLfloat sun_intensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat ambient_intensity[] = { 0.3f, 0.3f, 0.3f, 1.0f };

void setupGlut(int argc, char** argv, game& game_) {
	global_game = &game_;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("ARPong");
	glutDisplayFunc(display);
	glutIgnoreKeyRepeat(true);

	glEnable (GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); // Set up sunlight.
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);

	glShadeModel(GL_SMOOTH);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);
	glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glClearColor(0.03f, 0.03f, 0.2f, 0.0f);

	glViewport((WIDTH-HEIGHT)/2,0, HEIGHT, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (45, (GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}