
#include "stdafx.h"
#include "windows.h"
#include "stdlib.h"
#include "glut.h"
#include "Box2D\Box2D.h"
#include "imageloader.cpp"
#include "vector"




const float WIDTH = 800.0;
const float HEIGHT = 600.0;


const float m2p = 20; //meter to pixel
const float p2m = 1 / m2p; //pixel to meter
const float PI = 3.14;


bool mouseDown;
char keypress = 'a';


class MyQueryCallback : public b2QueryCallback
{
public:
	std::vector<b2Body*> foundBodies;
	bool ReportFixture(b2Fixture* fixture)
	{
		foundBodies.push_back(fixture->GetBody());
		return true;//keep going to find all fixtures in the query area
	}
};



GLuint _textureId;


float32 timeStep = 1 / 60.0;
int32 velocityIteration = 8.0;
int32 positionIteration = 3.0;


//define physics world
b2Vec2 gravity(0.0f, 9.8f);
b2World* world; //pointer, dynamically allocated



GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	return textureId;
}


b2Body* addRectangle(int x, int y, int w, int h, bool dyn = true){ //add bodydef
	//create dynamic body (tipe body dinamis yg dapat terpengaruh Force/gravity)
	b2BodyDef bodyDef;
	if (dyn == true) {
		bodyDef.type = b2_dynamicBody;
	}
	bodyDef.position.Set(x*p2m, y*p2m);                                //menetapkan posisi body dibuat -> convert ke meter ( *20)
	b2Body* body = world->CreateBody(&bodyDef);


	//attach polygon using fixture def
	b2PolygonShape dynamicBox;                                                //deklarasi shape dengan nama dynamicBox
	dynamicBox.SetAsBox(w*p2m / 2, h*p2m / 2);                //deklarasi ukuran box


	b2FixtureDef fixtureDef;                                                //deklarasi fixture
	fixtureDef.shape = &dynamicBox;                                        //dengan shape yg dideklarasikan dynamicBox
	fixtureDef.density = 0.5f;                                                //fixture punya kerapatan 0.5
	fixtureDef.friction = 0.1f;                                                //gaya gesek permukaan 0.1
	fixtureDef.restitution = 0.15f;                                        //kelembaman 0.15
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
	b2PolygonShape dyn3angle;                                                                        //polygon dengan bentuk sesuai vertices/titik2 yng ditentukan
	b2Vec2 Vertices[4];
	float a = 4 * PI / 3;
	for (int i = 0; i < 4; i++)
	{
		Vertices[i].Set(0 + cos(i*a)*r * 8, 0 + sin(i*a)*r * 8);        //mengisi titik2 sesuai rumus ke array b2vec2 Vertices
	}
	int32 count = 4;
	dyn3angle.Set(Vertices, count);                                                                //membentuk shape polygon dengan titik2 Vertices dan jumlah titik count.


	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dyn3angle;
	fixtureDef.density = 0.2f;
	fixtureDef.friction = 0.1f;
	fixtureDef.restitution = 0.5f;
	body->CreateFixture(&fixtureDef);


	return body;
}


void drawSquare(b2Vec2* points, b2Vec2 center, float angle){
	//menerima parameter pointer "points", "center", dan "angle"
	glPushMatrix();
	glTranslatef(center.x* m2p, center.y*m2p, 0);                                                        //geser bentuk ke center.x, dan center.y yg dikonvert ke pixel
	glRotatef(angle*180.0 / PI, 0, 0, 1);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	//glNormal3f(0.0, 0.0, 1.0);
	for (int i = 0; i<4; i++){
		if (i == 0){
			glTexCoord2f(0.0f, 0.0f);
		}
		else if (i == 1){
			glTexCoord2f(1.0f, 0.0f);
		}
		else if (i == 2){
			glTexCoord2f(1.0f, 1.0f);
		}
		else if (i == 3){
			glTexCoord2f(0.0f, 1.0f);
		}
		glVertex2f(points[i].x*m2p, points[i].y*m2p);
	}
	glEnd();


	glDisable(GL_TEXTURE_2D);
	glutSwapBuffers();
	glutPostRedisplay();
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
	for (int i = 0; i<4; i++){
		glVertex2d(points[i].x*m2p, points[i].y*m2p);
	}
	glEnd();
	glPopMatrix();
	glFlush();
}


void render(){
	b2Body* tmp = world->GetBodyList(); //get all body in the world and store it at tmp pointer
	b2Vec2 points[4];
	while (tmp){
		if (tmp->GetFixtureList()->GetShape()->GetType() == 0){                                                //if body at tmp shape type == 0 (circle)
			b2CircleShape* c = ((b2CircleShape*)tmp->GetFixtureList()->GetShape()); //store shape of the body at pointer circle c
			drawCircle(tmp->GetWorldCenter(), c->m_radius, tmp->GetAngle());                //call drawcircle, params: center point of the body, radius, angle
		}
		else {
			for (int i = 0; i < 4; i++)
			{
				points[i] = ((b2PolygonShape*)tmp->GetFixtureList()->GetShape())->GetVertex(i);        //store all vertex of the body shape on tmp to the array b2vec2 points[]
			}
			drawSquare(points, tmp->GetWorldCenter(), tmp->GetAngle());                //passing all vertex (on the array points) to drawsquare and angle.
			draw3angle(points, tmp->GetWorldCenter(), tmp->GetAngle());
		}
		tmp = tmp->GetNext();                                                                                                //go to next node of pointer tmp
	}




	world->Step(timeStep, velocityIteration, positionIteration);                        //update frame sesuai gravitasi dan waktu
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
	if ((keypress == 'a' || keypress == 'A') && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mouseDown = true;
		glColor3f(1, 1, 1);
		addRectangle(x, y, 50, 50, true);                                                                //menambahkan objek persegi tiap ditekan




		world->Step(timeStep, velocityIteration, positionIteration);        //update frame


		glutSwapBuffers();
		glutPostRedisplay();


	}
	else if ((keypress == 's' || keypress == 'S') && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		mouseDown = true;
		glColor3f(1, 1, 1);
		addCircle(x, y, 5000, true);


		world->Step(timeStep, velocityIteration, positionIteration);


		glutSwapBuffers();
		glutPostRedisplay();
	}
	else if (((keypress == 'd' || keypress == 'D') && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN))
	{
		mouseDown = true;
		glColor3f(1, 1, 1);
		add3angle(x, y, 2, true);




		world->Step(timeStep, velocityIteration, positionIteration);


		glutSwapBuffers();
		glutPostRedisplay();
	}
	else
	{
		mouseDown = false;
	}



	MyQueryCallback callback;
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;


	b2MouseJointDef mJointDef;
	mJointDef.bodyA = world->CreateBody(&bodyDef);
	mJointDef.collideConnected = true;
	mJointDef.maxForce = 500;


	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		b2AABB aabb;
		aabb.lowerBound.Set(x - 0.001*p2m, y - 0.001*p2m);
		aabb.upperBound.Set(x + 0.001*p2m, y + 0.001*p2m);
		world->QueryAABB(&callback, aabb);
		for (int i = 0; i < callback.foundBodies.size(); i++)
		{
			mJointDef.bodyB = callback.foundBodies[i];
		}
		mJointDef.target.Set(x, y);
		b2MouseJoint* joint = (b2MouseJoint*)world->CreateJoint(&mJointDef);
	}


	printf("%d %d %d \n", state, x, y);
}


void display(){
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//renderer
	render();



	glutSwapBuffers();
	glutPostRedisplay();


}


void init()
{
	glMatrixMode(GL_PROJECTION);
	//menyesuaikan sistem koordinat opengl
	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
	glViewport(0, WIDTH, 0, HEIGHT);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 1.0);


	//deklarasi (membuat) World dengan parameter gravitasi
	world = new b2World(gravity);


	Image* image = loadBMP("wall.bmp");
	_textureId = loadTexture(image);
	delete image;


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


	world->Step(timeStep, velocityIteration, positionIteration); //update frame
	glutSwapBuffers();




	glutMainLoop();
	return 0;
}