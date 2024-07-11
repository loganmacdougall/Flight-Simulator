#include "model.h"

static char _MODEL_LINE[200];

void _model_init_vertex(Model* model, FILE* model_file) {
	if (model->numOfVertices == model->maxNumOfVertices) {
		model->maxNumOfVertices *= 1.5;
		model->vertices = realloc(model->vertices, sizeof(Vector4) * model->maxNumOfVertices);
		model->normals = realloc(model->normals, sizeof(Vector4) * model->maxNumOfVertices);
	}

	Vector4* data = &model->vertices[model->numOfVertices];
	fscanf(model_file, "%f %f %f\n", &data->x, &data->y, &data->z);
	data->w = 1;

	model->numOfVertices++;
}

void _model_init_normal(Model* model, FILE* model_file) {
	Vector3* data = &model->normals[model->numOfVertices];
	fscanf(model_file, "%f %f %f\n", &data->x, &data->y, &data->z);

	model->numOfNormals++;
}

void _model_init_face(Model* model, FILE* model_file) {
	if (model->numOfFaces == model->maxNumOfFaces) {
		model->maxNumOfFaces *= 1.5;
		model->faces = realloc(model->faces, sizeof(int*) * model->maxNumOfFaces);
		model->verticesInFace = realloc(model->verticesInFace, sizeof(int) * model->maxNumOfFaces);
	}

	static int fd[20];
	fgets(_MODEL_LINE, 200, model_file);

	int faceCount = sscanf(_MODEL_LINE, "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
		&fd[0], &fd[1], &fd[2], &fd[3], &fd[4], &fd[5], &fd[6], &fd[7], &fd[8], &fd[9],
		&fd[10], &fd[11], &fd[12], &fd[13], &fd[14], &fd[15], &fd[16], &fd[17], &fd[18], &fd[19]);

	model->verticesInFace[model->numOfFaces] = faceCount;
	model->faces[model->numOfFaces] = calloc(faceCount, sizeof(int));
	memcpy(model->faces[model->numOfFaces], fd, sizeof(int) * faceCount);

	model->numOfFaces++;
}

void _model_init_group(Model* model, FILE* model_file) {
	if (model->numOfGroups == model->maxNumOfGroups) {
		model->maxNumOfGroups *= 1.5;
		model->groups = realloc(model->groups, sizeof(int) * model->maxNumOfGroups);
	}
	
	fgets(_MODEL_LINE, 200, model_file);
	model->groups[model->numOfGroups] = model->numOfFaces;

	model->numOfGroups++;
}

Model* model_init(char* filepath, ColorA (*groupToColorFunc)(int group)) {
	FILE* model_file = fopen(filepath, "r");

	Model* model = malloc(sizeof(Model));

	model->groupToColorFunc = groupToColorFunc;

	model->numOfVertices = 0;
	model->numOfNormals = 0;
	model->numOfFaces = 0;
	model->numOfGroups = 0;
	model->maxNumOfVertices = 10;
	model->maxNumOfFaces = 10;
	model->maxNumOfGroups = 10;
	model->vertices = calloc(sizeof(Vector4), model->maxNumOfVertices);
	model->normals = calloc(sizeof(Vector3), model->maxNumOfVertices);
	model->faces = calloc(sizeof(Vector4I), model->maxNumOfFaces);
	model->verticesInFace = calloc(sizeof(int), model->maxNumOfFaces);
	model->groups = calloc(sizeof(int), model->maxNumOfGroups);

	char type;

	while (fscanf(model_file, "%c", &type) != EOF) {
		switch (type)
		{
			case 'v':
				_model_init_vertex(model, model_file);
				break;

			case 'f':
				_model_init_face(model, model_file);
				break;

			case 'n':
				_model_init_normal(model, model_file);
				break;

			case 'g':
				_model_init_group(model, model_file);
				break;
		}
	}

	fclose(model_file);

	return model;
}

void model_draw(Model* model) {
	int groupIndex = -1;
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	for (int fi = 0; fi < model->maxNumOfFaces; fi++) {
		if (groupIndex + 1 < model->numOfGroups && model->groups[groupIndex + 1] == fi) {
			groupIndex++;
			glColor4fv(model->groupToColorFunc(groupIndex).arr);
		}

		glBegin(GL_POLYGON);
		for (int fvi = 0; fvi < model->verticesInFace[fi]; fvi++) {
			int vi = model->faces[fi][fvi] - 1;
			glNormal3fv(model->normals[vi].arr);
			glVertex4fv(model->vertices[vi].arr);
		}
		glEnd();
	}
}

void model_destroy(Model* model) {

	free(model->vertices);
	free(model->normals);
	free(model->groups);
	free(model->verticesInFace);

	for (int i = 0; i < model->numOfFaces; i++)
		free(model->faces[i]);
	
	free(model);
}