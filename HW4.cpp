/*********
   CTIS164 - HW4


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
#include "vec.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  30 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532



 //vertical phase shift
float angle = 0;
int counter = 0;
int live;
typedef struct {
	float r, g, b;
} color_t;


typedef struct {
	vec_t pos;
	float radius;
}planet_t;

typedef struct {
	vec_t pos;
	vec_t N;
} vertex_t;








int renk = 0;
typedef struct {
	vec_t pos;
	vec_t vel;  
	color_t color;
	bool  visibility; 
	double radius;
} light_t;

bool activeTimer = true;



color_t mulColor(float k, color_t c) {
	color_t tmp = { k * c.r, k * c.g, k * c.b };
	return tmp;
}

color_t addColor(color_t c1, color_t c2) {
	color_t tmp = { c1.r + c2.r, c1.g + c2.g, c1.b + c2.b };
	return tmp;
}

double distanceImpact(double d) {
	return fabs((-1.0 / 400.0) * d + 1.0);
}
double clampAbove0(double value)
{
	return value < 0 ? 0 : value;
}
color_t calculateColor(light_t source, vertex_t v) {
	vec_t L = subV(source.pos, v.pos);
	vec_t uL = unitV(L);
	float factor = clampAbove0(dotP(uL, v.N)) * distanceImpact(magV(L));
	return mulColor(factor, source.color);
}
void setColor(color_t c)
{
	glColor3f(c.r, c.g, c.b);
}


void setVertex(vec_t vector)
{
	glVertex2d(vector.x, vector.y);
}

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

light_t  e[1000];
light_t sun;


int m = 0;

planet_t planet{ {0,0}, 30 };
bool sun_color = true;
bool border_mode = false;
void lights()
{
	
	sun.pos = { 0,0 };
	
	sun.radius = 30;
}

bool checkCircle(float px, float py, float cx, float cy, float R) {
	float dx = px - cx;
	float dy = py - cy;
	float d = sqrt(dx * dx + dy * dy);
	return d <= R;
}
//
// to draw circle, center at (x,y)
// radius r
//
int angle_speed = 2;
int angle_direction = 1;

void randomGenerator()
{
	planet.radius = rand() % 30;
	planet.radius += 15;
	angle = rand() % 360;
	angle_speed = rand() % 4;
	angle_speed += 1;
	angle_direction = rand() % 2;
}
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
vertex_t fun;

void draw_sun()
{
	if (sun_color)
		sun.color = { 1,1,1 };
	else
		sun.color = { 0.2,0.2,0.2 };

	glColor3f(sun.color.r, sun.color.g, sun.color.b);
	circle(0, 0, 30);
}
void draw_planet()
{
	float a;
	 float radius = 220;
	 float x = planet.pos.x, y = planet.pos.y;
	 float ang = angle;
	

	
	glBegin(GL_TRIANGLE_FAN);
	
	glColor3f(0.2,0.2, 0.2);
	glVertex2f(radius * cos(ang * D2R), radius * sin(ang * D2R));
	fun.pos.x = radius* cos(ang * D2R);
	fun.pos.y = radius * sin(ang * D2R);
	

	for (int i = 0; i <= 100; i++)
	{
		a = 2 * PI * i / 100;

		x = radius * cos(ang * D2R) + planet.radius * cos(a);
		y = radius * sin(ang * D2R) + planet.radius * sin(a);
		
		vertex_t P = { { x, y },{ radius * cos(ang * D2R), radius * sin(ang * D2R) } };
		P.N = unitV(subV({ x, y }, { radius * cos(ang * D2R), radius * sin(ang * D2R) }));

		color_t res = {0.2,0.2,0.2};
		
		if(sun_color)
		res = addColor(res, calculateColor(sun, P));
		
		for (int i = 0; i < counter; i++)
		{
			
			if(e[i].visibility)
			res = addColor(res, calculateColor(e[i], P));
		}
	
		
		glColor3f(res.r, res.g, res.b);
		glVertex2f(x, y);
		
	}
	glEnd();
	
	
}

//
// To display onto window using OpenGL commands
//

void draw_light()
{
	int i;
		for (i = 0; i < counter; i++) {

			if (e[i].visibility)
			{
				glColor3f(e[i].color.r, e[i].color.g, e[i].color.b);
				circle(e[i].pos.x, e[i].pos.y, 9);
			}
				
				
			}
		live = i;
			
		
		
	
		
	
}
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	
	sun.color.r = 1;
	sun.color.g = 1;
	sun.color.b = 1;
	sun.pos = { 0, 0};
	
	

	draw_light();
	draw_planet();
	draw_sun();

	glColor3f(0, 0, 0.6);
	glRectf(-400, -400, 400, -370);
	
	glColor3f(1,1,1);
	if (activeTimer)
	{
		glColor3f(0, 1, 0);
		vprint(-370, -391, GLUT_BITMAP_8_BY_13, "[F1] Animation ON");
	}
	else
	{
		glColor3f(1, 0, 0);
		vprint(-370, -391, GLUT_BITMAP_8_BY_13, "[F1] Animation OFF");
	}
	
	if (sun_color)
	{
		glColor3f(0, 1, 0);
		vprint(-150, -391, GLUT_BITMAP_8_BY_13, "[F2] SUN ON");
	}
	else
	{
		glColor3f(1, 0, 0);
		vprint(-150, -391, GLUT_BITMAP_8_BY_13, "[F2] SUN OFF");
	}

	glColor3f(1, 1, 1);
	vprint(50, -391, GLUT_BITMAP_8_BY_13, "[F3] RESTART");

	if (!border_mode)
	{
		glColor3f(0, 1, 0);
		vprint(230, -391, GLUT_BITMAP_8_BY_13, "[F4] TARGET LOCK ON");
	}
	else
	{
		glColor3f(1, 1, 0);
		vprint(-200, -361, GLUT_BITMAP_8_BY_13, "ON TARGET MODE YOU CANNOT DISPLAY NEW LIGHT SOURCES");
		glColor3f(1, 0, 0);
		vprint(230, -391, GLUT_BITMAP_8_BY_13, "[F4] TARGET LOCK OFF");

		

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

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

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
		activeTimer = !activeTimer;
		break;
	case GLUT_KEY_F2:
		sun_color = !sun_color;
		break;
	case GLUT_KEY_F3:
		randomGenerator();
		break;
	case GLUT_KEY_F4:
		border_mode = !border_mode;
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
int a;
void onClick(int button, int stat, int x, int y)
{
	// Write your codes here.
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && m< 10) {
		int mx = x - winWidth / 2;
		int my = winHeight / 2 - y;
		
		
		if (!border_mode)
		{
			if (m < 10)
			{
				m++;
				renk++;
			}
			color_t temp;


			light_t eTemp;
			eTemp.pos.x = mx;
			eTemp.pos.y = my;

			if (renk % 3 == 0)
				eTemp.color = { 1,0,0 };
			else if (renk % 3 == 1)
				eTemp.color = { 0,1,0 };
			else
				eTemp.color = { 0,0,1 };

			eTemp.visibility = true;
			e[counter] = eTemp;
			counter++;
			

		}
		

	}


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


#if TIMER_ON == 1
void onTimer(int v) {
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	if (activeTimer)
	{

		// Write your codes here.

		if (angle_direction == 1)
		{
			angle += angle_speed;
			if (angle > 360)
				angle -= 360;
		}
		else
		{
			angle -= angle_speed;
			if (angle > 360)
				angle -= 360;
		}


		for (int i =0; i < counter;i++)
		{

			
				bool on;

				on = checkCircle(e[i].pos.x, e[i].pos.y, fun.pos.x, fun.pos.y, planet.radius);

				if (!border_mode)
				{
					e[i].vel = mulV(13, unitV(subV(fun.pos, e[i].pos)));

					e[i].pos = addV(e[i].pos, e[i].vel);
				}
				else
				{
					e[i].pos = addV(e[i].pos, e[i].vel);

					if (e[i].pos.x > 380 || e[i].pos.x < -380) {
						e[i].vel.x *= -1;
					}

					if (e[i].pos.y > 380 ||e[i].pos.y < -360) {
						e[i].vel.y *= -1;
					}
				}


				if (on && e[i].visibility == true)
				{
					e[i].visibility = false;
					counter++;
					m--;

				}
			

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
	glutCreateWindow("HW 4- Light Source ");

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