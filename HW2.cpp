/*********
   CTIS164 - HOMEWORK 2

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
#include <time.h>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  1000 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer
#define ENEMY_PERIOD 20
#define COLOR_PERIOD 60
#define D2R 0.0174532


#define SHOTGUN 1
#define RAINBOWGUN 2

#define ON 3
#define OFF 4
#define START 5
#define PAUSE 6
#define END 7



#define FIRE 10



#define DURATION 20
#define SECOND 60
int gX = -100;
int gY = 0;
int bX, bY, rX, rY;
int bY_a, bY_b;
int initial = SHOTGUN;
int game_start = OFF;
int timeCounter;
int running = END;
int d = 359;
int second;
int chosen;
bool activeTimer = true;
int cX[5] = { 700,700,700,700,700};
int cY[5] = {d,d+100,d+200,d+300,d+400};
double r[5], g[5], b[5];
bool fire = false;
void onTimer(int v);
void colors(int va);
/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
void enemies(int value);
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
void bullet()
{
	
	
	fire = true;

	
}


void gunChoose()
{
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(-600, 550);
		glVertex2f(600, 550);
	glColor3f(0,0,0.5);
	glVertex2f(600, -550);

	glVertex2f(-600, -550);
	glEnd();

	glColor3f(96.0/256, 96.0 / 256, 96.0 / 256);
	glBegin(GL_QUADS);
	glVertex2f(-200,0);
	glVertex2f(-200,10);
	glVertex2f(-400,10);
	glVertex2f(-400, 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(-400, 20);
	glVertex2f(-180, 20);
	glVertex2f(-180, 10);
	glVertex2f(-400, 10);
	glEnd();


	glBegin(GL_QUADS);
	glVertex2f(-400, 20);
	glVertex2f(-440, 0);
	glVertex2f(-440, -20);
	glVertex2f(-400, -10);
	glEnd();

	vprint(-350, -40, GLUT_BITMAP_8_BY_13, "SHOTGUN");
	glColor3f(102.0 / 256, 51.0 / 256, 0);
	glBegin(GL_QUADS);
	glVertex2f(-440, 0);
	glVertex2f(-480, 20);
	glVertex2f(-480, -40);
	glVertex2f(-440, -20);
	glEnd();

	glColor3f(102.0 / 256, 51.0 / 256, 0);
	glBegin(GL_QUADS);
	glVertex2f(-300, 0);
	glVertex2f(-360, 0);
	glVertex2f(-360, -10);
	glVertex2f(-300, -10);
	glEnd();

	//RAINBOW GUN
	glColor3f(1, 13.0 / 256, 204.0/256);
	glBegin(GL_QUADS);
	glVertex2f(200, 0);
	glVertex2f(260, 0);
	glVertex2f(260, 10);
	glVertex2f(200, 10);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(210, 10);
	glVertex2f(200, 10);
	glVertex2f(200, -20);
	glVertex2f(210, -20);
	
	glEnd();

	vprint(185, -40, GLUT_BITMAP_8_BY_13, "RAINBOW GUN");

	switch (initial)
	{
	case SHOTGUN:
		glColor3f(1, 0, 0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-550, 200);
		glVertex2f(-100, 200);
		glVertex2f(-100, -200);
		glVertex2f(-550, -200);
		glVertex2f(-550, 200);
		glEnd();
		break;
	case RAINBOWGUN:

		glColor3f(1, 0, 0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(160, 60);
		glVertex2f(300, 60);
		glVertex2f(300, -60);
		glVertex2f(160, -60);
		glVertex2f(160, 60);
	
		glEnd();
		break;
	}

}

void shotgun()
{
	
	glColor3f(1, 1, 0.3);
	glBegin(GL_QUADS);
	glVertex2f(bX - 300, bY + 20);
	glVertex2f(bX - 280, bY + 20);
	glVertex2f(bX - 280, bY + 10);
	glVertex2f(bX - 300, bY + 10);
	glEnd();

	glColor3f(96.0 / 256, 96.0 / 256, 96.0 / 256);
	glBegin(GL_QUADS);
	glVertex2f(gX - 200, gY + 0);
	glVertex2f(gX - 200, gY + 10);
	glVertex2f(gX - 400, gY + 10);
	glVertex2f(gX - 400, gY + 0);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(gX - 400, gY + 20);
	glVertex2f(gX - 180, gY + 20);
	glVertex2f(gX - 180, gY + 10);
	glVertex2f(gX - 400, gY + 10);
	glEnd();


	glBegin(GL_QUADS);
	glVertex2f(gX - 400, gY + 20);
	glVertex2f(gX - 440, gY + 0);
	glVertex2f(gX - 440, gY - 20);
	glVertex2f(gX - 400, gY - 10);
	glEnd();


	glColor3f(102.0 / 256, 51.0 / 256, 0);
	glBegin(GL_QUADS);
	glVertex2f(gX - 440, gY + 0);
	glVertex2f(gX - 480, gY + 20);
	glVertex2f(gX - 480, gY - 40);
	glVertex2f(gX - 440, gY - 20);
	glEnd();

	glColor3f(102.0 / 256, 51.0 / 256, 0);
	glBegin(GL_QUADS);
	glVertex2f(gX - 300, gY + 0);
	glVertex2f(gX - 360, gY + 0);
	glVertex2f(gX - 360, gY - 10);
	glVertex2f(gX - 300, gY - 10);
	glEnd();

	
}

void rainbowgun()
{

	
	glBegin(GL_TRIANGLES);
	glColor3f(0, 1, 0);
	glVertex2f(rX-500, gY-10+bY_a );
	glColor3f(0, 0, 1);
	glVertex2f(rX-500, gY  + bY_b);
	glColor3f(1, 0, 0);
	glVertex2f(gX-410, gY-5);
	glEnd();


	glColor3f(1, 13.0 / 256, 204.0 / 256);
	glBegin(GL_QUADS);
	glVertex2f(gX -400, gY);
	glVertex2f(gX - 460, gY);
	glVertex2f(gX - 460, gY-10);
	glVertex2f(gX - 400, gY-10);
	glEnd();

	glBegin(GL_QUADS);
	glVertex2f(gX - 460, gY-10);
	glVertex2f(gX - 450, gY-10);
	glVertex2f(gX - 450, gY -30);
	glVertex2f(gX - 460, gY -30);

	glEnd();

	
}
void displayBalls()
{
	
	glColor3f(r[0]/256, g[0]/256, b[0]/256);
	circle(cX[0], cY[0], 30);
	
	circle(cX[3], cY[3], 10);
	circle(cX[2], cY[2], 20);
	glColor3f(r[1] / 256, g[1] / 256, b[1] / 256);
	circle(cX[1], cY[1], 30);
	glColor3f(r[2] / 256, g[2] / 256, b[2] / 256);
	circle(cX[2], cY[2], 30);
	circle(cX[0], cY[0], 20);
	circle(cX[1], cY[1], 20);
	glColor3f(r[3] / 256, g[3] / 256, b[3] / 256);
	circle(cX[3], cY[3], 30);
	circle(cX[2], cY[2], 20);
	glColor3f(r[4] / 256, g[4] / 256, b[4] / 256);
	circle(cX[4], cY[4], 30);
	circle(cX[0], cY[0], 10);
	circle(cX[3], cY[3], 20);
	circle(cX[2], cY[2], 10);
	circle(cX[1], cY[1], 10);
	glColor3f(r[2] / 256, g[2] / 256, b[2] / 256);
	circle(cX[4], cY[4], 20);
	circle(cX[3], cY[3], 20);
	glColor3f(r[3] / 256, g[3] / 256, b[3] / 256);
	circle(cX[4], cY[4], 10);
	circle(cX[3], cY[3], 10);
	

	glColor3f(0, 0, 0);
	glRectf(-600, -400, 600, -360);
	glColor3f(1, 1, 0);
	vprint(-220, -380, GLUT_BITMAP_8_BY_13, "Press <Space> to fire, Press <F1> Pause/Restart ");
}
void balls()
{

	int num;
	if (cY[0] > 400)
	{
		cY[0] = -450;
		cX[0] = (rand() % 100);
	}
	if (cY[1] > 500)
	{
		cY[1] = -550;
		cX[1] = (rand() % 100)+100;
	}
	if (cY[2] > 600)
	{
		cY[2] = -650;
		cX[2] = (rand() % 100) + 200;
		
	}
	if (cY[3] > 700)
	{
		cY[3] = -750;
		cX[3] = (rand() % 100 )+ 300;
		
	}

	if (cY[4] > 800)
	{
		cY[4] = -850;
		cX[4] = (rand() % 100 + 400);
		
	}


	displayBalls();
}
void game()
{
	glColor3f(1, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(-600, 550);
	glColor3f(0, 0, 0);
	glVertex2f(600, 550);

	glVertex2f(600, -550);
	glColor3f(1, 0, 0);
	glVertex2f(-600, -550);
	glEnd();

	if (chosen == SHOTGUN)
	{
		shotgun();
	}
	if (chosen == RAINBOWGUN)
	{
		rainbowgun();
	}
	glColor3f(0, 0, 0);
	glRectf(-600, -400, 600, -360);
	glColor3f(1, 1, 0);
	vprint(-220, -380, GLUT_BITMAP_8_BY_13, "Press <Space> to fire, Press <F1> Pause/Restart ");

	
	

	balls();
	
}

void countdown()
{
	glColor3ub(90, 100, 140);
	vprint2(400, 300, 0.35, "%02d:00", timeCounter);
	
}
void disp_end()
{
	
	glColor3ub(90, 100, 140);
	vprint2(400, 300, 0.35, "00:00");
}

void F1_key()
{
	if (running == START)
	{
		activeTimer = !activeTimer;
		


	}
	if (running == END)
	{
		

		cX[0] = 700;
		cX[1] = 700;
		cX[2] = 700;
		cX[3] = 700;
		cX[4] = 700;
		cY[0] = d;
		cY[1] = d + 100;
		cY[2] = d + 200;
		cY[3] = d + 300;
		cY[4] = d + 400;

		timeCounter = DURATION;
		running = START;
		
		

	}
	
	
	
}

void Spacebar()
{
	
}

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//

	


	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	
	gunChoose();
	glColor3f(0, 0, 0);
	glRectf(-600, -400, 600, -360);
	glColor3f(1, 1, 0);
	vprint(-240, -380, GLUT_BITMAP_8_BY_13, "Use Left or Right Arrow Key to choose a weapon than press Enter");
	
	if (game_start == ON)
	{
		game();

		switch (running)
		{
		case START:

			countdown();
			
			break;
		case END:
			
			disp_end();
			glColor3f(0, 0, 0);
			glRectf(-600, -400, 600, -360);
			glColor3f(1, 1, 0);
			vprint(-150, -380, GLUT_BITMAP_8_BY_13, "press <F1> to start a new game");
			
			break;
		}
	}
	glEnd();


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

	if (key == 00,59 && running != START)
	{
		game_start = ON;
		chosen = initial;
	}
	if (key == 32)
		bullet();
	

	

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.

	switch (key)
	{
	case 27:
			exit(0);
			break;
	
	}
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
	case GLUT_KEY_UP: up = true;
		if (running == START)
		{

			if (gY < 370)
				gY += 10;
			if (fire == false && bY < 370)
				bY += 10;

			
		}
		break;
	case GLUT_KEY_DOWN: down = true; 
		if (running == START)
		{
			if (gY > -320)
				gY -= 10;
			if (fire == false && bY > -320)
				bY -= 10;
		}
		 break;
	case GLUT_KEY_LEFT: left = true;
		if (initial = RAINBOWGUN &&  running != START )
			initial = SHOTGUN;
		
		break;
	case GLUT_KEY_RIGHT: right = true; 
		
		if (initial = SHOTGUN && running != START)
			initial = RAINBOWGUN;
		break;
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

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	if (activeTimer)
	{
		
		if (running == START)
		{
			
		
			
			// Write your codes here.
			if (timeCounter > 0) {
				
				
				timeCounter--;
				
				
			}
			else
				running = END;


		}
		
		
	}
	

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
	
}
#endif

void colors(int va)
{
	glutTimerFunc(COLOR_PERIOD, colors, 0);
	if (activeTimer)
	{
		if (running == START)
		{
			r[0] = (rand() % 256);
			g[0] = (rand() % 256);
			b[0] = (rand() % 256);
			r[1] = (rand() % 256);
			g[1] = (rand() % 256);
			b[1] = (rand() % 256);
			r[2] = (rand() % 256);
			g[2] = (rand() % 256);
			b[2] = (rand() % 256);
			r[3] = (rand() % 256);
			g[3] = (rand() % 256);
			b[3] = (rand() % 256);
			r[4] = (rand() % 256);
			g[4] = (rand() % 256);
			b[4] = (rand() % 256);
		}

	}
}
void enemies(int value)
{
	glutTimerFunc(ENEMY_PERIOD, enemies, 0);
	if (activeTimer)
	{
		if (running == START )
		{

			
			cY[0] += 7;
			
			cY[1] += 7;
			
			cY[2] += 7;
			
			cY[3] += 7;
			
			cY[4] += 7;



			if (fire == true)
			{
				if(chosen == SHOTGUN)
				bX += 14;
				if (chosen == RAINBOWGUN)
				{
					bY_a -= 3;
					rX += 14;
					bY_b += 3;
				}
			}

			
			if (bX > 900)
			{
				fire = false;
				bX = gX;
				bY = gY;

			}
			if (rX > 1100)
			{
				bY_a = 0;
				bY_b = 0;
				fire = false;
				rX = gX+50;
				gY = bY;
			}

			if (bX-300 >= cX[0]-30 && bX - 300 <= cX[0] + 30 && bY <= cY[0] + 33 && bY >= cY[0] - 33) 
				cY[0] = -1500;

			if (bX - 300 >= cX[1] - 30 && bX - 300 <= cX[1] + 30 && bY <= cY[1] + 33 && bY >= cY[1] - 33)
				cY[1] = -1500;
			if (bX - 300 >= cX[2] - 30 && bX - 300 <= cX[2] + 30 && bY <= cY[2] + 33 && bY >= cY[2] - 33)
				cY[2] = -1500;
			if (bX - 300 >= cX[3] - 30 && bX - 300 <= cX[3] + 30 && bY <= cY[3] + 33 && bY >= cY[3] - 33)
				cY[3] = -1500;
			if (bX - 300 >= cX[4] - 30 && bX - 300 <= cX[4] + 30 && bY <= cY[4] + 33 && bY >= cY[4] - 33)
				cY[4] = -1500;

			if(cY[0]-33 >= bY_a && cY[0] + 30 <= bY_b && cX[0]+35  <= rX-500 && cX[0] - 35 <= rX - 500)
				cY[0] = -1500;
			if (cY[1] - 33 >= bY_a && cY[1] + 30 <= bY_b && cX[1] + 35 <= rX-500 && cX[1] - 35 <= rX - 500)
				cY[1] = -1500;
			if (cY[2] - 33 >= bY_a && cY[2] + 30 <= bY_b && cX[2] + 35 <= rX-500 && cX[2] - 35 <= rX - 500)
				cY[2] = -1500;
			if (cY[3] - 33 >= bY_a && cY[3] + 30 <= bY_b && cX[3] + 35 <= rX-500 && cX[3] - 35 <= rX - 500)
				cY[3] = -1500;
			if (cY[4] - 33 >= bY_a && cY[4] + 30 <= bY_b && cX[4] + 35 <= rX-500 && cX[4] - 35 <= rX - 500)
				cY[4] = -1500;

		}
		
	}
	glutPostRedisplay();
}


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
	glutCreateWindow("HW-2: Shooting Game ");

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
	glutTimerFunc(ENEMY_PERIOD, enemies, 0);
	glutTimerFunc(COLOR_PERIOD, colors, 0);
#endif

	Init();

	glutMainLoop();
}