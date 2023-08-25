//author£ºXianyao Li 1824764
/*
Scene description: In an oceanview room, there is a table,
a rotating chair, a scalable teapot, a bookcase, books, an open cabinet, a bed, a rotating lamp,
a 12-edge stone, a dynamic planetary system as decoration, a moving person, and four pictures made
by texture mapping.

Interactive buttons: E moves the view to the right, Q moves the view to the left,
 A moves camera to the left,  D moves camera to the right,  S moves camera backward and  W moves camera forward.
 left mouse button opens the planetary system,  right mouse button closes the planetary system, V opens the chair rotation, C closes the chair
rotation, O opens the teapot zooming, P closes the teapot rotation, H opens the cabinet drawer, B closes the
cabinet drawer, G opens the character movement, T closes the character movement.
*/
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#include <stdlib.h>
#include<math.h>
#include <stdio.h>
#include "vector"
GLboolean Flag = true;
GLboolean Flag1 = false;
GLboolean Flag2 = false;
GLboolean Flag3 = false;
GLboolean Flag4 = true;
GLboolean right;
GLboolean left;
GLboolean handright;
GLboolean handleft;

// set light
GLfloat light1_position[4] = { -1,1,-1,0 };
GLfloat light2_position[4] = { 1,-1,1,0 };
GLfloat ambientlight[4] = { 0.5,0.5,0.5,0.5 };
GLfloat diffuselight[4] = { 0.6,0.6,0.5,0.5 };
GLfloat specularlight[4] = { 0.5,0.5,0.5,0.5 };

// set material
GLfloat mat_ambient[4] = { 0.0,0.0,0.0,1.0 };
GLfloat mat_diffuse[4] = { 0.6,0.6,0.6,0.6 };
GLfloat mat_specular[4] = { 0.2,0.2,0.2,0.8 };
GLfloat mat_shininess[1] = { 0.0 };
using namespace std;

// set texture mapping
GLint imagewidth0, imagewidth1, imagewidth2, imagewidth3, imagewidth4, imagewidth5, imagewidth6;
GLint imageheight0, imageheight1, imageheight2, imageheight3, imageheight4, imageheight5, imageheight6;
GLint pixellength0, pixellength1, pixellength2, pixellength3, pixellength4, pixellength5, pixellength6;
vector<GLubyte*>p;
GLuint texture[7];

//Field Of View
float fltFOV = 70;

//Camera position
float fltX0 = 0.0;
float fltY0 = 0.0;
float fltZ0 = 0.0;

//Look At reference point
float fltXRef = 0.0;
float fltYRef = 0.0;
float fltZRef = 0.0;

//Up vector
float fltXUp = 0.0;
float fltYUp = 1.0;
float fltZUp = 0.0;
//Set dynamic parameters

float people1 = 0;
float peopleangle = -60;
float ball = 0;
float box = 0;
float teapot = 1;
float chair = 0;
float num1 = 360;

//set lighting and material

void myinit1(void) {

	//light1 position
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientlight);

	// Diffuse light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuselight);

	// Specular light
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularlight);

	// Material fuzzy light
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	// The material reflects light
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	// Material mirror light
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	// Material mirror index
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//Confirm light properties
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	//light2 position
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_AMBIENT, ambientlight);

	// Diffuse light
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuselight);

	// Specular light
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularlight);

	// Material fuzzy light
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);

	// The material reflects light
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	// Material mirror light
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	// Material mirror index
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//Confirm light properties
	glEnable(GL_LIGHT2);
	glEnable(GL_DEPTH_TEST);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
}

//Set readimage
void ReadImage(const char path[256], GLint& imagewidth, GLint& imageheight, GLint& pixellength)
{
	GLubyte* pixeldata;
	FILE* pfile;
	fopen_s(&pfile, path, "rb");
	if (pfile == 0) exit(0);
	fseek(pfile, 0x0012, SEEK_SET);
	fread(&imagewidth, sizeof(imagewidth), 1, pfile);
	fread(&imageheight, sizeof(imageheight), 1, pfile);
	pixellength = imagewidth * 3;
	while (pixellength % 4 != 0)pixellength++;
	pixellength *= imageheight;
	pixeldata = (GLubyte*)malloc(pixellength);
	if (pixeldata == 0) exit(0);
	fseek(pfile, 54, SEEK_SET);
	fread(pixeldata, pixellength, 1, pfile);
	p.push_back(pixeldata);
	fclose(pfile);
}

//readimage there are 6 picture
void myinit(void)
{
	glEnable(GL_TEXTURE_2D);

	//avoid repeat to read
	if (texture[0] == NULL) {
		glEnable(GL_TEXTURE_2D);
		ReadImage("1.bmp", imagewidth0, imageheight0, pixellength0);
		ReadImage("2.bmp", imagewidth1, imageheight1, pixellength1);
		ReadImage("3.bmp", imagewidth2, imageheight2, pixellength2);
		ReadImage("4.bmp", imagewidth3, imageheight3, pixellength3);
		ReadImage("5.bmp", imagewidth4, imageheight4, pixellength4);
		ReadImage("6.bmp", imagewidth5, imageheight5, pixellength5);
		ReadImage("7.bmp", imagewidth6, imageheight6, pixellength6);

		// set pixel storage mode (in the memory)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// number of texture names to be generated and array of texture names
		glGenTextures(6, &texture[0]);

		// target to which texture is bound and name of a texture
		//image1
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth0, imageheight0, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[0]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		//image2
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth1, imageheight1, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[1]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		//image3
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth2, imageheight2, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[2]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		//image4
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth3, imageheight3, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[3]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		//image5
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth4, imageheight4, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[4]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

		//image6
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth5, imageheight5, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[5]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		//image7
		glBindTexture(GL_TEXTURE_2D, texture[6]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imagewidth6, imageheight6, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, p[6]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
}

//set animate paramete
void animateTracks()
{

	//The speed of rotation of the lamp
	ball = ball + 1;

	// incremental angle per frame while 60 frames/second (so very fast)
	//The rate of rotation of the planetary system
	if (Flag == TRUE) {
		num1 += 1.5;
	}
	else if (Flag == FALSE) {

		//When the planetary system is shut down, the rotation speed is zero
		num1 += 0;
	}

	//seat rotation
	if (Flag1 == TRUE) {
		chair = chair + 0.6;
	}
	else if (Flag1 == FALSE) {
		chair = chair + 0;
	}

	//The teapot zoom
	if (Flag2 == TRUE) {
		if (teapot < 2) {
			teapot = teapot + 0.005;
			if (teapot > 1.8) {
				teapot = 1;
			}
		}
	}
	else if (Flag2 == FALSE) {
		teapot = teapot + 0;
	}

	//Bulb rotation
	if (Flag3 == TRUE) {
		if (box < 350) {

			box = box + 5;
		}
	}
	else if (Flag3 == FALSE) {
		if (box > 0) {
			box = box - 5;
		}
	}

	//people movement (from 0 to 1000)
	if (Flag4 == TRUE) {
		if (people1 <= 0) {
			right = true;
			left = false;
		}
		if (people1 >= 1000) {
			right = false;
			left = true;
		}
		if (peopleangle <= -60) {
			handright = true;
			handleft = false;
		}
		if (peopleangle >= 60) {
			handright = false;
			handleft = true;
		}
		if (left)
			people1 = people1 - 1.5;
		if (right)
			people1 = people1 + 1.5;
		if (handright)
			peopleangle = peopleangle + 1;
		if (handleft)
			peopleangle = peopleangle - 1;
	}
	else if (Flag4 == FALSE) {
		people1 = 0;
		peopleangle = 0;
	}
	glutPostRedisplay();
}

//Start texture mapping(Corresponding picture four vertices, photo width is a multiple of 12)
void ground() {
	myinit();
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2000, 1000, -2480);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2000, 2400, -2480);
	glTexCoord2f(1.0, 1.0); glVertex3f(-1100, 2400, -2480.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1100, 1000, -2480.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2500, 0, 2500);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2500, 2500, 2500);
	glTexCoord2f(1.0, 1.0); glVertex3f(-2500, 2500, -2500.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-2500, 0, -2500.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2500, 0, -2500);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2500, 0, 2500);
	glTexCoord2f(1.0, 1.0); glVertex3f(2500, 0, 2500);
	glTexCoord2f(1.0, 0.0); glVertex3f(2500, 0, -2500.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2400, 1000, 2480);
	glTexCoord2f(0.0, 1.0); glVertex3f(-2400, 2200, 2480);
	glTexCoord2f(1.0, 1.0); glVertex3f(-600, 2200, 2480.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(-600, 1000, 2480.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(2500, 2490, 2500);
	glTexCoord2f(0.0, 1.0); glVertex3f(2500, 2490, -2500);
	glTexCoord2f(1.0, 1.0); glVertex3f(-2500, 2490, -2500);
	glTexCoord2f(1.0, 0.0); glVertex3f(-2500, 2490, 2500);
	glEnd();

	//Mapping on the sphere
	glPushMatrix();
	glTranslatef(-1000, 2200, 0);
	glRotatef(num1, 0, 1, 0);
	glPushMatrix();
	glPushMatrix();
	glScalef(2, 2, 2);
	glBindTexture(GL_TEXTURE_2D, texture[6]);
	// Create a quadric surface object
	GLUquadric* quadricObj1 = gluNewQuadric();
	// Set texture mode to true
	gluQuadricTexture(quadricObj1, GL_TRUE);
	// object must be deleted or it will be created every call of the
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	gluSphere(quadricObj1, 80, 80, 80);
	gluDeleteQuadric(quadricObj1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//Mapping on the sphere
	glPushMatrix();
	glTranslatef(-200, 400, -1500);
	glPushMatrix();
	glRotatef(ball, 0, 1, 0);
	glPushMatrix();
	glScalef(2, 2, 2);
	glBindTexture(GL_TEXTURE_2D, texture[5]);

	// Create a quadric surface object
	GLUquadric* quadricObj = gluNewQuadric();

	// Set texture mode to true
	gluQuadricTexture(quadricObj, GL_TRUE);

	// object must be deleted or it will be created every call of the
	gluSphere(quadricObj, 80, 80, 80);
	gluDeleteQuadric(quadricObj);

	// function and memory taken!
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glColor3f(0.72, 0.5, 0.04);
	glutSolidCylinder(40, 400, 10, 10);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	//Draw a wire, more realistic
	GLfloat curSizeLine = 5;
	glLineWidth(curSizeLine);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(-200, 0, -1500);
	glVertex3f(-200, 0, -2500);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

//Draw the illumination material model

void light() {
	glPushMatrix();
	glTranslatef(-1000, 2200, 0);
	glRotatef(num1, 0, 1, 0);
	glPushMatrix();
	glTranslatef(340, 0, 0);
	glPushMatrix();
	glScalef(1, 1, 1);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glColor3f(1, 0.8, 0);
	glutSolidSphere(80, 80, 80);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//========Walls ========
	glBegin(GL_QUADS);
	glColor3f(1, 1, 0.87);
	glVertex3f(2500, 0, 2500);
	glVertex3f(2500, 2500, 2500);
	glVertex3f(-2500, 2500, 2500.0);
	glVertex3f(-2500, 0, 2500.0);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(1, 1, 0.87);
	glVertex3f(-2500, 0, -2500);
	glVertex3f(-2500, 2500, -2500);
	glVertex3f(2500, 2500, -2500.0);
	glVertex3f(2500, 0, -2500.0);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(1, 1, 0.87);
	glVertex3f(2500, 0, 2500);
	glVertex3f(2500, 2500, 2500);
	glVertex3f(2500, 2500, -2500.0);
	glVertex3f(2500, 0, -2500.0);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(1, 0.9, 0.87);
	glVertex3f(-2400, 2500, 2500);
	glVertex3f(-2400, 2500, -2500);
	glVertex3f(-2400, 2200, -2500.0);
	glVertex3f(-2400, 2200, 2500.0);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(1, 0.9, 0.87);
	glVertex3f(-2400, 300, 2500);
	glVertex3f(-2400, 300, -2500);
	glVertex3f(-2400, 0, -2500.0);
	glVertex3f(-2400, 0, 2500.0);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(1, 0.9, 0.87);
	glVertex3f(-2400, 2500, 2500);
	glVertex3f(-2400, 2200, 2200);
	glVertex3f(-2400, 0, 2200.0);
	glVertex3f(-2400, 0, 2500.0);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(1, 0.9, 0.87);
	glVertex3f(-2400, 2500, -2500);
	glVertex3f(-2400, 2200, -2200);
	glVertex3f(-2400, 0, -2200.0);
	glVertex3f(-2400, 0, -2500.0);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(0.9, 1, 1);
	glVertex3f(2500, 2500, 2500);
	glVertex3f(-2500, 2500, 2500);
	glVertex3f(-2500, 2500, -2500.0);
	glVertex3f(2500, 2500, -2500.0);
	glEnd();

	//========bed ========
	glPushMatrix();
	glTranslatef(-2000.0, 0.0, 2400);
	glPushMatrix();
	glScalef(1, 2, 0.1);
	glColor3f(139 / 255, 115 / 255, 85 / 255);
	glutSolidCube(500);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, -650);
	glPushMatrix();
	glScalef(1, 1, 2.5);
	glColor3f(0.6, 0.2, 0.0);
	glutSolidCube(500);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 300, -90);
	glPushMatrix();
	glScalef(1, 0.2, 0.2);
	glColor3f(0.2, 0.4, 0.5);
	glutSolidCube(500);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 300, -700);
	glPushMatrix();
	glScalef(1, 0.15, 2);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidCube(500);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//========bookrack ========
	//Overall translation data
	glPushMatrix();
	glTranslatef(-2000, 0.0, -2400);
	glPushMatrix();
	glScalef(0.05, 5, 0.5);
	glColor3f(139 / 255, 115 / 255, 85 / 255);
	glutSolidCube(500);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 1000.0, 0);
	glPushMatrix();
	glScalef(0.1, 0.7, 0.4);

	//==============books===========
	for (int i = 0;i < 10;i = i + 2) {
		glPushMatrix();
		glTranslatef(0 * i, 0, 0);
		glColor3f(0.1, 0.1, 0.0);
		glutWireCube(500);
		glutSolidCube(500);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(500 * i, 0, 0);
		glColor3f(0.1, 0.4, 0.0);
		glutWireCube(500);
		glutSolidCube(500);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(1000 * i, 0, 0);
		glColor3f(0.3, 0.5, 0.0);
		glutWireCube(500);
		glutSolidCube(500);
		glPopMatrix();
	}

	//Bookcase grid
	glPushMatrix();
	glTranslatef(4500, 300.0, 0);
	glPushMatrix();
	glScalef(20, 0.1, 1);
	glColor3f(139 / 255, 115 / 255, 85 / 255);
	glutSolidCube(500);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(4500, -300.0, 0);
	glPushMatrix();
	glScalef(20, 0.1, 1);
	glColor3f(139 / 255, 115 / 255, 85 / 255);
	glutSolidCube(500);
	glPopMatrix();
	glPopMatrix();
	for (int i = 2;i < 10;i = i + 2) {
		glPushMatrix();
		glTranslatef(0 * i, -1200, 0);
		glColor3f(0.1, 0.1, 0.3);
		glutWireCube(500);
		glutSolidCube(500);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(500 * i, -1200, 0);
		glColor3f(0.6, 0.4, 0.1);
		glutWireCube(500);
		glutSolidCube(500);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(1000 * i, -1200, 0);
		glColor3f(0.8, 0.5, 0.8);
		glutWireCube(500);
		glutSolidCube(500);
		glPopMatrix();
	}
	for (int i = 2;i < 10;i = i + 2) {
		glPushMatrix();
		glTranslatef(0 * i, -600, 0);
		glColor3f(0.7, 0.1, 0.5);
		glutWireCube(500);
		glutSolidCube(500);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(500 * i, -600, 0);
		glColor3f(0.5, 0.4, 0.4);
		glutWireCube(500);
		glutSolidCube(500);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(1000 * i, -600, 0);
		glColor3f(0.3, 0.9, 0.3);
		glutWireCube(500);
		glutSolidCube(500);
		glPopMatrix();
	}

	//Bookcase grid
	glPushMatrix();
	glTranslatef(4500, -900.0, 0);
	glPushMatrix();
	glScalef(20, 0.1, 1);
	glColor3f(139 / 255, 115 / 255, 85 / 255);
	glutSolidCube(500);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(1000, 0.0, 0);
	glPushMatrix();
	glScalef(0.05, 5, 0.5);
	glColor3f(139 / 255, 115 / 255, 85 / 255);
	glutSolidCube(500);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//======some tiny thing=====
	glPushMatrix();
	glTranslatef(-2400, 50, -1800);
	glPushMatrix();
	glScalef(100, 100, 100);
	glColor3f(1, 0.84, 0);
	glutSolidRhombicDodecahedron();
	glPopMatrix();
	glPopMatrix();

	//=======table======
	glPushMatrix();
	glTranslatef(-1000, 500, -500);
	glPushMatrix();
	glScalef(1.5, 0.1, 1.5);
	glColor3f(0.72, 0.5, 0.04);
	glutSolidCube(500);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -500, 0);
	glPushMatrix();
	glScalef(1, 0.05, 1);
	glColor3f(0, 0, 0);
	glutSolidCube(500);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glColor3f(0.72, 0.5, 0.04);
	glutSolidCylinder(80, 800, 10, 10);
	glPopMatrix();

	// ====a teapot above the table====
	glPushMatrix();
	glTranslatef(0, 100, 0);
	glPushMatrix();
	glScalef(100 * teapot, 100 * teapot, 100 * teapot);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glColor3f(0.72, 0.5, 0.04);
	glutSolidTeapot(1);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	// ====chair====
	glPushMatrix();
	glTranslatef(150, -200, 700);
	glPushMatrix();
	glRotatef(chair, 0, 1, 0);
	glPushMatrix();
	glScalef(1, 0.05, 1);
	glColor3f(1, 0, 0);
	glutSolidCube(300);
	glPopMatrix();
	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glColor3f(0.5, 0.5, 0.5);
	glutSolidCylinder(60, 500, 10, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 150, 150);
	glPushMatrix();
	glScalef(1, 1, 0.05);
	glColor3f(1, 0, 0);
	glutSolidCube(300);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//=====box======
	glPushMatrix();
	glTranslatef(-1500, 0, 2500);
	glPushMatrix();
	glScalef(1, 2, 1);
	glColor3f(0.54, 0.45, 0.33);
	glutSolidCube(500);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 400, -box - 75);
	glPushMatrix();
	glScalef(0.8, 0.2, 0.8);
	glColor3f(1, 0.8, 0);
	glutSolidCube(500);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 150, -box - 100);
	glPushMatrix();
	glScalef(0.8, 0.2, 0.8);
	glColor3f(1, 0.8, 0);
	glutSolidCube(500);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//=====people=====
	glPushMatrix();
	glTranslatef(-700, -200, people1 + 600);
	glPushMatrix();
	glTranslatef(0, 900, 0);
	glPushMatrix();
	glScalef(0.8, 1, 0.8);
	glColor3f(1, 0.8, 0);
	glutSolidCube(100);
	glPopMatrix();
	glPopMatrix();

	//hair
	glPushMatrix();
	glTranslatef(0, 960, 0);
	glPushMatrix();
	glScalef(0.8, 0.2, 0.8);
	glColor3f(0, 0, 0);
	glutSolidCube(100);
	glPopMatrix();
	glPopMatrix();

	//body
	glPushMatrix();
	glTranslatef(0, 675, 0);
	glPushMatrix();
	glScalef(2.5, 3.5, 0.8);
	glColor3f(0, 0, 1);
	glutSolidCube(100);
	glPopMatrix();
	glPopMatrix();

	//arm
	glPushMatrix();
	glTranslatef(155, 775, 0);
	glPushMatrix();
	glRotatef(peopleangle, 1, 0, 0);
	glPushMatrix();
	glScalef(0.625, 1.5, 0.8);
	glColor3f(0, 0, 1);
	glutSolidCube(100);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -160, 0);
	glPushMatrix();
	glScalef(0.625, 1.75, 0.8);
	glColor3f(1, 0.8, 0);
	glutSolidCube(100);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-155, 775, 0);
	glPushMatrix();
	glRotatef(-peopleangle, 1, 0, 0);
	glPushMatrix();
	glScalef(0.625, 1.5, 0.8);
	glColor3f(0, 0, 1);
	glutSolidCube(100);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -160, 0);
	glPushMatrix();
	glScalef(0.625, 1.75, 0.8);
	glColor3f(1, 0.8, 0);
	glutSolidCube(100);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	//leg
	glPushMatrix();
	glTranslatef(77.5, 475, 0);
	glPushMatrix();
	glRotatef(-peopleangle / 2, 1, 0, 0);
	glPushMatrix();
	glScalef(0.625, 1.5, 0.8);
	glColor3f(0, 1, 1);
	glutSolidCube(100);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -160, 0);
	glPushMatrix();
	glScalef(0.625, 1.75, 0.8);
	glColor3f(1, 0.8, 0.1);
	glutSolidCube(100);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-77.5, 475, 0);
	glPushMatrix();
	glRotatef(peopleangle / 2, 1, 0, 0);
	glPushMatrix();
	glScalef(0.625, 1.5, 0.8);
	glColor3f(0, 1, 1);
	glutSolidCube(100);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, -160, 0);
	glPushMatrix();
	glScalef(0.625, 1.75, 0.8);
	glColor3f(1, 0.8, 0.1);
	glutSolidCube(100);
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();


}

void displayObject()
{
	//Initializing projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Initialize the curtain
	gluPerspective(fltFOV, 1, 0.1, 5000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Set the camera
	gluLookAt(fltX0 + 2000, 1000, fltZ0, fltX0 + fltY0 - 7000, fltYRef, fltZ0 + fltY0, fltXUp, fltYUp, fltZUp);
	glEnable(GL_DEPTH_TEST);

	//Set the background color
	glClearColor(0.47, 0.77, 0.8, 0.0);
	//Clear the color cache
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Load the background
	ground();
	glEnable(GL_LIGHTING);
	light();
	glDisable(GL_LIGHTING);
	glutSwapBuffers();
}
void reshapeWindow(GLint intNewWidth, GLint intNewHeight)
{
	glViewport(0, 0, intNewWidth, intNewHeight);
}
void adjustDisplay(unsigned char key, int x, int y)
{
	int i;
	//Camera turn right
	if (key == 'e' || key == 'E')
		if (fltY0 > -1500)
			fltY0 -= 150;
	//Camera turn left
	if (key == 'q' || key == 'Q')
		if (fltY0 < 1000)
			fltY0 += 150;
	//Camera move to the left
	if (key == 'a' || key == 'A')
		if (fltZ0 < 1000)
			fltZ0 += 10;
	//Camera move to the right
	if (key == 'd' || key == 'D')
		if (fltZ0 > -1000)
			fltZ0 -= 10;
	//Camera move to the back
	if (key == 's' || key == 'S')
		if (fltX0 < 10)
			fltX0 += 10;
	//Camera move to the forward
	if (key == 'w' || key == 'W')
		if (fltX0 > -1000)
			fltX0 -= 10;
	//opens the planetary system
	if (key == 'm' || key == 'M')
		if (Flag == FALSE) {
			Flag = TRUE;
		}
	// closes the planetary
	if (key == 'n' || key == 'N')
		if (Flag == TRUE) {
			Flag = FALSE;
		}
	//opens the chair rotation
	if (key == 'v' || key == 'V')
		if (Flag1 == FALSE) {
			Flag1 = TRUE;
		}
	//closes the chair rotation
	if (key == 'c' || key == 'C')
		if (Flag1 == TRUE) {
			Flag1 = FALSE;
		}
	//opens the teapot zooming
	if (key == 'o' || key == 'O')
		if (Flag2 == FALSE) {
			Flag2 = TRUE;
		}
	//closes the teapot rotation
	if (key == 'p' || key == 'P')
		if (Flag2 == TRUE) {
			Flag2 = FALSE;
		}
	//opens the cabinet drawer
	if (key == 'h' || key == 'H')
		if (Flag3 == FALSE) {
			Flag3 = TRUE;
		}
	//closes the cabinet drawer
	if (key == 'b' || key == 'B')
		if (Flag3 == TRUE) {
			Flag3 = FALSE;
		}
	//opens the character movement
	if (key == 'g' || key == 'G')
		if (Flag4 == FALSE) {
			Flag4 = TRUE;
		}
	//closes the character movement
	if (key == 't' || key == 'T')
		if (Flag4 == TRUE) {
			Flag4 = FALSE;
		}
	if (key == '9' || key == '9')
		if (ambientlight[0] > 0)
			for (i = 0; i < 4; ++i)
			{
				ambientlight[i] -= 0.1;
				diffuselight[i] -= 0.1;
				specularlight[i] -= 0.1;
			}
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHTING);
	glutPostRedisplay();
}
void mouse_input(int button, int state, int x, int y) { // mouse interaction
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		//opens the planetary system

		if (Flag == FALSE) {
			Flag = TRUE;
		}
	}
	if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
		// closes the planetary

		if (Flag == TRUE) {
			Flag = FALSE;
		}
	}
	glutPostRedisplay();
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(150, 150);
	//Set window size
	glutInitWindowSize(800, 600);
	glutCreateWindow("CW2 ");
	glutDisplayFunc(displayObject);
	myinit1();
	glutReshapeFunc(reshapeWindow);
	glutKeyboardFunc(adjustDisplay);
	glutMouseFunc(mouse_input);
	glutIdleFunc(animateTracks);
	glutMainLoop();
}