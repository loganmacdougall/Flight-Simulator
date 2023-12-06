#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <freeglut.h>
#include "model.h"
#include "global.h"
#include "consts.h"

void printControls();
ColorA groupToColorCessna(int group);
ColorA groupToColorPropeller(int group);
ColorA groupToColorText(int group);
void DrawAxis();
void DrawGrid(bool showWireFrame);
void DrawSeaAndSky(unsigned int skyTexture, unsigned int seaTexture, GLUquadric* skyQuad, GLUquadric* seaQuad, bool showFog);
void DrawPlane(Vector3 pos, Model* cessna, Model* propeller, float planeDirectionAngle, float planeTurnAmount, float propellerRotation);
void printGLError(char* message);