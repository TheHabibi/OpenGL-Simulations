/*********
   CTIS164 - HW3

*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

#define WINDOW_WIDTH  1300
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define START 4653

#define RESUME 5423


int ammo = 30;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, spacebar = false;


int start = START;

bool activeTimer = false;
int  winWidth, winHeight; // current Window width and height

float A = 200,
fq = 3,  //frequency
C = 0,
B = 0,
D = 250;

int shot = 0;

float angle = 0;

float r_q, r;
#define  CY 25
#define CX 100
#define HX 100
#define HY -150
#define MX 500
#define MY 300
float cY = 25,
cX = 100,
hX = 100,
hY = -150;

int mX = 500, mY = 300;

typedef struct {
	float x, y;
} point_t;

typedef struct {
	point_t pos;    //position of the object
	float   angle;  //view angle 
	float   r;
} player_t;

typedef struct {
	point_t pos;
	float angle;
	bool active;
} fire_t;

player_t player = { {-500, 0}, 45, 40 };
fire_t   fire = { { 0, 0 }, 0, false };
//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI * i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char* string, void* font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//
// To display onto window using OpenGL commands
//

void drawPlayer(player_t tp) {
	

	glLineWidth(17);
	glColor3f(153.0/255, 0,0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(tp.pos.x +30, tp.pos.y );
	glVertex2f(tp.pos.x, tp.pos.y-40);
	glVertex2f(tp.pos.x-30, tp.pos.y );
	glVertex2f(tp.pos.x, tp.pos.y-40);
	glVertex2f(tp.pos.x, tp.pos.y-40);
	glVertex2f(tp.pos.x, tp.pos.y - 100);
	glEnd();


	
	glLineWidth(20);

	glColor3f(1,0,0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(tp.pos.x+33, tp.pos.y);
	glVertex2f(tp.pos.x  -70 * cos(tp.angle * D2R), tp.pos.y - 70 * sin(tp.angle * D2R));
	glVertex2f(tp.pos.x-30, tp.pos.y);
	
	glEnd();

	



	glLineWidth(1);

	
}
float f(float x) {
	return A * sin((fq * x + C) * D2R) + B;
}
void drawFire(fire_t f) {
	
	if (f.active) {
		glColor3f(0.2, 0.2, 0.2);
		circle(f.pos.x, f.pos.y, 15);
		
	}
	
}

void F1_key()
{
	if (start == START)
	{
		shot = 0;
		ammo = 30;
		activeTimer = !activeTimer;
		start = RESUME;
	}
	else if (start == RESUME)
	{
		activeTimer = !activeTimer;
		
	}

}

void display_info()
{
	glColor3f(0, 0, 0);
	glRectf(-650, -350, 650, -315);
	glColor3f(1, 1, 0);
	vprint(-120, -334, GLUT_BITMAP_8_BY_13, "Press <F1> to start a new game");
}

void display_info2()
{
	glColor3f(0, 0, 0);
	glRectf(-650, -350, 650, -315);
	glColor3f(1, 1, 0);
	vprint(-120, -334, GLUT_BITMAP_8_BY_13, "<SpaceBar> Fire,  <F1> Pause/Restart");
}

void enemies()
{
	glColor3f(0, 1, 1);
	 r = fabs(A);
	r_q = fabs(D);
	glColor3f(0.3, 0.3, 1.0);
	circle_wire(100, 25, r_q);
	glColor3f(0, 0, 0);
	

	if(ammo == 0)
		vprint2(-580, 300, 0.25, " OUT OF AMMO !");
	else
		vprint2(-580, 300, 0.25, " AMMO  x%d", ammo);
	//harmonic 
	circle(f(angle) + hX, angle + hY, 10);
	
	float c = 0.5 * sin(0.5 * angle * D2R) + 0.5; //between c=0 and c=1.0
	glColor3f(c, c * c, 1 - c);
	//circular
	circle(r_q * cos(angle * D2R) + cX, r_q * sin(angle * D2R) + cY, 30);



	
	// parachute guy
	glColor3f(1,1,0);
	circle(mX, mY+42, 25);
	glLineWidth(17);
	glColor3f(0,0,0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(mX + 10, mY - 20);
	glVertex2f(mX, mY);
	glVertex2f(mX - 10, mY - 20);
	glVertex2f(mX, mY);
	glVertex2f(mX, mY + 25);
	glVertex2f(mX, mY);
	glEnd();

	
	glBegin(GL_LINES);

	glVertex2f(mX - 15, mY + 15);
	glVertex2f(mX + 15, mY + 15);
	glEnd();

	
	circle(mX, mY + 35, 10);


	
}


void display() {
	//
	// clear window to black
	//
	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex2f(-650, -350);
	glVertex2f(650, -350);
	glColor3f(1, 0, 1);
	glVertex2f(650, 350);
	glVertex2f(-650, 350);
	glEnd();

	drawPlayer(player);
	drawFire(fire);
	
	
	switch (start)
	{
	case START:
		display_info();
		break;
	case RESUME:
		enemies();
		display_info2();
		break;
	}
	

	
	glutSwapBuffers();

	
	
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (key == ' ')
		spacebar = true;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	if (key == ' ')
		spacebar = false;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_F1:
		F1_key();
		break;

	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.



	// to refresh the window it calls display() function
	glutPostRedisplay();
}
void turnPlayer(player_t* tp, float inc) {
	
	if (tp->angle < 90 && tp->angle > -90)
		tp->angle += inc;


	if (tp->angle == 90)
		tp->angle -= 3;

	if (tp->angle == -90)
		tp->angle += 3;

	
	
}

void movePlayer(player_t* tp, float speed) {

	float dy = speed * sin(tp->angle * D2R);

	
	tp->pos.y += speed;
	float r = tp->r;

	// check rectangular boundary
	if (tp->pos.y >(370 - r) || tp->pos.y < (-270 + r)) {
		
		tp->pos.y -= speed;
	}
}

#if TIMER_ON == 1
void onTimer(int v) {


	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	if (activeTimer)
	{
		
		// Write your codes here.
		if (right) {
			turnPlayer(&player, -3);
		}
		if (left) {
			turnPlayer(&player, 3);
		}
		if (up) {
			movePlayer(&player, 3);
		}
		if (down) {
			movePlayer(&player, -3);
		}

		if (spacebar && fire.active == false && ammo > 0) {
			fire.pos = player.pos;
			fire.angle = player.angle;
			ammo--;
			fire.active = true;
		}

		if (fire.active) {
			fire.pos.x += 10 * cos(fire.angle * D2R);
			fire.pos.y += 10 * sin(fire.angle * D2R);

			if (fire.pos.x > 850 || fire.pos.x < -850 || fire.pos.y>850 || fire.pos.y < -850) {
				fire.active = false;
			}

			
		}
		angle += 2;
		if (angle > 360)
			angle -= 360;

		if (fire.pos.y > (r_q * sin(angle * D2R) + cY - 30) && fire.pos.y < (r_q * sin(angle * D2R) + cY + 30) && (fire.pos.x > r_q * cos(angle * D2R) + cX - 30) && (fire.pos.x < r_q * cos(angle * D2R) + cX + 30))
		{
			cY = 1000;
			shot++;
		}
		if (fire.pos.x < (f(angle) + hX + 30) && fire.pos.x >(f(angle) + hX - 30) && fire.pos.y > (angle + hY - 30) && (fire.pos.y < angle + hY + 30))
		{
			hY = 1000;
			shot++;
		}
		if (fire.pos.x < mX + 30 && fire.pos.x > mX - 30 && fire.pos.y < mY + 50 && fire.pos.y > mY - 30)
		{
			mX = 1000;
			shot++;
		}

		if (mY < -500)
			mY = 300;
		else
			mY -= 4;
		
		if (shot == 3 )
		{
			
			cY = CY;
			cX = CX;
			hX = HX;
			hY = HY;
			mX = MX;
			mY = MY;
			fire.active = false;
			activeTimer = !activeTimer;
			start = START;
			
		}

			
		
	}
	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("HW-3 Slingshot Game");

	glutDisplayFunc(display);
	glutReshapeFunc(onResize);

	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);

	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);

	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

	Init();

	glutMainLoop();
}