#include "mountain.h"

ColorA _mountain_y_color(float y) {
	float r, g, b;
	if (y > 0.8) {
		float x = (1.0 - y) / 0.2;
		r = interpolate(x, 0.8, 0.95) + randrange(0, 0.03);
		b = r;
		g = 1;
	} else {
		float x = y / 0.7;
		r = interpolate(x, 0.1, 0.3) + randrange(0, 0.03);
		b = r;
		g = interpolate(x, 0.5, 0.9);
	}

	return (ColorA){ r,g,b,1 };
}

MountainQuad* _mountain_split_quad(MountainQuad quad, unsigned int iteration) {
	MountainQuad* splitQuads = calloc(sizeof(MountainQuad), 4);
	float randomStep = 1.5 * powf(0.5f, iteration);

	Vector3 BL = quad.vertices[0], BR = quad.vertices[1], TR = quad.vertices[2], TL = quad.vertices[3];
	
	float midY = (BL.y + BR.y + TL.y + TR.y) / 4.0f + randrange(-randomStep, randomStep);
	float midX = (BL.x + BR.x) / 2.0f;
	float midZ = (BL.z + TL.z) / 2.0f;
	
	Vector3 BM = { midX, (BL.y + BR.y) / 2.0f, BL.z };
	Vector3 TM = { midX, (TL.y + TR.y) / 2.0f, TL.z };
	Vector3 MR = { BR.x, (TR.y + BR.y) / 2.0f, midZ };
	Vector3 ML = { BL.x, (TL.y + BL.y) / 2.0f, midZ };
	
	Vector3 MM = { midX, midY, midZ };

	splitQuads[0] = (MountainQuad){ BL, BM, MM, ML };
	splitQuads[1] = (MountainQuad){ BM, BR, MR, MM };
	splitQuads[2] = (MountainQuad){ MM, MR, TR, TM };
	splitQuads[3] = (MountainQuad){ ML, MM, TM, TL };

	return splitQuads;
}

Mountain* mountain_init(unsigned int iterations)
{
	Mountain* mountain = malloc(sizeof(Mountain));
	mountain->iterations = iterations;
	
	unsigned int currentIteration = 1;
	unsigned int currentTotalQuads = 4;

	MountainQuad* currentQuads = calloc(sizeof(MountainQuad), 4);
	
	currentQuads[0] = (MountainQuad){ (Vector3) {0,0,0},(Vector3) {0.5,0,0},(Vector3) {0.5,1,0.5},(Vector3) {0,0,0.5} };
	currentQuads[1] = (MountainQuad){ (Vector3) { 0,0,0.5 },(Vector3) { 0.5,1,0.5 },(Vector3) { 0.5,0,1 },(Vector3) { 0,0,1 } };
	currentQuads[2] = (MountainQuad){ (Vector3) { 0.5,0,0 },(Vector3) { 1,0,0 },(Vector3) { 1,0,0.5 },(Vector3) { 0.5,1,0.5 } };
	currentQuads[3] = (MountainQuad){ (Vector3) { 0.5,1,0.5 },(Vector3) { 1,0,0.5 },(Vector3) { 1,0,1 },(Vector3) { 0.5,0,1 } };

	while (currentIteration < iterations) {
		MountainQuad* tempQuads = currentQuads;
		currentQuads = calloc(sizeof(MountainQuad), currentTotalQuads * 4);
		
		for (int qi = 0; qi < currentTotalQuads; qi++) {
			MountainQuad quad = tempQuads[qi];
			MountainQuad* subquads = _mountain_split_quad(quad, currentIteration);
			for (int sqi = 0; sqi < 4; sqi++) {
				currentQuads[qi * 4 + sqi] = subquads[sqi];
			}
			free(subquads);
		}
		free(tempQuads);
		
		currentTotalQuads *= 4;
		currentIteration++;
	}
	
	mountain->quads = currentQuads;
	mountain->totalQuads = currentTotalQuads;

	for (int i = 0; i < mountain->totalQuads; i++) {
		MountainQuad* quad = &mountain->quads[i];
		for (int vi = 0; vi < 4; vi++) {
			quad->colors[vi] = _mountain_y_color(quad->vertices[vi].y);
		}
	}

	return mountain;
}

void mountain_draw_no_texture(Mountain* mountain, unsigned int texture, bool use_texture, bool show_fog) {
	if (use_texture) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glEnable(GL_TEXTURE_2D);
		glColor4f(1.0, 1.0, 1.0, 1.0);
	}
	glDisable(GL_FOG);
	
	glBegin(GL_QUADS);
	for (int i = 0; i < mountain->totalQuads; i++) {
		MountainQuad* quad = &mountain->quads[i];
		for (int vi = 0; vi < 4; vi++) {
			if (use_texture) glTexCoord2d(quad->vertices[vi].x, quad->vertices[vi].z);
			else glColor4fv(quad->colors[vi].arr);
			glVertex3fv(quad->vertices[vi].arr);
		}
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	if (show_fog) glEnable(GL_FOG);
}

void mountain_destroy(Mountain* mountain)
{
	free(mountain->quads);
	free(mountain);
}
