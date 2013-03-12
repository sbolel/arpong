#include "ARPong.h"

#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <winsock.h>
#include <process.h>
#include <list>

#include <GL/freeglut.h>
#include <escapi.h>


#include "library/tcp_message.h"
#include "library/tcp_server.h"
#include "library/tcp_client.h"
#include "library/player_class.h"
#include "library/glut_draw.h"
#include "library/glut_materials_lights.h"
#include "library/image/histogram.h"
#include "library/image/video.h"
#include "library/image/objectDetection.h"


using namespace std;

const double pi = atan(1.0)*4.0;
const double rad = (pi/180.0);
const GLfloat moveSpeed=0.001;
// Video variables
const char* TRAINING_FILE = "../skin_rgb.txt";
video_stream stream;
frame dtn_frame;
histogram hist;
// Player variables
player_class player(1);
player_class enemy(2);
player_class ball(3);
// GLUT lighting variables
GLfloat sun_direction[] = { 0.0, 2.0, -1.0, 1.0 };
GLfloat sun_intensity[] = { 0.7, 0.7, 0.7, 1.0 };
GLfloat ambient_intensity[] = { 0.3, 0.3, 0.3, 1.0 };
// Control variables
static int glutClearStatus = 1;
static int glutAnimationStatus = 1;
static bool key_state[256] = {false};
bool window_closed = false;



void updateClient(server_data sd){
  enemy.x = sd.x;
  ball.x = sd.b_x;
  ball.z = sd.b_z;
}

void tcp_comm(void *) {
  WSACleanup();
  cout << "---------------------------------\n";
  if (player.id==1) {
    ListenOnPort(9000);
    while(1) {
      Sleep(20);
      ServerWrite(player.x, ball.x, ball.z);
      enemy.x = ServerRead();
    }
  }
  else {
    bool connect_status = 0;
    while (connect_status==0) {
      cout << "Enter server IP: ";
      std::string ip_str;
      getline(cin,ip_str,'\n');
      char * server_ip = new char[ip_str.size()+1];
      std::copy(ip_str.begin(), ip_str.end(), server_ip);
      connect_status = ConnectToHost(9000, server_ip);
      delete server_ip;
    }
    while(1) {
      Sleep(30);
      updateClient(ClientRead());
      ClientWrite(player.x);
    }
  }
}

void setup_player(void) {
  cout << "Enter player number: ";
  std::string id;
  getline(cin,id,'\n');
  player.id = std::stoi(id);
  ball.x = 0.0;
  ball.z = ARENA_LENGTH/2;
}


int main(int argc, char** argv) {
  setup_player();
  glSetupOpenGL(argc, argv);
  _beginthread(tcp_comm, 0, (void*)1);
  glutMainLoop();
  return 0;
}

void glDisplay() {

  moveObjects();

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glTranslated(0.0, -0.75,-0.75);
  glRotated(180,0.0,1.0,0.0);

// Movement and Rotation
  drawScene();

  glPushMatrix();
    player.Tx();
    drawAxes(0);
	glPopMatrix();
  glPushMatrix();
    glTranslated(0.0,0.0,ARENA_LENGTH);
    glRotated(180, 0.0,1.0,0.0);
    enemy.Tx();
    drawAxes(0);
  glPopMatrix();
  glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    if(player.id==2) {
      glRotated(180, 0.0,1.0,0.0);
      glTranslated(0.0,0.0,-ARENA_LENGTH);
    }
    ball.Txz();
    glutSolidSphere(0.1, 20, 20);
    glScaled(0.7,0.7,0.7);
    drawAxes(0);
  glPopMatrix();

	glFlush();
	glutSwapBuffers();
}

void glReshape( int width, int height ) {
  glViewport(0,0, WIDTH, HEIGHT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glRotated(-45.0,1.0,0.0,0.0);
  gluPerspective (140.0, (WIDTH/HEIGHT), 0.5, ARENA_LENGTH+0.75f);
  glMatrixMode(GL_MODELVIEW);
  glutPostRedisplay();
}

void glKeyboard(unsigned char key, int xw, int yw) {
  key_state[key] = true;
  switch(key) {
    case'a': case's':
      cout << "Move player\n";
      break;
    case'd': case'f':
      cout << "Move enemy\n";
      break;
    case'j': case'k':
      cout << "Move ball\n";
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
    default:
      // cout << "Special Key Pressed, Value: "<< (int)key << "\n)";
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

void moveObjects(void){
  if (player.id==1)
    moveBall();
  movePlayer();
}

void movePlayer(void) {
  if(key_state['a'] == true) {
    player.xInc(moveSpeed);
  }
  if(key_state['s'] == true) {
    player.xDec(moveSpeed);
  }
}

void moveEnemy(void) {
  if(key_state['f'] == true) {
    enemy.xDec(moveSpeed);
  }
  if(key_state['d'] == true) {
    enemy.xInc(moveSpeed);
  }
}

void moveBall(void) {
  if(key_state['j'] == true) {
      ball.xInc(moveSpeed);
  }
  if(key_state['k'] == true) {
      ball.xDec(moveSpeed);
  }
  if(key_state['i'] == true) {
      ball.zInc(moveSpeed);
  }
  if(key_state['m'] == true) {
      ball.zDec(moveSpeed);
  }
}