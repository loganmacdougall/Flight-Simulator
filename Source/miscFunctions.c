#include "miscFunctions.h"

void printControls() {
	char* controls = "\n"
		"\n"
		"\n"
		"Scene Controls\n"
		"-----------------\n"
		"f: toggle fullscreen\n"
		"b : toggle fog\n"
		"m : toggle mountains\n"
		"t : toggle mountain texture\n"
		"s : toggle sea & sky\n"
		"w : toggle wire frame\n"
		"q : quit\n"
		"\n"
		"Camera Controls\n"
		"-----------------\n"
		"Page Up : faster\n"
		"Page Down : slower\n"
		"Up    Arrow : move up\n"
		"Down  Arrow : move down\n"
		"Mouse Right : move right\n"
		"Mouse Left : move left\n";

	puts(controls);
}

ColorA groupToColorCessna(int group)
{
	switch (group) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 31:
		case 32:
			// Yellow
			return (ColorA) { 1.0f, 1.0f, 0.0f, 1.0f };
		case 4:
		case 5:
			// Black
			return (ColorA) { 0.0f, 0.0f, 0.0f, 1.0f };
		case 6:
			// Light Purple
			return (ColorA) { 0.4f, 0.1f, 0.8f, 1.0f };
		case 7:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
			// Blue
			return (ColorA) { 0.0f, 0.0f, 0.8f, 1.0f };
		default:
			return (ColorA) { 1.0f, 0.0f, 1.0f, 1.0f };

	}
}

ColorA groupToColorPropeller(int group)
{
	return (ColorA) {1.0f, 1.0f, 0.0f, 1.0f};
}

void DrawAxis() {
	glPushMatrix();
	glScalef(5.0, 5.0, 5.0);
	glLineWidth(5.0f);
	glBegin(GL_LINES);
	glColor4f(1, 0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glColor4f(0, 1, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glColor4f(0, 0, 1, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();
	glLineWidth(1.0f);
	glPopMatrix();
}

void DrawGrid(bool showWireFrame)
{
	glColor4f(0.5, 0.5, 1.0, 1.0);
	if (showWireFrame) {
		glBegin(GL_LINES);
		for (int i = -50; i <= 50; i++) {
			glVertex3f(-50, 0, i);
			glVertex3f(50, 0, i);
			glVertex3f(i, 0, -50);
			glVertex3f(i, 0, 50);
		}
		glEnd();
	} else {
		glBegin(GL_QUADS);
		glVertex3f(-50, 0, -50);
		glVertex3f(50, 0, -50);
		glVertex3f(50, 0, 50);
		glVertex3f(-50, 0, 50);
		glEnd();
	}
}

void DrawSeaAndSky(unsigned int skyTexture, unsigned int seaTexture, GLUquadric* skyQuad, GLUquadric* seaQuad, bool showFog)
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();

	glDisable(GL_FOG);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, textureEmission.arr);
	glRotatef(-90, 1, 0, 0);

	glBindTexture(GL_TEXTURE_2D, skyTexture);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	gluCylinder(skyQuad, worldRadius, worldRadius, worldRadius, 50, 50);

	if (showFog) glEnable(GL_FOG);

	glBindTexture(GL_TEXTURE_2D, seaTexture);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	gluDisk(seaQuad, 0.0, (double)worldRadius + 1, 50, 50);

	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, V0001.arr);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DrawPlane(Vector3 pos, Model* cessna, Model* propeller, float planeDirectionAngle, float planeTurnAmount, float propellerRotation) {
	glPushMatrix();
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 40);
	glTranslatef(pos.x, pos.y, pos.z);
	glRotatef(180 - planeDirectionAngle / PI * 180.0, 0, 1, 0);
	glRotatef(-planeTurnAmount * 90, 1, 0, 0);
	model_draw(cessna);
	glPushMatrix();
	glTranslatef(-0.55, -0.145, 0.35);
	glRotatef(propellerRotation, 1, 0, 0);
	glTranslatef(0.55, 0.145, -0.35);
	model_draw(propeller);
	glPopMatrix();
	glTranslatef(-0.55, -0.145, -0.35);
	glRotatef(propellerRotation, 1, 0, 0);
	glTranslatef(0.55, 0.145, -0.35);
	model_draw(propeller);
	glPopMatrix();
}

void printGLError(char* message)
{
	unsigned int errorEnum = glGetError();

	if (errorEnum == GL_NO_ERROR) return;

	printf("GL_ERROR - ");
	switch (errorEnum)
	{
	case GL_INVALID_ENUM:
		printf("GL_INVALID_ENUM");
		break;
	case GL_INVALID_VALUE:
		printf("GL_INVALID_VALUE");
		break;
	case GL_INVALID_OPERATION:
		printf("GL_INVALID_OPERATION");
		break;
	case GL_STACK_OVERFLOW:
		printf("GL_STACK_OVERFLOW");
		break;
	case GL_STACK_UNDERFLOW:
		printf("GL_STACK_UNDERFLOW");
		break;
	case GL_OUT_OF_MEMORY:
		printf("GL_OUT_OF_MEMORY");
		break;
	default:
		break;
	}
	if (message) {
		printf(" - %s\n", message);
	}
	else {
		puts("");
	}
}