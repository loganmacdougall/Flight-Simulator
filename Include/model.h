#pragma once

#include <stdio.h>
#include <string.h>
#include <freeglut.h>
#include "consts.h"
#include "global.h"

typedef struct _Model {
	Vector3* vertices;
	Vector3* normals;
	int** faces;
	int* verticesInFace;
	int* groups;

	ColorA(*groupToColorFunc)(int group);
	
	int numOfVertices;
	int numOfFaces;
	int numOfNormals;
	int numOfGroups;
	int maxNumOfVertices;
	int maxNumOfFaces;
	int maxNumOfGroups;
} Model;

Model* model_init(char* filepath, ColorA(*groupToColorFunc)(int group));
void model_draw(Model* model);
void model_destroy(Model* model);