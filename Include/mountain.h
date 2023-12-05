#pragma once

#include <stdbool.h>
#include <math.h>
#include <freeglut.h>
#include "miscFunctions.h"
#include "global.h"

typedef struct _MountainQuad {
	Vector3 vertices[4];
	ColorA colors[4];
} MountainQuad;

typedef struct _Mountain {
	unsigned int iterations;
	unsigned int totalQuads;
	MountainQuad *quads;
} Mountain;

Mountain* mountain_init(unsigned int size);
void mountain_draw_no_texture(Mountain* mountain, unsigned int texture, bool use_texture, bool show_fog);
void mountain_destroy(Mountain* mountain);