#include "texture.h"

unsigned int texture_generate(char* filepath)
{
	Image* image = image_init(filepath);
    unsigned int texture_pointer;

    glGenTextures(1, &texture_pointer);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, texture_pointer);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, image->width, image->height, GL_RGB, GL_UNSIGNED_BYTE, image->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    image_destroy(image);

    return texture_pointer;
}
