#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <freeglut.h>

typedef struct _Image {
	unsigned char *data;
	unsigned int width;
	unsigned int height;
	unsigned int totalPixels;
} Image;

Image* image_init(char* filepath);
void image_draw(Image* image, unsigned int bottomX, unsigned int bottomY);
void image_destroy(Image* image);