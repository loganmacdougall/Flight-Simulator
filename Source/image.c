#include "image.h"

void _image_init_ppm(char* filepath, Image* image) {
	FILE* image_file = fopen(filepath, "r");

	char headerLine[100];
	char tempChar;
	int maxValue;

	fscanf(image_file, "%[^\n] ", headerLine);

	// make sure that the image begins with 'P3', which signifies a PPM file
	if ((headerLine[0] != 'P') || (headerLine[1] != '3'))
	{
		printf("This is not a PPM file!\n");
		exit(0);
	}

	fscanf(image_file, "%c", &tempChar);

	// while we still have comment lines (which begin with #)
	while (tempChar == '#')
	{
		// read in the comment
		fscanf(image_file, "%[^\n] ", headerLine);
		printf("%s\n", headerLine);
		fscanf(image_file, "%c", &tempChar);
	}

	// the last one was not a comment character '#', so we need to put it back into the file stream (undo)
	ungetc(tempChar, image_file);
	fscanf(image_file, "%d %d %d", &image->width, &image->height, &maxValue);

	image->totalPixels = image->width * image->height;
	image->data = malloc(3 * sizeof(unsigned char) * image->totalPixels);

	// determine the scaling for RGB values
	float RGBScaling = 255.0 / (float)maxValue;

	for (int i = 0; i < image->totalPixels; i++)
	{
		int r, g, b;
		fscanf(image_file, "%d %d %d", &r, &g, &b);

		// store the red, green and blue data of the current pixel in the data array
		image->data[3 * i] = (unsigned char)(r * RGBScaling);
		image->data[3 * i + 1] = (unsigned char)(g * RGBScaling);
		image->data[3 * i + 2] = (unsigned char)(b * RGBScaling);
	}

	// close the image file
	fclose(image_file);
}

void _image_init_bmp(char* filepath, Image* image) {
	FILE * image_file = fopen(filepath, "rb");

	unsigned char header[54];
	fread(header, sizeof(unsigned char), 54, image_file);

	image->width = *(int*)&header[18];
	image->height = *(int*)&header[22];

	image->totalPixels = image->width * image->height;
	image->data = malloc(3 * sizeof(unsigned int) * image->totalPixels);

	fread(image->data, sizeof(unsigned char), image->totalPixels * 3, image_file);

	fclose(image_file);
	
	// Change BGR to RGB
	for (int i = 0; i < image->totalPixels; i++) {
		int i1 = i * 3;
		int i2 = i * 3 + 2;
		unsigned char t = image->data[i1];
		image->data[i1] = image->data[i2];
		image->data[i2] = t;
	}

}

// Based on lecture21 'displayPPM.c'
Image* image_init(char* filepath)
{
	Image* image = malloc(sizeof(Image));
	FILE * image_file = fopen(filepath, "rb");
	unsigned char magic_bytes[8];

	fgets(magic_bytes, 8, image_file);

	fclose(image_file);

	if (magic_bytes[0] == 'P' && magic_bytes[1] == '3')
	{
		_image_init_ppm(filepath, image);
	}
	else if (magic_bytes[0] == 'B' && magic_bytes[1] == 'M') {
		_image_init_bmp(filepath, image);
	}
	else {
		printf("Unreconized file type \"%s\", can't read as image", filepath);
		exit(0);
	}

	return image;
}

void image_destroy(Image* image) {
	free(image->data);
	free(image);
}