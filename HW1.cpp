/*********
   CTIS164 - HW1

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

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  15 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define PLAY 2
#define PAUSE 3
#define OFF 4

int appState = OFF;

#define UFO_ON 5
#define UFO_OFF 6

int UFOstate = UFO_OFF;

bool inStartButton = false;
bool inPauseButton = false;
bool inUFObutton = false;

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//

int xU = -300, yU = -280; 

int mX = 200, mY = -220;



bool activeTimer = false;

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


//I DREW THE SATELLITE HERE
void SATELLITE()
{
	
	// INTERIOR
	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2f(xU, yU);
	glColor3f(1, 0, 0);
	glVertex2f(-350, -280);
	
	glVertex2f(-250, -280);
	glEnd();

	// BOUNDARY
	glColor3f(1, 1, 1);
	glBegin(GL_LINE_LOOP);
	glVertex2f(xU+20, yU+40);
	glVertex2f(xU+40, yU+20);
	glVertex2f(xU+13, yU-11);
	glVertex2f(xU-12, yU+13); 
	glEnd();

	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2f(xU-3, yU+20);
	glVertex2f(xU, yU+10);
	glVertex2f(xU-30, yU);
	glVertex2f(xU-10, yU+20);
	glEnd();

	glColor3f(1, 1, 0);
	glBegin(GL_POLYGON);
	glVertex2f(xU +10, yU);
	glVertex2f(xU+20, yU -2);
	glVertex2f(xU +20, yU-12);
	glVertex2f(xU - 3, yU -25);
	glEnd();


	glColor3f(0, 1, 0);
	glPointSize(3);
	glBegin(GL_POINTS);
	glVertex2f(xU+30, yU+20);
	glVertex2f(xU + 11, yU+20);
	glVertex2f(xU+20, yU+30);
	glVertex2f(xU + 20, yU + 11);
	glEnd();

	//OUR SOLAR PANELS' CONNECTING LINES
	glColor3f(160.0 / 255, 160.0 / 255, 160.0 / 255);
	glBegin(GL_LINES);
	// line 1
	glVertex2f(xU+20, yU+40);
	glVertex2f(xU+10, yU+50);
	// line 2
	glColor3f(160.0 / 255, 160.0 / 255, 160.0 / 255);
	glVertex2f(xU+40, yU+20);
	glVertex2f(xU+50, yU+10);
	glEnd();

	//LEFT SOLAR PANEL
	glColor3f(0, 128.0/255, 1);
	glBegin(GL_QUADS);
	glVertex2f(xU +6, yU +46);
	glVertex2f(xU + 14, yU+54);
	glVertex2f(xU -2, yU+70);
	glVertex2f(xU - 10, yU +62);
	glEnd();


	glColor3f(1, 1, 1);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xU + 6, yU + 46);
	glVertex2f(xU + 14, yU + 54);
	glVertex2f(xU - 2, yU + 70);
	glVertex2f(xU - 10, yU + 62);
	glVertex2f(xU + 6, yU + 46);
	glEnd();

	//RIGHT SOLAR PANEL
	glColor3f(0, 128.0 / 255, 1);
	glBegin(GL_QUADS);
	glVertex2f(xU + 54, yU + 14);
	glVertex2f(xU + 46, yU + 6);
	glVertex2f(xU +62, yU -10);
	glVertex2f(xU + 71, yU -1);
	glEnd();

	glColor3f(1, 1, 1);
	glBegin(GL_LINE_STRIP);
	glVertex2f(xU + 54, yU + 14);
	glVertex2f(xU + 46, yU + 6);
	glVertex2f(xU + 62, yU - 10);
	glVertex2f(xU + 71, yU - 1);
	glVertex2f(xU + 54, yU + 14);
	glEnd();


	glColor3f(1,1,1);
	glBegin(GL_LINES);
	// right
	glVertex2f(xU + 2, yU + 66);
	glVertex2f(xU - 6, yU + 58);
	
	glVertex2f(xU + 6, yU + 62);
	glVertex2f(xU -2, yU + 54);

	glVertex2f(xU + 10, yU + 58);
	glVertex2f(xU +2, yU + 50);

	glVertex2f(xU -8, yU + 64);
	glVertex2f(xU +8, yU + 48 );

	glVertex2f(xU -4, yU + 68);
	glVertex2f(xU + 12, yU + 52);

	glVertex2f(xU -6, yU + 66);
	glVertex2f(xU + 10, yU + 50);

	// left
	glVertex2f(xU + 52, yU + 12);
	glVertex2f(xU + 68, yU -4);

	glVertex2f(xU + 48, yU + 8);
	glVertex2f(xU + 64, yU -8);

	glVertex2f(xU + 58, yU + 10);
	glVertex2f(xU + 50, yU +2);

	glVertex2f(xU +63, yU +6);
	glVertex2f(xU +54, yU -2);

	glVertex2f(xU + 66, yU + 2);
	glVertex2f(xU +58, yU -6);

	glVertex2f(xU + 50, yU + 10);
	glVertex2f(xU + 66, yU - 6);


	glEnd();

	
	glColor3f(160.0 / 256, 160.0 / 256, 160.0 / 256);
	circle(xU, yU, 15);

	glColor3f(224.0 / 256, 224.0 / 256, 224.0 / 256);
	circle(xU, yU, 10);
}


// HERE IS THE TV
void displayTV()
{
	glColor3f(102.0 / 256, 51.0 / 256, 0);
	glRectf(400, 375, -400, -370);


	glColor3f(32.0 / 256, 32.0 / 256, 32.0 / 256);
	circle(-100, -320, 30);


	//ON - OFF BUTTON
	glColor3f(255.0 / 256, 0, 0);
	circle(-100, -320, 10);

	glColor3f(255.0 / 256, 0, 0);
	glRectf(-97, -320, -103, -300);

	vprint(-130, -360, GLUT_BITMAP_8_BY_13, "ON / OFF");


	//PAUSE BUTTON
	glColor3f(32.0 / 256, 32.0 / 256, 32.0 / 256);
	circle(100, -320, 30);

	glColor3f(255.0 / 256, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(110, -320);
	glVertex2f(90, -330);
	glVertex2f(90, -310);
	glEnd();
	vprint(81, -360, GLUT_BITMAP_8_BY_13, "PAUSE");
	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);

	glVertex2f(-400, -370);
	glVertex2f(400, -370);
	glVertex2f(400, 375);
	glVertex2f(-400, 375);
	
	glEnd();

	//UFO Button
	glColor3f(0,0,0);
	circle(0, -320, 20);
	glColor3f(1, 0, 0);
	circle(0, -320, 15);
	glColor3f(0, 1, 0);
	circle(0, -320, 10);
	glColor3f(0, 0, 1);
	circle(0, -320, 5);
	vprint(-30, -360, GLUT_BITMAP_8_BY_13, "UFO TIME");


	//TV Screen

	glColor3f(0, 0, 0);
	glRectf(350, 280, -350, -280);

	glColor3f(0,0,0);
	glBegin(GL_QUADS);
	glVertex2f(350, 280);
	glVertex2f(350, -280);
	glColor3f(0, 0, 51.0/255);
	glVertex2f(-350, -280);
	glVertex2f(-350, 280);
	glEnd();
}

//I DREW THE MAN HERE
void house_MAN()
{
	

	glColor3f(204.0/255, 0, 102.0/255);
	glBegin(GL_LINE_STRIP);
	glVertex2f(mX+10, mY -20);
	glVertex2f(mX, mY);
	glVertex2f(mX-10, mY -20);
	glVertex2f(mX, mY);
	glVertex2f(mX, mY+25);
	glVertex2f(mX, mY);
	glEnd();

	glColor3f(204.0 / 255, 0, 102.0 / 255);
	glBegin(GL_LINES);
	
	glVertex2f(mX -15, mY+15);
	glVertex2f(mX +15, mY + 15);
	glEnd();

	glColor3f(204.0 / 255, 0, 102.0 / 255);
	circle(mX, mY +35, 10);


	glColor3f(128.0/255, 128.0 / 255, 128.0 / 255);
	circle(200, -220 + 260, 42);
	glColor3f(1,1,1);
	circle(200, -220 + 279, 30);

}
void display_SAT()
{
	SATELLITE();

	glColor3f(102.0 / 256, 51.0 / 256, 0);
	glRectf(380, 280, -400, 375);

	glColor3f(102.0 / 256, 51.0 / 256, 0);
	glRectf(-400, -370, -200, -280);

	glColor3f(0, 1, 0);
	glRectf(350, -320, 340, -300);

	glColor3f(0, 153.0 / 255, 0);
	glRectf(350, -240, -350, -280);

	glColor3f(0, 1, 0);
	glBegin(GL_QUADS);
	glVertex2f(350, -240);
	glVertex2f(-350, -240);
	glColor3f(25.0/255, 52.0/255, 0);
	glVertex2f(-350, -280);
	glVertex2f(350, -280);
	glEnd();


}

void UFO_light()
{
	
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 0); // red
	glVertex2f(200, 40);
	glColor3f(0, 1, 0); // green
	glVertex2f(260, -260);
	glColor3f(1, 0.5, 0); // blue
	glVertex2f(150, -260);
	glEnd();
}

void display_off()
{
	glColor3f(192.0 / 256, 192.0 / 256, 192.0 / 256);
	glRectf(350, 280, -350, -280);

	glColor3f(1, 0, 0);
	glRectf(350, -320,  340, -300);
	
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);

	glVertex2f(350, 280);
	glVertex2f(-350, 280);
	glVertex2f(-350, -280);
	glVertex2f(350, -280);

	glEnd();
}
void display() {
	//
	// clear window to black
	//
	glClearColor(255.0 / 256, 153.0 / 256, 51.0 / 256, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	
	
	displayTV();
	

	switch (UFOstate)
	{
	case UFO_OFF: ; break;
	case UFO_ON: UFO_light(); break;
	}
	house_MAN();

	

 switch (appState) {
 case PLAY: display_SAT(); break;
 case OFF: display_off(); break;
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
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
	{
	

		if (inStartButton == true)
		{
			
			if (appState == PLAY)
		{
				appState = OFF;
				
			
		}
		   else if(appState == OFF)
			{
				appState = PLAY;
				
				activeTimer = true;
				
			}

		}
		 
	}
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
	{
		if (inPauseButton == true && appState == PLAY )
			activeTimer = !activeTimer;
	}
	if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
	{
		if (inUFObutton == true)
			if (UFOstate == UFO_OFF)
			{
				mY++;
				UFOstate = UFO_ON;
			}
			else
				UFOstate = UFO_OFF;
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


// (px, py) : coordinate of a point, p
// (cx, cy) : center point (centroid) of the circle
// R : radius of the circle
// Result: true (point is withing the circle), otherwise false.
bool checkCircle(float px, float py, float cx, float cy, float R) {
	float dx = px - cx;
	float dy = py - cy;
	float d = sqrt(dx * dx + dy * dy);
	return d <= R;
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.

	int mx = x - winWidth / 2;
	int my = winHeight / 2 - y;
	inStartButton = checkCircle(mx, my, -100, -320, 30);
	inPauseButton = checkCircle(mx, my, 100, -320, 30);
	inUFObutton = checkCircle(mx, my, 0, -320, 20);
	
	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	
	
	
	if (activeTimer)
	{
		
			//FOR SATELLITE
			if (yU +20 > 324)
				xU = -300, yU = -300;
			else
			{
				xU++;
				yU += 1;
			}
		//FOR UFO
			if (UFOstate == UFO_ON)
			{
				if (mY < 40)
					mY++;
			}
			else
				mY = -220;
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
	glutCreateWindow("HW-1: Television");

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