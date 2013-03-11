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

  glSetupOpenGL(argc, argv);

  // keep running until the video source quits or someone closes us
  // while(main_loop_iter()) { }

  glutMainLoop();

	// deinitCapture(DEVICE);
  return 0;
}

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

	glFlush();
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

void glKeyboard(unsigned char key, int xw, int yw) {
  key_state[key] = true;
  switch(key) {
    case'a': case's': case'd': case'w':
      cout << "Move player\n";
      break;
    case'f': case'g': case'h': case't':
      cout << "Move enemy\n";
      break;
    case'z': case'x':
      cout << "Move Z direction\n";
      break;
    case 'c':
      if(glutClearStatus==0) glutClearStatus = 1;
      else                   glutClearStatus = 0;
      break;
    case 'p':
      if(glutAnimationStatus ==1) glRunTimer(0);
      else                        glRunTimer(1);
      break;
    case ' ': case 27: case 'Q': case 'q':
      exit(0);
      break;
    default:
      cout << "Invalid key\n";
      break;
  }
}

void glKeyUpFunc(unsigned char key, int x, int y) {
    key_state[key] = false;
}

void glKeySpecial(int key, int xw, int yw) {
  // mouse pos in coords with origin at lower left
  //(window system puts origin at UPPER left)
  int xpos,ypos;
      xpos = xw;
      ypos = HEIGHT - yw;
  switch(key) {
    case GLUT_KEY_F1:
      cout << "\nARPong Help File (Press F1 to Resume)\n";
      cout << "----------------------------\n";
      cout << "() \n";
      cout << "() \n";
      break;
    default:
      cout << "Special Key Pressed, Value: "<< (int)key << "\n)";
      break;
  }
}

void drawText2D(void *pFont, double x0, double y0, const char *pString) {
  int i, imax;
  glRasterPos2d(x0, y0);
  imax = 1023;
  for(i=0; pString[i] != '\0' && i<imax; i++) {
    glutBitmapCharacter(pFont, pString[i]);
  }
}

void glReshape( int width, int height ) {
  glViewport((WIDTH-HEIGHT)/2,0, HEIGHT, HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (45, (GLfloat)WIDTH / (GLfloat)HEIGHT, 1.0, 1000.0);
  glMatrixMode(GL_MODELVIEW);
  glutPostRedisplay();
}

void glHidden (int isVisible) {
    if(isVisible == GLUT_VISIBLE)
      glRunTimer(1);
    else
      glRunTimer(0);
}

void glRunTimer(int isOn) {
    glutAnimationStatus = isOn;
    if(isOn == 1)
      glutTimerFunc(NU_PAUSE, glTimer, 1);
}

void glTimer (int value) {
    if(value != glutAnimationStatus) {
        cout << "\n(Anim cancelled during GLUT wait)\n";
        glutPostRedisplay();
        glRunTimer(1);
    }
}

void glSetupOpenGL(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("ARPong");
  glutDisplayFunc(glDisplay);
  glutIdleFunc(glDisplay);
  glutReshapeFunc(glReshape);
  glutKeyboardFunc(glKeyboard);
  glutKeyboardUpFunc(glKeyUpFunc);
  glutSpecialFunc(glKeySpecial);
  glutIgnoreKeyRepeat(true);

  glEnable (GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);          // Set up sunlight.
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_TEXTURE_2D);

  GLuint texture;
  glShadeModel(GL_SMOOTH);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient_intensity);
  glLightfv(GL_LIGHT0, GL_POSITION, sun_direction);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_intensity);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glClearColor(0.03, 0.03, 0.2, 0.0);
  glRunTimer(1);
}