// _TugasRancang.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "windows.h"
#include "stdlib.h"
#include "glut.h"
#include "Box2D\Box2D.h"

const float WIDTH = 800.0;
const float HEIGHT = 600.0;

const float m2p = 20; //meter to pixel
const float p2m = 1 / m2p; //pixel to meter
const float PI = 3.14;

bool mouseDown;
char keypress = 'a';



//define physics world
b2Vec2 gravity(0.0f, 9.8f);
b2World* world; //pointer, dynamically allocated

b2Body* addRectangle(int x, int y, int w, int h, bool dyn = true){ //add bodydef
	//create dynamic body
	b2BodyDef bodyDef;
	if (dyn == true) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(x*p2m, y*p2m);
	b2Body* body = world->CreateBody(&bodyDef);

	//attach polygon using fixture def
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(w*p2m / 2, h*p2m / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 0.5f;
	fixtureDef.friction = 0.1f;
	fixtureDef.restitution = 0.15f;
	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* addCircle(int x, int y, int r, bool dyn = true){
	//create dynamic body
	b2BodyDef bodyDef;
	if (dyn == true) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(x*p2m, y*p2m);
	b2Body* body = world->CreateBody(&bodyDef);

	//attach circle using fixture def
	b2CircleShape dynCircle;
	dynCircle.m_radius = 10 * p2m;
	dynCircle.m_p.Set(0, 0);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynCircle;
	fixtureDef.density = 0.5f;
	fixtureDef.friction = 0.1f;
	fixtureDef.restitution = 0.15f;
	body->CreateFixture(&fixtureDef);

	return body;
}

b2Body* add3angle(int x, int y, float r, bool dyn = true){ //add bodydef
	//create dynamic body
	b2BodyDef bodyDef;
	if (dyn == true) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(x*p2m, y*p2m);
	b2Body* body = world->CreateBody(&bodyDef);

	//attach polygon using fixture def
	b2PolygonShape dyn3angle;
	b2Vec2 Vertices[3];
	float a = 6.28 / 3;
	for (int i = 0; i < 3; i++)
	{
		Vertices[i].Set(0 + cos(i*a)*r*p2m, 0 + sin(i*a)*r*p2m);
	}
	int32 count = 3;
	dyn3angle.Set(Vertices, count);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dyn3angle;
	fixtureDef.density = 0.5f;
	fixtureDef.friction = 0.1f;
	fixtureDef.restitution = 0.15f;
	body->CreateFixture(&fixtureDef);

	return body;
}

void drawSquare(b2Vec2* points, b2Vec2 center, float angle){
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(center.x* m2p, center.y*m2p, 0);
	glRotatef(angle*180.0 / PI, 0, 0, 1);
	glBegin(GL_QUADS);
	for (int i = 0; i<4; i++){
		glVertex2f(points[i].x*m2p, points[i].y*m2p);
	}
	glEnd();
	glPopMatrix();
	glFlush();
}

void drawCircle(b2Vec2 center, float r, float angle){
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(center.x* m2p, center.y*m2p, 0);
	glRotatef(angle*180.0 / PI, 0, 0, 1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0, 0); //center point
	for (float i = 0; i<360; i++){
		glVertex2f((cos(i)*r)*m2p, (sin(i)*r)*m2p);
	}
	glEnd();
	glPopMatrix();
	glFlush();
}

void draw3angle(b2Vec2* points, b2Vec2 center, float angle){
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(center.x*m2p, center.y*m2p, 0);
	glRotatef(angle*180.0 / PI, 0, 0, 1);
	glBegin(GL_POLYGON);
	for (int i = 0; i<3; i++){
		glVertex2f(points[i].x*m2p, points[i].y*m2p);
	}
	glEnd();
	glPopMatrix();
	glFlush();
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
	case 'A':
		keypress = 'A';
		break;
	case's':
	case'S':
		keypress = 'S';
		break;
	case'd':
	case 'D':
		keypress = 'D';
		break;

	}
	printf("%c", keypress);
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;
		glColor3f(1, 1, 1);
		//addRectangle(x, y, 20, 20, true);
		add3angle(x, y, 20, true);

		float32 timeStep = 1 / 20.0;
		int32 velocityIteration = 8.0;
		int32 positionIteration = 3.0;
		world->Step(timeStep, velocityIteration, positionIteration);

		glutSwapBuffers();
		glutPostRedisplay();

	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		mouseDown = true;
		glColor3f(1, 1, 1);
		addCircle(x, y, 2000, true);

		float32 timeStep = 1 / 20.0;
		int32 velocityIteration = 8.0;
		int32 positionIteration = 3.0;
		world->Step(timeStep, velocityIteration, positionIteration);

		glutSwapBuffers();
		glutPostRedisplay();
	}
	mouseDown = false;

	printf("%d %d %d \n", state, x, y);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	b2Body* tmp = world->GetBodyList();
	b2Vec2 points[4];
	while (tmp){
		if (tmp->GetFixtureList()->GetShape()->GetType() == 0){
			b2CircleShape* c = ((b2CircleShape*)tmp->GetFixtureList()->GetShape());
			drawCircle(tmp->GetWorldCenter(), c->m_radius, tmp->GetAngle());
		}
		else {
			for (int i = 0; i < 3; i++)
			{
				points[i] = ((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);
			}
			//drawSquare(points, tmp->GetWorldCenter(), tmp->GetAngle());
			draw3angle(points, tmp->GetWorldCenter(), tmp->GetAngle());
		}
		tmp = tmp->GetNext();
	}

	float32 timeStep = 1 / 20.0;
	int32 velocityIteration = 8.0;
	int32 positionIteration = 3.0;
	world->Step(timeStep, velocityIteration, positionIteration);

	glutSwapBuffers();
	glutPostRedisplay();

}

void init()
{
	glMatrixMode(GL_PROJECTION);
	//menyesuaikan sistem koordinat opengl
	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
	//gluOrtho2D(0, WIDTH, HEIGHT, 0);
	glViewport(0, WIDTH, 0, HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	world = new b2World(gravity);

	addRectangle(0, 600, 1600, 20, false);


	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Tugas Rancang");
	init();
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(handleKeypress);

	float32 timeStep = 1 / 20.0;
	int32 velocityIteration = 8.0;
	int32 positionIteration = 3.0;
	world->Step(timeStep, velocityIteration, positionIteration); //update frame
	glutSwapBuffers();


	/*
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(handleResize);
	glutTimerFunc(25, update, 0);*/

	glutMainLoop();
	return 0;
}

