#include "stdafx.h"
#include "gl/glut.h"

float sxs[] = {0.0f};
float sys[] = {0.0f};

float txs[] = {0.0f};
float tys[] = {0.0f};
float tzs[] = {0.0f};

void wall(float sx, float sy, float tx, float ty, float tz) {
	glPushMatrix();
	glTranslatef(tx, ty, tz);
	glScalef(sx, sy, 1);
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
	GLfloat light_position[] = { 5.0f, 5.0f, 5.0f, 1.0f };
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
	gluLookAt(5, 5, 20, 5, 5, 0, 0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	wall(0.2, 11.2, 0, 0, 0);
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