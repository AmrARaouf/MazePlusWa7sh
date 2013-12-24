#include "stdafx.h"
//#include <cstdlib>
#include "glew.h"
#include "glaux.h"
#include "gl/glut.h"
#include <iostream>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "GLAUX.lib")

using namespace std;

float sxs[] = {0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 10.8f, 1.8f, 1.8f, 0.6f, 4.0f, 3.4f, 3.0f, 4.2f, 10.2f};
float szs[] = {11.2f, 2.6f, 6.0f, 6.0f, 1.8f, 1.8f, 10.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f};
float txs[] = {0.0f, 1.8f, 3.8f, 4.8f, 7.8f, 8.8f, 10.8f, 0.2f, 0.2f, 0.2f, 4.2f, 6.8f, 0.2f, 7.8f, 1.8f, 1.0f};
float tzs[] = {0.0f, 8.2f, 1.0f, 1.0f, 6.0f, 2.0f, 0.4f, 0.0f, 1.8f, 3.8f, 3.8f, 3.8f, 5.8f, 7.8f, 7.8f, 10.8f};

bool hp[] = {true, true, true};
float hpx[] = {0.6f, 2.4f, 4.4f};
float hpz[] = {3.4f, 8.4f, 4.4f};

float px = 10.4f, py=0.5f, pz = 0.7f;
float lookatX = -1.0f, lookatY = 0.0f, lookatZ = 0.0f;
float health = 100, maxHealth = 100;

float lastx=500, lasty=250;
float angle = 0.0f;

const int wa7shX=0.0, wa7shZ=15.0;
bool bulletWa7shFired = false;
float bulletWa7shX, bulletWa7shZ, lastpx, lastpz;


UINT textureID;

void CreateFromBMP(UINT *textureID, LPSTR strFileName) {
	AUX_RGBImageRec *pBitmap = NULL;
	FILE *pFile = NULL;           
	// The File Handle we will use to read the bitmap
	fopen_s(&pFile, strFileName,"r"); // Check To See If The File Exists
	if(pFile) {
		// If we have a valid file pointer we found the file
		pBitmap = auxDIBImageLoadA(strFileName);  // Load the bitmap and store the data
	} else {          
	// If we can't find the file, quit! Prompt the error message
		MessageBoxA(NULL, "Couldn't find a texture!", "Error!", MB_OK);
		exit(0);
	}
	// Generate a texture with the associative texture ID stored in the array
	glGenTextures(1, textureID);
	// Bind the texture to the texture arrays index and init the texture
	glBindTexture(GL_TEXTURE_2D, *textureID);
	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, 
	pBitmap->data);
	// Lastly, we need to tell OpenGL the quality of our texture map. 
	GL_LINEAR_MIPMAP_LINEAR;
	// is the smoothest. GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated. Good for slower computers though. 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	// The default GL_TEXTURE_WRAP_S and ""_WRAP_T property is GL_REPEAT.
	// We need to turn this to GL_CLAMP_TO_EDGE, otherwise it creates ugly seems
	// in our sky box. GL_CLAMP_TO_EDGE does not repeat when bound to an object.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// Now we need to free the bitmap data that we loaded since openGL stored it as a texture
	if (pBitmap) { // If we loaded the bitmap
		if (pBitmap->data) { //if there is texture data
			free(pBitmap->data);// Free the texture data, we don't need it anymore
		}
		free(pBitmap); // Free the bitmap structure
	}
}

void brick() {
		glBegin(GL_QUADS);
		//face 1
		glTexCoord2f(0,0);
		glVertex3f(1,0,1);
		glTexCoord2f(1,0);
		glVertex3f(1,0,0);
		glTexCoord2f(1,1);
		glVertex3f(1,1,0);
		glTexCoord2f(0,1);
		glVertex3f(1,1,1);
		//face 2
		glTexCoord2f(0,0);
		glVertex3f(0,0,1);
		glTexCoord2f(1,0);
		glVertex3f(1,0,1);
		glTexCoord2f(1,1);
		glVertex3f(1,1,1);
		glTexCoord2f(0,1);
		glVertex3f(0,1,1);
		//face 3
		glTexCoord2f(0,0);
		glVertex3f(1,1,1);
		glTexCoord2f(1,0);
		glVertex3f(1,1,0);
		glTexCoord2f(1,1);
		glVertex3f(0,1,0);
		glTexCoord2f(0,1);
		glVertex3f(0,1,1);
		//face 4
		glTexCoord2f(0,0);
		glVertex3f(0,0,0);
		glTexCoord2f(0,1);
		glVertex3f(0,1,0);
		glTexCoord2f(1,1);
		glVertex3f(1,1,0);
		glTexCoord2f(1,0);
		glVertex3f(1,0,0);
		//face 5
		glTexCoord2f(0,0);
		glVertex3f(0,0,1);
		glTexCoord2f(0,1);
		glVertex3f(0,1,1);
		glTexCoord2f(1,1);
		glVertex3f(0,1,0);
		glTexCoord2f(1,0);
		glVertex3f(0,0,0);
		//face 6
		glTexCoord2f(0,0);
		glVertex3f(1,0,1);
		glTexCoord2f(0,1);
		glVertex3f(0,0,1);
		glTexCoord2f(1,1);
		glVertex3f(0,0,0);
		glTexCoord2f(1,0);
		glVertex3f(1,0,0);
	glEnd();
}

void drawWall(float sx, float sz, float tx, float tz) {
	GLfloat mat_diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	glTranslatef(tx, 0, tz);
	glScalef(sx, 1, sz);
	brick();
	glPopMatrix();
}

void drawFloor() {
	GLfloat mat_diffuse[] = { 0.5f, 0.3f, 0.15f, 0.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	glScalef(11.2,0.01,11.2);
	glTranslatef(0.5,0.5,0.5);
	glutSolidCube(1);
	glPopMatrix();
}

void drawHealthBar() {
	float factor = 1;
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2f(50,450);
	glVertex2f(75,450);
	glVertex2f(75, 450 - (maxHealth * factor));
	glVertex2f(50, 450 - (maxHealth * factor));
	glVertex2f(50, 450);
	glEnd();
	glColor3f(0.6f, 0.0f, 0.0f);
	glBegin(GL_POLYGON);
	glVertex2f(50,450);
	glVertex2f(75,450);
	glVertex2f(75, 450 - (health * factor));
	glVertex2f(50, 450 - (health * factor));
	glVertex2f(50, 450);
	glEnd();
}

void drawAimingCross() {
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2f(500,240);
	glVertex2f(500,260);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex2f(490,250);
	glVertex2f(510,250);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	glVertex2f(499,239);
	glVertex2f(501,239);
	glVertex2f(501,249);
	glVertex2f(511,249);
	glVertex2f(511,251);
	glVertex2f(501,251);
	glVertex2f(501,261);
	glVertex2f(499,261);
	glVertex2f(499,251);
	glVertex2f(489,251);
	glVertex2f(489,249);
	glVertex2f(499,249);
	glVertex2f(499,239);
	glEnd();
}

void drawBulletWa7sh(){
	GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
		glTranslatef(bulletWa7shX, 0.5, bulletWa7shZ);
		glScalef(0.2, 0.2, 0.2);
		glutSolidSphere(0.1, 100, 100);
	glPopMatrix();
}

void fireWa7sh(){
	if(bulletWa7shFired){
		// check bullet arrived
		if(fabs(bulletWa7shX-px)<0.1 && fabs(bulletWa7shZ-pz)<0.1){
			// hit
			bulletWa7shFired = false;
			health -= 20;
		}else if(fabs(bulletWa7shX-lastpx)<0.1 && fabs(bulletWa7shZ-lastpz)<0.1){
			// reached but no hit
			bulletWa7shFired = false;
		}else{
			float xdir = lastpx-bulletWa7shX,zdir=lastpz-bulletWa7shZ;
			float det = hypot(xdir, zdir);
			xdir/=det;
			zdir/=det;
			bulletWa7shX += 0.01*xdir; // multiply by factor
			bulletWa7shZ += 0.01*zdir;
		}
		drawBulletWa7sh();
	}else{
		// init bullet
		bulletWa7shX=wa7shX;
		bulletWa7shZ=wa7shZ;
		lastpx=px;
		lastpz=pz;
		bulletWa7shFired=true;
	}
}

void drawHealthPack(float tx, float tz) {
	GLfloat mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	glTranslatef(tx, 0, tz);
	glScalef(0.15,0.1,0.15);
	glTranslatef(0.5,0.5,0.5);
	glutSolidCube(1);
	glPopMatrix();
	GLfloat mat_diffuse1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse1);
	glPushMatrix();
	glTranslatef(tx + 0.05, 0, tz +0.025);
	glScalef(0.05, 0.12, 0.1);
	glTranslatef(0.5,0.5,0.5);
	glutSolidCube(1);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(tx + 0.025, 0, tz +0.05);
	glScalef(0.1, 0.12, 0.05);
	glTranslatef(0.5,0.5,0.5);
	glutSolidCube(1);
	glPopMatrix();
}

void drawCastle() {
	for(int i = 0; i <= 16; i++) {
		drawWall(sxs[i], szs[i], txs[i], tzs[i]);
	}
	for(int i = 0; i <= 3; i++) {
		if (hp[i]) {
			drawHealthPack(hpx[i], hpz[i]);
		}
	}
	drawFloor();
}

void drawGun() {
	GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	glTranslatef(px,0.48,pz);
	glRotatef(angle*57.3,0,1,0);
	glTranslatef(-0.05, 0, -0.01);
	glScalef(0.1, 0.01, 0.01);
	glutSolidCube(1);
	glPopMatrix();
}

void drawWa7sh(){
	// big O
	GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
		glTranslatef(wa7shX, 0.3, wa7shZ);
		glScalef(0.3, 0.3, 0.3);
		glutSolidSphere(1, 100, 100);
	glPopMatrix();
	// small O
	glPushMatrix();
		glTranslatef(wa7shX, 0.75, wa7shZ);
		glScalef(0.2, 0.2, 0.2);
		glutSolidSphere(1, 100, 100);
	glPopMatrix();
	// nose
	GLfloat mat_diffuse2[] = { 1.0f, 0.5f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse2);
	glPushMatrix();
		glTranslatef(wa7shX, 0.75, wa7shZ-0.19);
		glScalef(1.5, 1.5, 1.5);
		glRotatef(180, 0,1,0);
		glutSolidCone(0.03, 0.1, 100, 100);
	glPopMatrix();	
	
	// eye
	GLfloat mat_diffuse3[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse3);
	glPushMatrix();
		glTranslatef(wa7shX, 0.84, wa7shZ-0.18);
		glScalef(0.3, 0.3, 0.3);
		glutSolidSphere(0.1, 100, 100);
	glPopMatrix();	
}

void SetupLightsAndMaterial() {
	// material
	GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_diffuse[] = { 1.0f, 0.6f, 0.6f, 1.0f };
	GLfloat mat_specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat mat_shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	// light
	GLfloat light_intensity[] = { 0.7f, 0.7f, 1, 0.5f };
	GLfloat light_position[] = { 5.6f, 5.0f, 5.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_intensity);
}

bool between(float a, float b, float c) {
	return (a <= b && b <= c);
}

bool collision() {
	for (int i = 0; i < 16; i++) {
		if (between(txs[i], px, txs[i] + sxs[i]) && between(tzs[i], pz, tzs[i] + szs[i])) {
			return true;
		}
	}
	return false;
}

bool outOfMaze() {
	return !(between(0, px, 11.2) && between(0, pz, 11.2));
}

void pickUpHealth() {
	for (int i = 0; i <= 3; i++) {
		if (between(hpx[i], px, hpx[i] + 0.15) && between(hpz[i], pz, hpz[i] + 0.15) && hp[i]) {
			hp[i] = false;
			maxHealth += 20;
			health += 20;
		}
	}
}

void display() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	SetupLightsAndMaterial();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1000 / 500, 0.001, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(px, py, pz, lookatX + px, lookatY + py, lookatZ + pz, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawCastle();
	drawGun();
	drawWa7sh();
	if (outOfMaze()) {
		fireWa7sh();
	}

	glDisable(GL_LIGHTING);
	glClear(GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f); // set the drawing color
	glPointSize(4.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,1000,0,500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	glColor3f(1,0,0);
	drawHealthBar();
	drawAimingCross();

	pickUpHealth();
	glFlush();
	glutPostRedisplay();
}

void keyboardPressed(unsigned char thekey, int mouseX, int mouseY) {
	float displacementX = 0.05 * lookatX;
	float displacementZ = 0.05 * lookatZ;
	switch (thekey) {
		case 'w':
			px += displacementX;
			pz += displacementZ;
			if (collision()) {
				px -= displacementX;
				pz -= displacementZ;
			}
			break;
		case 's':
			px -= displacementX;
			pz -= displacementZ;
			if (collision()) {
				px += displacementX;
				pz += displacementZ;
			}
			break;
		case 'd':
			angle -= 0.05f;
			lookatX= -cos(angle);
			lookatZ = sin(angle);
			break;
		case 'a':
			angle += 0.05f;
			lookatX = -cos(angle);
			lookatZ = sin(angle);
			break;
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000,500);
	glutInitWindowPosition(200, 150);
	glutCreateWindow("Maze + Was7sh");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardPressed);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	CreateFromBMP(&textureID, "wall.bmp");
	glEnable(GL_TEXTURE_2D);
	glClearColor(1.0,1.0,1.0,0.0);
	glutMainLoop();
	return 0;
}