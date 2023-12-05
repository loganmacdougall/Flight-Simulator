#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PI 3.141592654

#define randrange(s,e) ((float)rand() / (float)(RAND_MAX / (((float)e)-((float)s))) + ((float)s))
#define interpolate(x,s,e) (((float)s) + ((float)x)*(((float)e)-((float)s)))

typedef union _Vector3 {
    struct {
        float x, y, z;
    };
    struct {
        float r, g, b;
    };
    float arr[3];
} Vector3;

typedef union _Vector3I {
    struct {
        int x, y, z;
    };
    struct {
        int r, g, b;
    };
    int arr[3];
} Vector3I;

typedef union _Vector4 {
    struct {
        float x, y, z, w;
    };
    struct {
        float r, g, b, a;
    };
    float arr[4];
} Vector4;

typedef union _Vector4I {
    struct {
        int x, y, z, w;
    };
    struct {
        int r, g, b, a;
    };
    int arr[4];
} Vector4I;

typedef Vector3 Color;
typedef Vector4 ColorA;