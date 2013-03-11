#include "ARPong.h"
using namespace std;


// Set up the image capture library ESCAPI
void setup_escapi() {
	// Initialize the DLL and ask how many webcams it found
	int devices = setupESCAPI();
	if(!devices) {
		cerr << "ESCAPI found no devices.\n";
		throw runtime_error("setupESCAPI");
	}
	// Tell the library where our buffer is and how big it is
	SimpleCapParams params = { frame_buffer, WIDTH, HEIGHT };
	if(!initCapture(DEVICE, &params)) {
		cerr << "initCapture failed, please install a better webcam\n";
		throw runtime_error("initCapture");
	}
}

bool main_loop_iter() {
	doCapture(DEVICE);

	// If we got a video frame, render it and run the game
	if(!isCaptureDone(DEVICE)) {
		/// TODO: Game logic and networking calls go here

		// Tell GLUT to render this frame and manually proceed to the next one
		glutPostRedisplay();
		glutMainLoopEvent();
		return true;
	}
	// Can't capture more video, exit the program
	else {
		return false;
	}
}

int main(int argc, char** argv) {
	// setup_escapi();
  GLfloat sun_direction[] = { 0.0, 2.0, -1.0, 1.0 };
  GLfloat sun_intensity[] = { 0.7, 0.7, 0.7, 1.0 };
  GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };

	glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("ARPong");

  glutDisplayFunc(glDisplay);
  glutIdleFunc(glDisplay);        // GLUT idle function
	glutReshapeFunc(glReshape);
	glutKeyboardFunc(glKeyboard);
  glutKeyboardUpFunc(glKeyUpFunc);// keystate function
	glutSpecialFunc(glKeySpecial);
  glutIgnoreKeyRepeat(true);
  glEnable (GL_DEPTH_TEST);       // enable DEPTH test

  glEnable(GL_LIGHTING);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);
  glEnable(GL_LIGHT0);                // Set up sunlight.
  glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);
  glEnable(GL_COLOR_MATERIAL);        // Configure glColor().
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	GLuint texture;
  glClearColor(0.03, 0.03, 0.2, 0.0);     // Dark blue background set
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glShadeModel(GL_SMOOTH);
  runAnimTimer(1);                // start our animation loop.

  // keep running until the video source quits or someone closes us
  // while(main_loop_iter()) { }

  glutMainLoop();                 // enter GLUT's event-handler; NEVER EXITS.

	// deinitCapture(DEVICE);
  return 0;                       // SUCCESSFUL exit, C++ Compatibility
}


void glReshape( int width, int height )
//------------------------------------------------------------------------------
{
  glViewport((WIDTH-HEIGHT)/2,0, HEIGHT, HEIGHT);
  glMatrixMode(GL_PROJECTION);  // select camera-setting matrix stack
  glLoadIdentity();             // clear it: identity matrix.
  gluPerspective (45, (GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);        //return to modelview coords
  glutPostRedisplay();      // request redraw--we changed window size!
}


// OpenGL display function, called whenever a new frame is requested
void glDisplay() {
  movePlayer();
  moveEnemy();
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glLoadIdentity ();
  gluLookAt(  0.0f, 0.0f, -2.0f,
      0.0f, 0.0f,  30.0f,
      0.0f, 1.0f,  2.0f);
  // vertices for simple textured-quad rendering
  const struct { float tu, tv; float x, y, z; } vertices[] = {
    { 0.0f, 0.0f, -1.0f,-1.0f, 0.0f },
    { 1.0f, 0.0f,  1.0f,-1.0f, 0.0f },
    { 1.0f, 1.0f,  1.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, -1.0f, 1.0f, 0.0f }
  };

// Movement and Rotation
  drawScene();
  glPushMatrix();
    charPlayer.apply_T_Matrix();
  	// glScaled(-1.0, -1.0, 0.0);
  	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, frame_buffer);
  	// glInterleavedArrays(GL_T2F_V3F, 0, vertices);
  	// glDrawArrays(GL_QUADS, 0, 4);
    drawAxes(0);
	glPopMatrix();
  glPushMatrix();
    glTranslated(0.0,0.0,30.0);
    glRotated(180, 0.0,1.0,0.0);
    charEnemy.apply_T_Matrix();
    // glScaled(-1.0, -1.0, 0.0);
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, frame_buffer);
    // glInterleavedArrays(GL_T2F_V3F, 0, vertices);
    // glDrawArrays(GL_QUADS, 0, 4);
    drawAxes(0);
  glPopMatrix();
  glPushMatrix();
    glTranslated(0.0,0.0,13.0);
    glColor3f(0.0, 1.0, 0.25);
    glutSolidSphere(0.5, 20, 20);
    glScaled(0.7,0.7,0.7);
    drawAxes(0);
  glPopMatrix();

	glFlush();	                // do any and all pending openGL rendering.
	glutSwapBuffers();
}

void movePlayer(void){
float yaw = charPlayer.y_rot;
  if(key_state['a'] == true) {
      charPlayer.x_pos += moveSpeed;
  }
  if(key_state['d'] == true) {
      charPlayer.x_pos -= moveSpeed;
  }
  if(key_state['w'] == true) {
    charPlayer.y_pos += moveSpeed;
  }
  if(key_state['s'] == true) {
    charPlayer.y_pos -= moveSpeed;
  }
  if(key_state['z'] == true) {
    charPlayer.z_pos += moveSpeed;
  }
  if(key_state['x'] == true) {
    charPlayer.z_pos -= moveSpeed;
  }
}

void moveEnemy(void){
float yaw = charEnemy.y_rot;
  if(key_state['f'] == true) {
    charEnemy.x_pos -= moveSpeed;
  }
  if(key_state['h'] == true) {
    charEnemy.x_pos += moveSpeed;
  }
  if(key_state['t'] == true) {
    charEnemy.y_pos += moveSpeed;
  }
  if(key_state['g'] == true) {
    charEnemy.y_pos -= moveSpeed;
  }
  if(key_state['z'] == true) {
    charEnemy.z_pos += moveSpeed;
  }
  if(key_state['x'] == true) {
    charEnemy.z_pos -= moveSpeed;
  }
}