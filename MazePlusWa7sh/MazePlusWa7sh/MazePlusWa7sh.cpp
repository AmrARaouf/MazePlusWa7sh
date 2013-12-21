#include "stdafx.h"
#include "gl/glut.h"

float sxs[] = {0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 10.8f, 1.8f, 1.8f, 0.6f, 4.0f, 3.4f, 3.0f, 4.2f, 10.2f};
float szs[] = {11.2f, 2.6f, 6.0f, 6.0f, 1.8f, 1.8f, 9.8f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f};
float txs[] = {0.0f, 1.8f, 3.8f, 4.8f, 7.8f, 8.8f, 10.8f, 0.2f, 0.2f, 0.2f, 4.2f, 6.8f, 0.2f, 7.8f, 1.8f, 1.0f};
float tzs[] = {0.0f, 8.2f, 1.0f, 1.0f, 6.0f, 2.0f, 1.0f, 0.0f, 1.8f, 3.8f, 3.8f, 3.8f, 5.8f, 7.8f, 7.8f, 10.8f};

void wall(float sx, float sz, float tx, float tz) {
	glPushMatrix();
	glTranslatef(tx, 0, tz);
	glScalef(sx, 1, sz);
	glTranslatef(0.5,0.5,0.5);
	glutSolidCube(1);
	glPopMatrix();
}

void SetupLights() {
	GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat mat_diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0, 1.0f };
	GLfloat mat_shininess[] = { 50 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	GLfloat lightIntensity[] = { 0.7f, 0.7f, 1, 1.0f };
	GLfloat light_position[] = { 24.0f, 0.5f, 1.7f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);
}

void display() {
	SetupLights();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1000 / 500, 0.001, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(15, 0.5, 0.7, 10, 0.5, 0.7, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(int i = 0; i <= 16; i++) {
		wall(sxs[i], szs[i], txs[i], tzs[i]);
	}
	glFlush();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1000,500);
	glutInitWindowPosition(200, 150);
	glutCreateWindow("Maze + Was7sh");
	glutDisplayFunc(display);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glClearColor(1.0,1.0,1.0,0.0);
	glutMainLoop();
}