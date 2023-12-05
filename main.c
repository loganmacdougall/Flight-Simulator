/* include the library header files */
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <freeglut.h>
#include "model.h"
#include "texture.h"
#include "mountain.h"
#include "miscFunctions.h"
#include "consts.h"

Model *cessna;
Model *propeller;

Mountain **mountains;

// current camera position
Vector3 pos = { 0, 1, 1 };
float planeSpeed = 0.1;
float planeDirectionAngle = 0;
float planeTurnAmount = 0;
float propellerRotation = 0;
int choosenSeed = 0;

GLUquadric *skyQuad, *seaQuad;

// window dimensions
int windowWidth = 900;
int windowHeight = 600;

float mouseXPos = 0.5;
float mouseYPos = 0.5;

unsigned int moutainTexture, skyTexture, seaTexture;

bool showMountains = false;
bool showMountainTexture = false;
bool showFog = false;
bool showSeaAndSky = false;
bool showWireframe = true;
bool fullscreen = false;

void myDisplay(void)
{
	Vector3 camOffest = {
		pos.x - (planeOffset.x * cosf(planeDirectionAngle) - planeOffset.z * sinf(planeDirectionAngle)),
		pos.y - planeOffset.y,
		pos.z - (planeOffset.z * cosf(planeDirectionAngle) + planeOffset.x * sinf(planeDirectionAngle))
	};
	Vector3 lookAt = { pos.x, pos.y + 1.3, pos.z };
	
	if (showWireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (showFog) glEnable(GL_FOG);
	else glDisable(GL_FOG);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camOffest.x, camOffest.y, camOffest.z,
		lookAt.x, lookAt.y, lookAt.z,
		0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos.arr);

	printGLError("Error after lights");
	
	DrawPlane(pos, cessna, propeller, planeDirectionAngle, planeTurnAmount, propellerRotation);

	printGLError("Error after plane");

	for (int mi = 0; mi < mountainCount; mi++) {
		glPushMatrix();
		glTranslatef(mountainPos[mi].x, mountainPos[mi].y, mountainPos[mi].z);
		glScalef(16.0f, 5.0f, 16.0f);
		mountain_draw_no_texture(mountains[mi], moutainTexture, showMountainTexture, showFog);
		glPopMatrix();
	}
	
	printGLError("Error after mountain");

	if (showSeaAndSky) {
		DrawSeaAndSky(skyTexture, seaTexture, skyQuad, seaQuad, showFog);
	} else {
		DrawAxis();
		DrawGrid(showWireframe);
	}

	printGLError("Error after grid/sea and sky");


	glutSwapBuffers();
}


void myIdle()
{
	propellerRotation = propellerRotation + 21.5;
	planeDirectionAngle += planeTurnAmount * 0.1;
	pos.z += planeSpeed * sinf(planeDirectionAngle);
	pos.x += planeSpeed * cosf(planeDirectionAngle);
	
	// redraw the new state
	glutPostRedisplay();
}

void myKeyboard(char key)

{
	if (key == 'm' || key == 'M') {
		showMountains = !showMountains;
	}
	else if (key == 't' || key == 'T') {
		showMountainTexture = !showMountainTexture;
	}
	else if (key == 'b' || key == 'B') {
		showFog = !showFog;
	}
	else if (key == 's' || key == 'S') {
		showSeaAndSky = !showSeaAndSky;
	}
	else if (key == 'w' || key == 'W') {
		showWireframe = !showWireframe;
	}
	else if (key == 'f' || key == 'F') {
		if (!fullscreen) {
			glutFullScreen();
		}
		else {
			windowWidth = 900;
			windowHeight = 600;
			glutReshapeWindow(windowWidth, windowHeight);
			glutPositionWindow(50, 50);
		}
		fullscreen = !fullscreen;
	}
	else if (key == 'q' || key == 'Q') {
		exit(0);
	}
}


void mySpecial(unsigned char key) {
	if (key == GLUT_KEY_PAGE_UP) {
		planeSpeed = planeSpeed + 0.01;
	}
	else if (key == GLUT_KEY_PAGE_DOWN) {
		if (0.05 < planeSpeed) {
			planeSpeed = planeSpeed - 0.01;
		}
	}
	else if (key == GLUT_KEY_UP) {
		pos.y = pos.y + 0.1;
	}
	else if (key == GLUT_KEY_DOWN) {
		pos.y = pos.y - 0.1;
	}
}

void myPassiveMotion(int x, int y) {
	mouseXPos = (float)x / windowWidth;
	mouseYPos = (windowHeight - (float)y) / windowHeight;
	planeTurnAmount = mouseXPos - 0.5;
}

void myReshape(int width, int height) {

	windowWidth = width;
	windowHeight = height;
	
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	float aspect;
	if (height < width) {
		aspect = (float)width / (float)height;
	}
	else {
		aspect = (float)height / (float)width;
	}

	gluPerspective(45.0, (double)aspect, 3.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
}

void initializeGL(void)
{
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLightIntensity.arr);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glLightfv(GL_LIGHT0, GL_POSITION, light0Pos.arr);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLightColor.arr );
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLightColor.arr );
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLightColor.arr);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMaterialColor.arr);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseMaterialColor.arr);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specularMaterialColor.arr);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, V0001.arr);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)windowWidth / (double)windowHeight, 3.0, 2000.0);
	glMatrixMode(GL_MODELVIEW);
	
	glEnable(GL_DEPTH_TEST);

	skyQuad = gluNewQuadric();
	gluQuadricTexture(skyQuad, 1);
	seaQuad = gluNewQuadric();
	gluQuadricTexture(seaQuad, 1);

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogfv(GL_FOG_COLOR, fogColor.arr);
	glFogf(GL_FOG_DENSITY, 0.008);
	glHint(GL_FOG_HINT, GL_NICEST);
	glEnable(GL_NORMALIZE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	/* GLUT_DOUBLE | GLUT_DEPTH */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Flight Simulator");
	
	printControls();

	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(mySpecial);
	glutPassiveMotionFunc(myPassiveMotion);
	glutReshapeFunc(myReshape);

	initializeGL();

	cessna = model_init("Resources/cessna.txt", groupToColorCessna);
	propeller = model_init("Resources/propeller.txt", groupToColorPropeller);

	moutainTexture = texture_generate("Resources/mount03.bmp");
	skyTexture = texture_generate("Resources/sky08.bmp");
	seaTexture = texture_generate("Resources/sea02.bmp");

	mountains = malloc(sizeof(Mountain*) * mountainCount);
	for (int mi = 0; mi < mountainCount; mi++) {
		srand(mountainSeeds[mi]);
		mountains[mi] = mountain_init(6);
	}

	glutMainLoop();

	model_destroy(cessna);
	model_destroy(propeller);

	for (int mi = 0; mi < mountainCount; mi++) {
		free(mountains[mi]);
	}
	free(mountains);

	return 0;
}

