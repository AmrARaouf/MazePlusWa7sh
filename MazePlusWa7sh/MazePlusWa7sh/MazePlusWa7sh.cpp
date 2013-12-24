#include "stdafx.h"
#include <cstdlib>
#include "glew.h"
#include "glaux.h"
#include "gl/glut.h"
#include <iostream>

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

void wall(float sx, float sz, float tx, float tz) {
	GLfloat mat_diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	glTranslatef(tx, 0, tz);
	glScalef(sx, 1, sz);
	glTranslatef(0.5,0.5,0.5);
	glutSolidCube(1);
	glPopMatrix();
}

void healthPack(float tx, float tz) {
	GLfloat mat_diffuse[] = { 1.0f, 0.0f, 0.0f, 0.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glPushMatrix();
	glTranslatef(tx, 0, tz);
	glScalef(0.15,0.1,0.15);
	glTranslatef(0.5,0.5,0.5);
	glutSolidCube(1);
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
	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 0.5f };
	GLfloat light_position[] = { 5.6f, 5.0f, 5.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
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

void pickUpHealth() {
	for (int i = 0; i <= 3; i++) {
		if (between(hpx[i], px, hpx[i] + 0.15) && between(hpz[i], pz, hpz[i] + 0.15) && hp[i]) {
			hp[i] = false;
			maxHealth += 20;
			health += 20;
		}
	}
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
	for(int i = 0; i <= 16; i++) {
		wall(sxs[i], szs[i], txs[i], tzs[i]);
	}
	for(int i = 0; i <= 3; i++) {
		if (hp[i]) {
			healthPack(hpx[i], hpz[i]);
		}
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
			angle -= 0.02f;
			lookatX= -cos(angle);
			lookatZ = sin(angle);
			break;
		case 'a':
			angle += 0.02f;
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
	glClearColor(1.0,1.0,1.0,0.0);
	glutMainLoop();
	return 0;
}