#include "ARPong.h"
#pragma comment(lib, "ws2_32.lib")

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
#include "library/image/histogram.h"
#include "library/image/video.h"
#include "library/image/objectDetection.h"

using namespace std;

const double pi = atan(1.0)*4.0;
const double rad = (pi/180.0);
const GLfloat moveSpeed=0.001;
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

// Video variables
const char* TRAINING_FILE = "../skin_rgb.txt";
video_stream stream;
frame dtn_frame;
histogram hist;
bool window_closed = false;
double tld = .000001;   // detection threshold
GLuint texture;

class sliding_window {
  enum { SIZE = 10 };
  list<glm::ivec2> history;
  public:
    void push_value(glm::ivec2 p) {
      history.push_back(p);
      if(history.size() > SIZE) {
        history.pop_front();
      }
    }
    glm::ivec2 value() const {
      glm::vec2 tmp(0);
      int factor = 1;
      for(auto& p : history) {
        tmp += glm::vec2(p) / pow(2.0f, factor++);
      }
      return glm::ivec2(tmp);
    }
};

sliding_window cursor_pos;

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

void on_close() {
  window_closed = true;
  stream.cleanup();
}

frame detect_skin(const frame& in) {
  dtn_frame = in;

  const rgb_byte ZERO = { };
  for(int y = HEIGHT - 1; y; --y) {
    for(int x = WIDTH - 1; x; --x) {
      auto orig = dtn_frame.get_pixel(x, y);
      rgb hist_in = { orig[2], orig[1], orig[0] };
      if(hist.value(hist_in) < tld) {
        orig[0] = orig[1] = orig[2] = 0;
      }
    }
  }

  return dtn_frame;
}

bool main_loop_iter() {
  if(!window_closed && stream.next_frame()) {
    dtn_frame = detect_skin(stream.current_frame);
    cursor_pos.push_value(calculate_median(dtn_frame));
    glutPostRedisplay();
    glutMainLoopEvent();
    return true;
  }
  else {
    return false;
  }
}

int main(int argc, char** argv) {
  setup_player();
  glSetupOpenGL(argc, argv);

  _beginthread(tcp_comm, 0, (void*)1);

  ifstream training(TRAINING_FILE);
  hist = load_histogram(training);

  while(main_loop_iter()) { }
}

void glDisplay() {
  const struct { float tu, tv; float x, y, z; } vertices[] = {
    { 0.0f, 0.0f, -1.0f,-1.0f, 0.0f },
    { 1.0f, 0.0f,  1.0f,-1.0f, 0.0f },
    { 1.0f, 1.0f,  1.0f, 1.0f, 0.0f },
    { 0.0f, 1.0f, -1.0f, 1.0f, 0.0f }
  };
  moveObjects();

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glPushMatrix();
  glScaled(-1.0, -1.0, 1.0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, dtn_frame.get_buffer_data());
  glInterleavedArrays(GL_T2F_V3F, 0, vertices);
  glDrawArrays(GL_QUADS, 0, 4);
  glPopMatrix();

  glPushMatrix();
    glScaled(-1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(1.0, 0.0, 0.0);
    glPointSize(15.0);
    auto cur = cursor_pos.value();
      player.x = cur.x * 2. / WIDTH - 1;
      // player.y = cur.y * 2. / HEIGHT - 1;
    glBegin(GL_POINTS);
      glVertex2d(cur.x * 2. / WIDTH - 1., cur.y * 2. / HEIGHT - 1.);
    glEnd();
  glPopMatrix();

  glTranslated(0.0, -0.75,-0.75);
  glRotated(180,0.0,1.0,0.0);

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

void setup_player(void) {
  cout << "Enter player number: ";
  std::string id;
  getline(cin,id,'\n');
  player.id = std::stoi(id);
  ball.x = 0.0;
  ball.z = ARENA_LENGTH/2;
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