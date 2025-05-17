// Author: Abyunalvin


#include <stdlib.h>
#include <vector>
#include <deque>
#include <windows.h>
#include <GL/glut.h>
#include <iostream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <mmsystem.h>
#include <thread>
using namespace std;

static bool replay = false; //check if starts a new game
static bool over = true; //check for the game to be over
static float squareSize = 50.0; //size of one square on the game
static float xIncrement = 2.5; // x movement on pacman
static float yIncrement = 6.5; // y movement on pacman
static int rotation = 0; // orientation of pacman
float* monster1 = new float[4] {10.5, 8.5, 1.0, 0}; //coordinates and direction of first monster with vision
float* monster2 = new float[4] {13.5, 1.5, 2.0, 0}; //coordinates and direction of second monster with vision
float* monster3 = new float[4] {4.5, 6.5, 3.0, 0}; //coordinates and direction of third monster with vision
float* monster4 = new float[4] {2.5, 13.5, 4.0, 0}; //coordinates and direction of fourth monster with vision
static vector<int> border = { 0, 0, 15, 1, 15, 15, 14, 1, 0, 14, 15, 15, 1, 14, 0, 0 }; //coordinates of the border walls


//coordinates of the obstacles (divided into 3 for clarity)
static vector<int> obstaclesTop = { 2, 2, 3, 6, 3, 6, 4, 5, 4, 2, 5, 4, 5, 3, 6, 5, 6, 1, 9, 2, 7, 2, 8, 5, 9, 5, 10, 3, 10, 4, 11, 2, 11, 5, 12, 6, 12, 6, 13, 2 };
static vector<int> obstaclesMiddle = { 2, 9, 3, 7, 3, 7, 4, 8, 4, 9, 5, 11, 5, 6, 6, 10, 6, 10, 7, 8, 7, 8, 8, 9, 6, 7, 7, 6, 8, 6, 9, 7, 10, 6, 9, 10, 9, 10, 8, 8, 11, 9, 10, 11, 11, 8, 12, 7, 12, 7, 13, 9 };
static vector<int> obstaclesBottom = { 2, 10, 3, 13, 3, 13, 4, 12, 5, 12, 6, 13, 6, 13, 7, 11, 8, 11, 9, 13, 9, 13, 10, 12, 11, 12, 12, 13, 12, 13, 13, 10 };
static deque<float> food = { 1.5, 1.5, 1.5, 2.5, 1.5, 3.5, 1.5, 4.5, 1.5, 5.5, 1.5, 6.5, 1.5, 7.5, 1.5, 8.5, 1.5, 9.5, 1.5, 10.5, 1.5, 11.5, 1.5, 12.5, 1.5, 13.5, 2.5, 1.5, 2.5, 6.5, 2.5, 9.5, 2.5, 13.5, 3.5, 1.5, 3.5, 2.5, 3.5, 3.5, 3.5, 4.5, 3.5, 6.5, 3.5, 8.5, 3.5, 9.5, 3.5, 10.5, 3.5, 11.5, 3.5, 13.5, 4.5, 1.5, 4.5, 4.5, 4.5, 5.5, 4.5, 6.5, 4.5, 7.5, 4.5, 8.5, 4.5, 11.5, 4.5, 12.5, 4.5, 13.5, 5.5, 1.5, 5.5, 2.5, 5.5, 5.5, 5.5, 10.5, 5.5, 11.5, 5.5, 13.5, 6.5, 2.5, 6.5, 3.5, 6.5, 4.5, 6.5, 5.5, 6.5, 7.5, 6.5, 10.5, 6.5, 13.5, 7.5, 5.5, 7.5, 6.5, 7.5, 7.5, 7.5, 9.5, 7.5, 10.5, 7.5, 11.5, 7.5, 12.5, 7.5, 13.5, 8.5, 2.5, 8.5, 3.5, 8.5, 4.5, 8.5, 5.5, 8.5, 7.5, 8.5, 10.5, 8.5, 13.5, 9.5, 1.5, 9.5, 2.5, 9.5, 5.5, 9.5, 10.5, 9.5, 11.5, 9.5, 13.5, 10.5, 1.5, 10.5, 4.5, 10.5, 5.5, 10.5, 6.5, 10.5, 7.5, 10.5, 8.5, 10.5, 11.5, 10.5, 12.5, 10.5, 13.5, 11.5, 1.5, 11.5, 2.5, 11.5, 3.5, 11.5, 4.5, 11.5, 6.5, 11.5, 8.5, 11.5, 9.5, 11.5, 10.5, 11.5, 11.5, 11.5, 13.5, 12.5, 1.5, 12.5, 6.5, 12.5, 9.5, 12.5, 13.5, 13.5, 1.5, 13.5, 2.5, 13.5, 3.5, 13.5, 4.5, 13.5, 5.5, 13.5, 6.5, 13.5, 7.5, 13.5, 8.5, 13.5, 9.5, 13.5, 10.5, 13.5, 11.5, 13.5, 12.5, 13.5, 13.5 };
static vector<vector<bool>> bitmap; // 2d image of which squares are blocked and which are clear for pacman to move in 
bool* keyStates = new bool[256]; // record of all keys pressed 
int points = 0; // total points collected
int drawstate = 0; // pacman draw state
int angle = 180; // angle of pacman mouth
clock_t duration = 1; // time of the game
clock_t startTime = 0; // time of the game
int gametime = 0; // time of the game
int currentSound = 0; // current sound being played



//Initializes the game with the appropiate information 
void init(void){
	//clear screen
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
	//reset all keys
	for (int i = 0; i < 256; i++){
		keyStates[i] = false;
	}
	//fill the bitmap with the obstacles
	bitmap.push_back({ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true });
	bitmap.push_back({ true, false, false, false, false, false, false, false, false, false, false, false, false, false, true });
	bitmap.push_back({ true, false, true, true, true, true, false, true, true, false, true, true, true, false, true });
	bitmap.push_back({ true, false, false, false, false, true, false, true, false, false, false, false, true, false, true});
	bitmap.push_back({ true, false, true, true, false, false, false, false, false, true, true, false, false, false, true});
	bitmap.push_back({ true, false, false, true, true, false, true, true, true, true, false, false, true, false, true});
	bitmap.push_back({ true, true, false, false, false, false, true, false, true, true, false, true, true, false, true});
	bitmap.push_back({ true, true, true, true, true, false, false, false, true, false, false, false, false, false, true});
	bitmap.push_back({ true, true, false, false, false, false, true, false, true, true, false, true, true, false, true });
	bitmap.push_back({ true, false, false, true, true, false, true, true, true, true, false, false, true, false, true });
	bitmap.push_back({ true, false, true, true, false, false, false, false, false, true, true, false, false, false, true });
	bitmap.push_back({ true, false, false, false, false, true, false, true, false, false, false, false, true, false, true });
	bitmap.push_back({ true, false, true, true, true, true, false, true, true, false, true, true, true, false, true });
	bitmap.push_back({ true, false, false, false, false, false, false, false, false, false, false, false, false, false, true });
	bitmap.push_back({ true, true, true, true, true, true, true, true, true, true, true, true, true, true, true });
}

void drawRectWithRoundedCorners(float cx,float cy, float dx, float dy, float r,unsigned __int32 rgba){
	int i;
	float x0,y0,x,y;
	static const float sina[45]={0,0.1736482,0.3420201,0.5,0.6427876,0.7660444,0.8660254,0.9396926,0.9848077,1,0.9848078,0.9396927,0.8660255,0.7660446,0.6427878,0.5000002,0.3420205,0.1736485,3.894144E-07,-0.1736478,-0.3420197,-0.4999996,-0.6427872,-0.7660443,-0.8660252,-0.9396925,-0.9848077,-1,-0.9848078,-0.9396928,-0.8660257,-0.7660449,-0.6427881,-0.5000006,-0.3420208,-0.1736489,0,0.1736482,0.3420201,0.5,0.6427876,0.7660444,0.8660254,0.9396926,0.9848077};
	static const float *cosa=sina+9;
	dx-=r+r;
	dy-=r+r;
	glColor4ubv((unsigned __int8*)(&rgba));
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(cx,cy);
	x0=cx+(0.5*dx);
	y0=cy+(0.5*dy);
	for (i=0;i<9;i++)
		{
		x=x0+(r*cosa[i]);
		y=y0+(r*sina[i]);
		glVertex2f(x,y);
		}
	x0-=dx;
	for (;i<18;i++)
		{
		x=x0+(r*cosa[i]);
		y=y0+(r*sina[i]);
		glVertex2f(x,y);
		}
	y0-=dy;
	for (;i<27;i++)
		{
		x=x0+(r*cosa[i]);
		y=y0+(r*sina[i]);
		glVertex2f(x,y);
		}
	x0+=dx;
	for (;i<36;i++)
		{
		x=x0+(r*cosa[i]);
		y=y0+(r*sina[i]);
		glVertex2f(x,y);
		}
	glVertex2f(x,cy+(0.5*dy));
	glEnd();
}

//Method to draw the obstacle course and the walls
void drawLaberynth(){
	glColor3f(0.8, 0.8, 0.8);
	//Border
	for (int i = 0; i < border.size(); i = i + 4){
		glRectf(border.at(i) * squareSize, border.at(i + 1)*squareSize, border.at(i + 2)*squareSize, border.at(i + 3)*squareSize);
	}

	//Obstacles
	for (int j = 0; j < obstaclesBottom.size(); j = j + 4){
		glRectf(obstaclesBottom.at(j) * squareSize, obstaclesBottom.at(j + 1)*squareSize, obstaclesBottom.at(j + 2)*squareSize, obstaclesBottom.at(j + 3)*squareSize);
	}
	for (int k = 0; k < obstaclesMiddle.size(); k = k + 4){
		glRectf(obstaclesMiddle.at(k) * squareSize, obstaclesMiddle.at(k + 1)*squareSize, obstaclesMiddle.at(k + 2)*squareSize, obstaclesMiddle.at(k + 3)*squareSize);
	}
	for (int p = 0; p < obstaclesTop.size(); p = p + 4){
		glRectf(obstaclesTop.at(p) * squareSize, obstaclesTop.at(p + 1)*squareSize, obstaclesTop.at(p + 2)*squareSize, obstaclesTop.at(p + 3)*squareSize);
	}
}

//Method to draw time
void drawTitle(){	
	glColor3f(0.1, 0.2, 0.4);
	glRectf(180, 10, 700, 40);
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(330, 30);
	string str = "PAKMAN";
	for (int i = 0; i < str.size(); i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
	
}

//Method to draw time
void drawTime(){
	gametime = (int)(clock() - startTime) / 1000;
	unsigned __int32 rgba = 0xFF205070;
	drawRectWithRoundedCorners(115, 25, 3 * squareSize, squareSize, 10, rgba);
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(50, 30);
	string str = "Time: " + to_string(gametime);
	for (int i = 0; i < str.size(); i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
	
}

//Method to draw points
void drawPoints(){	
	unsigned __int32 rgba = 0xFF205070;
	drawRectWithRoundedCorners(635, 25, 3 * squareSize, squareSize, 10, rgba);
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(575, 30);
	string str = "Points: " + to_string(points);
	for (int i = 0; i < str.size(); i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
	
}

//Method to check if the food has been eaten
bool foodEaten(int x, int y, float pacmanX, float pacmanY){
	if (x >= pacmanX - 16.0 *cos(359 * M_PI / 180.0) && x <= pacmanX + 16.0*cos(359 * M_PI / 180.0)){
		if (y >= pacmanY - 16.0*cos(359 * M_PI / 180.0) && y <= pacmanY + 16.0*cos(359 * M_PI / 180.0)){
			return true;
		}
	}
	return false;
}

//Method to draw all the food left and delete the ate one
void drawFood(float pacmanX, float pacmanY){
	deque<float> temp;
	//check if the food has not been eaten
	for (int i = 0; i < food.size(); i = i + 2){
		if (!foodEaten(food.at(i)*squareSize, food.at(i + 1)*squareSize, pacmanX, pacmanY)){
			temp.push_back(food.at(i));
			temp.push_back(food.at(i + 1));
		}
		else {
			PlaySound(TEXT("eat.wav"), NULL, SND_ASYNC | SND_FILENAME);
			points++;
		}
	}
	food.swap(temp);
	glPointSize(5.0);
	glBegin(GL_POINTS);
	glColor3f(1.0, 1.0, 1.0);
	//draw all the food avilable
	for (int j = 0; j < food.size(); j = j + 2){
		glVertex2f(food.at(j)*squareSize, food.at(j + 1)*squareSize);
	}
	glEnd();
}

//Method to draw the pacman character through consicutive circle algorithm
void drawPacman(float positionX, float positionY, float rotation){
	int x, y, angle0;
	angle0 = (int)(angle / 6)+1;	
	glBegin(GL_LINES);
	glColor3f(1.0, 1.0, 0.0);
	for (int k = 0; k < 32; k++){
		for (int i = angle0-1; i < (360 - angle0); i++){
			if(i>angle0){ 
				glVertex2f(x, y);
			}
			x = (float)k / 2.0 * cos((i + 90 * rotation) * M_PI / 180.0) + (positionX*squareSize);
			y = (float)k / 2.0* sin((i + 90 * rotation) * M_PI / 180.0) + (positionY*squareSize);
			glVertex2f(x, y);
		}
	}
	glEnd();	
	if(drawstate==0) angle -= 1;
	else angle += 1;
	if(angle==0) drawstate = 1;
	else if(angle==180) drawstate = 0;	
}

//Method to draw the monster character through consecutive circles algorithm
void drawMonster(float positionX, float positionY, float r, float g, float b){
	int x, y;
	glBegin(GL_LINES);
	glColor3f(r, g, b);
	//draw the head
	for (int k = 0; k < 32; k++){
		x = (float)k / 2.0 * cos(360 * M_PI / 180.0) + (positionX*squareSize);
		y = (float)k / 2.0* sin(360 * M_PI / 180.0) + (positionY*squareSize);
		for (int i = 180; i <= 360; i++){
			glVertex2f(x, y);
			x = (float)k / 2.0 * cos(i * M_PI / 180.0) + (positionX*squareSize);
			y = (float)k / 2.0* sin(i * M_PI / 180.0) + (positionY*squareSize);
			glVertex2f(x, y);
		}
	}
	glEnd();	
	//draw body
	glRectf((positionX*squareSize) - 17, positionY*squareSize, (positionX*squareSize) + 15, (positionY*squareSize) + 15);
	glBegin(GL_POINTS);
	glColor3f(0, 0.2, 0.4);
	//draw eyes and legs
	glVertex2f((positionX*squareSize) - 11, (positionY*squareSize) + 14); //legs
	glVertex2f((positionX*squareSize) - 1, (positionY*squareSize) + 14); //legs
	glVertex2f((positionX*squareSize) + 8, (positionY*squareSize) + 14); //legs
	glVertex2f((positionX*squareSize) + 4, (positionY*squareSize) - 3); //eyes
	glVertex2f((positionX*squareSize) - 7, (positionY*squareSize) - 3); //eyes 
	glEnd();
}

//Method to change the direction of the monster
void ghostDirection(float* monster,int current){
	//update the direction of the monster
	float dx = xIncrement - monster[0];
	float dy = yIncrement - monster[1];
	int w[4];
	w[0] = (current==1)?0:((abs(dx) > abs(dy) && dx < 0)? 20:((dx < 0)? 15:(abs(dx) > abs(dy)? 5:3)));
	w[1] = ((current==2)?0:((abs(dx) > abs(dy) && dx > 0)? 20:((dx > 0)? 15:(abs(dx) > abs(dy)? 5:3)))) + w[0];
	w[2] = ((current==3)?0:((abs(dx) < abs(dy) && dy < 0)? 20:((dy < 0)? 15:(abs(dx) < abs(dy)? 5:3)))) + w[1];
	w[3] = ((current==4)?0:((abs(dx) < abs(dy) && dy > 0)? 20:((dy > 0)? 15:(abs(dx) < abs(dy)? 5:3)))) + w[2];
	int rval = rand() % w[3];
	if (rval < w[0]){
		monster[2] = 1;
	}
	else if (rval < w[1]){
		monster[2] = 2;
	}
	else if (rval < w[2]){
		monster[2] = 3;
	}
	else {
		monster[2] = 4;
	}
}

//Special Method to change the direction of the monster
void ghostSPDirection(float* monster){
	int xmQuadrant = (int)monster[0];
	int ymQuadrant = (int)monster[1];
	int xpQuadrant = (int)xIncrement;
	int ypQuadrant = (int)yIncrement;
	float dx = xpQuadrant - xmQuadrant;
	float dy = ypQuadrant - ymQuadrant;
	int sx = dx > 0 ? 1 : -1;
	int sy = dy > 0 ? 1 : -1;	
	if(dy==0){		
		bool check = false;
		while(!check){
			if (bitmap.at(xmQuadrant).at(ymQuadrant)){
				break;
			}
			else {
				if(xmQuadrant == xpQuadrant){					
					monster[3] = sx == 1 ? 2 : 1;
					check = true;
				}
				else {
					xmQuadrant += sx;
				}				
			}
		}
	}
	if(dx==0){		
		bool check = false;
		while(!check){
			if (bitmap.at(xmQuadrant).at(ymQuadrant)){
				break;
			}
			else {
				if(ymQuadrant == ypQuadrant){					
					monster[3] = sy == 1 ? 4 : 3;
					check = true;
				}
				else {
					ymQuadrant += sy;
				}				
			}
		}
	}
}

//Method to update the position of the monsters randomly
void updateMonster(float* monster, int id){
		//find the current position of the monster
		/* int x1Quadrant = (int)((monster[0] - (2/squareSize)) - (16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		int x2Quadrant = (int)((monster[0] + (2/squareSize)) + (16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		int y1Quadrant = (int)((monster[1] - (2/squareSize)) - (16.0 *cos(360 * M_PI / 180.0)) / squareSize);
		int y2Quadrant = (int)((monster[1] + (2/squareSize)) + (16.0 *cos(360 * M_PI / 180.0)) / squareSize); */
		int x1Quadrant = (int)(monster[0] - (25/squareSize));
		int x2Quadrant = (int)(monster[0] + (25/squareSize));
		int y1Quadrant = (int)(monster[1] - (25/squareSize));
		int y2Quadrant = (int)(monster[1] + (25/squareSize));

		ghostSPDirection(monster);

		//move him acording to its direction until he hits an obstacle
		switch ((int)monster[2]){
		case 1:			
			if (!bitmap.at(x1Quadrant).at((int)monster[1])){ 	
				if(monster[3] == 2) { // suddenly change direction if see pacman 
					monster[2] = monster[3];
					monster[3] = 0;					
				} else {
					monster[0] -= duration * 0.11 / squareSize;
				}	
			} else if(monster[3] > 0){
				monster[2] = monster[3];
				monster[3] = 0;
			} else {	
				monster[3] = 0;			
				ghostDirection(monster, 1);
			}
			break;
		case 2:
			if (!bitmap.at(x2Quadrant).at((int)monster[1])){ 	
				if(monster[3] == 1) { // suddenly change direction if see pacman 
					monster[2] = monster[3];
					monster[3] = 0;
				} else {
					monster[0] += duration * 0.11 / squareSize;
				}	
			} else if(monster[3] > 0){
				monster[2] = monster[3];
				monster[3] = 0;
			} else {
				monster[3] = 0;
				ghostDirection(monster, 2);
			}
			break;
		case 3:
			if (!bitmap.at((int)monster[0]).at(y1Quadrant)){
				if(monster[3] == 4) { // suddenly change direction if see pacman 
					monster[2] = monster[3];
					monster[3] = 0;
				} else {
					monster[1] -= duration * 0.11 / squareSize;
				}	
			} else if(monster[3] > 0){
				monster[2] = monster[3];
				monster[3] = 0;
			} else {
				monster[3] = 0;
				ghostDirection(monster, 3);
			}
			break;
		case 4:
			if (!bitmap.at((int)monster[0]).at(y2Quadrant)){
				if(monster[3] == 3) { // suddenly change direction if see pacman 
					monster[2] = monster[3];
					monster[3] = 0;
				} else {
					monster[1] += duration * 0.11 / squareSize;
				}	
			} else if(monster[3] > 0){
				monster[2] = monster[3];
				monster[3] = 0;
			} else {
				monster[3] = 0;		
				ghostDirection(monster, 4);	
			}
			break;
		default:
			break;
		}	
}

//Method to set the pressed key
void keyPressed(unsigned char key, int x, int y){
	keyStates[key] = true;
}

//Method to unset the released key
void keyUp(unsigned char key, int x, int y){
	keyStates[key] = false;
}

//Method to reset all the variable necessaries to start the game again
void resetGame(){	
	over = false;
	xIncrement = 2.5;
	yIncrement = 6.5; 
	rotation = 0;
	monster1 = new float[4] {10.5, 8.5, 1.0, 0};
	monster2 = new float[4] {13.5, 1.5, 2.0, 0};
	monster3 = new float[4] {4.5, 6.5, 3.0, 0};
	monster4 = new float[4] {2.5, 13.5, 4.0, 0};
	points = 0;
	for (int i = 0; i < 256; i++){
		keyStates[i] = false;
	}
	food = { 1.5, 1.5, 1.5, 2.5, 1.5, 3.5, 1.5, 4.5, 1.5, 5.5, 1.5, 6.5, 1.5, 7.5, 1.5, 8.5, 1.5, 9.5, 1.5, 10.5, 1.5, 11.5, 1.5, 12.5, 1.5, 13.5, 2.5, 1.5, 2.5, 6.5, 2.5, 9.5, 2.5, 13.5, 3.5, 1.5, 3.5, 2.5, 3.5, 3.5, 3.5, 4.5, 3.5, 6.5, 3.5, 8.5, 3.5, 9.5, 3.5, 10.5, 3.5, 11.5, 3.5, 13.5, 4.5, 1.5, 4.5, 4.5, 4.5, 5.5, 4.5, 6.5, 4.5, 7.5, 4.5, 8.5, 4.5, 11.5, 4.5, 12.5, 4.5, 13.5, 5.5, 1.5, 5.5, 2.5, 5.5, 5.5, 5.5, 10.5, 5.5, 11.5, 5.5, 13.5, 6.5, 2.5, 6.5, 3.5, 6.5, 4.5, 6.5, 5.5, 6.5, 7.5, 6.5, 10.5, 6.5, 13.5, 7.5, 5.5, 7.5, 6.5, 7.5, 7.5, 7.5, 9.5, 7.5, 10.5, 7.5, 11.5, 7.5, 12.5, 7.5, 13.5, 8.5, 2.5, 8.5, 3.5, 8.5, 4.5, 8.5, 5.5, 8.5, 7.5, 8.5, 10.5, 8.5, 13.5, 9.5, 1.5, 9.5, 2.5, 9.5, 5.5, 9.5, 10.5, 9.5, 11.5, 9.5, 13.5, 10.5, 1.5, 10.5, 4.5, 10.5, 5.5, 10.5, 6.5, 10.5, 7.5, 10.5, 8.5, 10.5, 11.5, 10.5, 12.5, 10.5, 13.5, 11.5, 1.5, 11.5, 2.5, 11.5, 3.5, 11.5, 4.5, 11.5, 6.5, 11.5, 8.5, 11.5, 9.5, 11.5, 10.5, 11.5, 11.5, 11.5, 13.5, 12.5, 1.5, 12.5, 6.5, 12.5, 9.5, 12.5, 13.5, 13.5, 1.5, 13.5, 2.5, 13.5, 3.5, 13.5, 4.5, 13.5, 5.5, 13.5, 6.5, 13.5, 7.5, 13.5, 8.5, 13.5, 9.5, 13.5, 10.5, 13.5, 11.5, 13.5, 12.5, 13.5, 13.5 };
	startTime = clock();
}

//Method to update the movement of the pacman according to the movement keys pressed
void keyOperations(){
	//get current position
	float x = xIncrement * squareSize;
	float y = yIncrement * squareSize;	
	int x1Quadrant = (int)((x - 16.0) / squareSize);
	int x2Quadrant = (int)((x + 16.0) / squareSize);
	int y1Quadrant = (int)((y - 16.0) / squareSize);
	int y2Quadrant = (int)((y + 16.0) / squareSize);
	//update according to keys pressed
	if (keyStates['a']){
		x1Quadrant = (int)((x - ( squareSize/2 + 1) ) / squareSize);
		if(rotation != 2){	
			if(rotation % 2 == 0){
				//snap to grid horizontally
				float checkX = xIncrement - floor(xIncrement) - 0.5;					
				if(abs(checkX)<0.36) xIncrement -= checkX;
			} else{
				//snap to grid vertically
				float checkY = yIncrement - floor(yIncrement) - 0.5;	
				if(abs(checkY)<0.36) yIncrement -= checkY;
			}
			rotation = 2;
		} else if (!bitmap.at(x1Quadrant).at(y1Quadrant) && !bitmap.at(x1Quadrant).at(y2Quadrant)){
			xIncrement -= 5 / squareSize;		
		}
		keyStates['a'] = false;		
	}
	if (keyStates['d']){
		x2Quadrant = (int)((x + ( squareSize/2 + 1)) / squareSize);
		if(rotation != 0){
			if(rotation % 2 == 0){
				//snap to grid horizontally
				float checkX = xIncrement - floor(xIncrement) - 0.5;					
				if(abs(checkX)<0.36) xIncrement -= checkX;
			} else{
				//snap to grid vertically
				float checkY = yIncrement - floor(yIncrement) - 0.5;	
				if(abs(checkY)<0.36) yIncrement -= checkY;
			}
			rotation = 0;
		} else if (!bitmap.at(x2Quadrant).at(y1Quadrant) && !bitmap.at(x2Quadrant).at(y2Quadrant)){
			xIncrement += 5 / squareSize;		
		}
		keyStates['d'] = false;
	}
	if (keyStates['w']){
		y1Quadrant = (int)((y - ( squareSize/2 + 1)) / squareSize);
		if(rotation != 3){
			if(rotation % 2 == 0){
				//snap to grid horizontally
				float checkX = xIncrement - floor(xIncrement) - 0.5;					
				if(abs(checkX)<0.36) xIncrement -= checkX;
			} else{
				//snap to grid vertically
				float checkY = yIncrement - floor(yIncrement) - 0.5;	
				if(abs(checkY)<0.36) yIncrement -= checkY;
			}
			rotation = 3;
		} else if (!bitmap.at(x1Quadrant).at(y1Quadrant) && !bitmap.at(x2Quadrant).at(y1Quadrant)){
			yIncrement -= 5 / squareSize;	
		}
		keyStates['w'] = false;
	}
	if (keyStates['s']){
		y2Quadrant = (int)((y + ( squareSize/2 + 1)) / squareSize);
		if(rotation != 1){
			if(rotation % 2 == 0){
				//snap to grid horizontally
				float checkX = xIncrement - floor(xIncrement) - 0.5;					
				if(abs(checkX)<0.36) xIncrement -= checkX;
			} else{
				//snap to grid vertically
				float checkY = yIncrement - floor(yIncrement) - 0.5;	
				if(abs(checkY)<0.36) yIncrement -= checkY;
			}
			rotation = 1;
		} else if (!bitmap.at(x1Quadrant).at(y2Quadrant) && !bitmap.at(x2Quadrant).at(y2Quadrant)){
			yIncrement += 5 / squareSize;	
		}
		keyStates['s'] = false;
	}
	if (keyStates[' ']){
		if (!replay && over){
			resetGame();
			replay = true;
		}
		else if (replay && over){
			replay = false;
		}
		keyStates[' '] = false;
	}
}

//Method to check if the game is over
void gameOver(){
	int pacmanX = (int)(xIncrement);
	int pacmanY = (int)(yIncrement);
	int monster1X = (int)(monster1[0]);
	int monster1Y = (int)(monster1[1]);
	int monster2X = (int)(monster2[0]);
	int monster2Y = (int)(monster2[1]);
	int monster3X = (int)(monster3[0]);
	int monster3Y = (int)(monster3[1]);
	int monster4X = (int)(monster4[0]);
	int monster4Y = (int)(monster4[1]);
	if ((pacmanX == monster1X && pacmanY == monster1Y)||(pacmanX == monster2X && pacmanY == monster2Y)||
		(pacmanX == monster3X && pacmanY == monster3Y)||(pacmanX == monster4X && pacmanY == monster4Y)){
		over = true;
		currentSound = 2;		
	}
	if (points == 106){
		over = true;
		currentSound = 3;
	}
}

//Method to display the results of the game at the ends
void resultsDisplay(){
	if (points == 106){
		//Won
		char* message = "*************************************";
		glRasterPos2f(170, 250);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "CONGRATULATIONS, YOU WON! ";
		glColor3f(1, 1, 1);
		glRasterPos2f(200, 300);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "*************************************";
		glRasterPos2f(170, 350);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);

		message = "You got : ";
		glRasterPos2f(260, 400);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		string result = to_string(points) + " points";
		message = (char*)result.c_str();
		glRasterPos2f(365, 400);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);	

		message = "Duration: ";
		glRasterPos2f(260, 450);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		result = to_string(gametime) + " seconds";
		message = (char*)result.c_str();
		glRasterPos2f(350, 450);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);

		message = "To start or restart the game, press the space key.";
		glRasterPos2f(170, 550);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
	}else {
		//Lost
		char* message = "*************************";
		glRasterPos2f(210, 250);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "SORRY, YOU LOST!";
		glColor3f(1, 1, 1);
		glRasterPos2f(250, 300);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "*************************";
		glRasterPos2f(210, 350);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		message = "You got : ";
		glRasterPos2f(260, 400);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		string result = to_string(points) + " points";
		message = (char*)result.c_str();
		glRasterPos2f(365, 400);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
					
		message = "Duration: ";
		glRasterPos2f(260, 450);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
		result = to_string(gametime) + " seconds";
		message = (char*)result.c_str();
		glRasterPos2f(365, 450);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);

		message = "To start or restart the game, press the space key.";
		glRasterPos2f(170, 550);
		while (*message)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
	}
}

//Method to display the starting instructions
void welcomeScreen(){
	glClearColor(0, 0.2, 0.4, 1.0);
	char* message = "*************************************";
	glRasterPos2f(150, 200);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "PAKMAN - ABYUNALVIN";
	glColor3f(1, 1, 1);
	glRasterPos2f(225, 250);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "*************************************";
	glRasterPos2f(150, 300);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *message++);
	message = "To control Pacman use A to go right, D to go left, W to go up and S to go down.";
	glRasterPos2f(50, 400);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
	message = "To start or restart the game, press the space key.";
	glRasterPos2f(170, 450);
	while (*message)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *message++);
}

//Method to display the screen and its elements
void display(){	
	keyOperations();
	glClear(GL_COLOR_BUFFER_BIT);
	gameOver();
	if (replay){
		if (!over){			
			clock_t prevtime = clock();		
			currentSound = 0;	
			drawLaberynth();
			drawFood(xIncrement * squareSize, yIncrement * squareSize);
			drawPacman(xIncrement, yIncrement, rotation);
			updateMonster(monster1, 1);
			updateMonster(monster2, 2);
			updateMonster(monster3, 3);
			updateMonster(monster4, 4);
			drawMonster(monster1[0], monster1[1], 0.0, 1.0, 1.0); //cyan
			drawMonster(monster2[0], monster2[1], 1.0, 0.0, 0.0); //red
			drawMonster(monster3[0], monster3[1], 1.0, 0.0, 0.6); //magenta
			drawMonster(monster4[0], monster4[1], 1.0, 0.3, 0.0); //orange	
			drawTitle();
			drawTime();
			drawPoints();
			duration = clock();		
			duration = duration - prevtime;		
		}
		else {
			resultsDisplay();
		}
	}
	else {		
		currentSound = 1;	
		welcomeScreen();
	}
	glutSwapBuffers();
}

//Methdo to reshape the game is the screen size changes
void reshape(int w, int h){
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glOrtho(0, 750, 750, 0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void processSounds(){
	int prevSound = 0;
	while(true){
		if(currentSound != prevSound){
			switch (currentSound){
			case 1:
				PlaySound(NULL, NULL, SND_ASYNC);
				PlaySound(TEXT("start_finish.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
				break;
			case 2:
				PlaySound(NULL, NULL, SND_ASYNC);
				PlaySound(TEXT("lost.wav"), NULL, SND_ASYNC | SND_FILENAME);
				break;		
			case 3:
				PlaySound(NULL, NULL, SND_ASYNC);
				PlaySound(TEXT("win.wav"), NULL, SND_ASYNC | SND_FILENAME);
				break;			
			default:
				PlaySound(NULL, NULL, SND_ASYNC);
				break;
			}
			prevSound = currentSound;				
		}
		Sleep(1000);
	}
}


//Main functions that controls the running of the game
int main(int argc, char** argv){
	//initialize and create the screen
	std::thread t1([]() {
		processSounds();
	});

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(750, 750);
	glutInitWindowPosition(500, 50);
	glutCreateWindow("PAKMAN - ABYUNALVIN");

	//define all the control functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutKeyboardFunc(keyPressed);
	//glutKeyboardFunc(keyUp);
	

	//run the game
	init();
	glutMainLoop();
	t1.join();
	return 0;
}