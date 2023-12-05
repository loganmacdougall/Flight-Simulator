#include "consts.h"

float worldRadius = 700.0f;

Vector4 V0001 = { 0, 0, 0, 1 };

Vector4 ambientLightIntensity = { 0, 0, 0, 1 };
Vector4 textureEmission = { 2, 2, 2, 1 };

Vector4 ambientLightColor = { 0.3, 0.3, 0.3, 1 };
Vector4 diffuseLightColor = { 1, 1, 1, 1 };
Vector4 specularLightColor = { 1, 1, 1, 1 };

Vector4 ambientMaterialColor = { 1, 1, 1, 1 };
Vector4 diffuseMaterialColor = { 1, 1, 1, 1 };
Vector4 specularMaterialColor = { 1, 1, 1, 1 };

Vector4 fogColor = { 0.75, 0.5, 0.6, 0.05 };

Vector3 planeOffset = { 4, -0.5, 0 };
Vector3 propOffset = { 0, 0, -0.7 };

Vector4 light0Pos = { 0, -1, 0, 0 };

unsigned int mountainCount = 10;
Vector3 mountainPos[] = {
	{10, -0.25, 60},
	{15, -0.25, 40},
	{120, -0.25, -6},
	{-50, -0.25, 40},
	{70, -0.25, -200},
	{80, -0.25, -100},
	{-60, -0.25, 100},
	{140, -0.25, 30},
	{10, -0.25, 130},
	{70, -0.25, -50},
};
unsigned int mountainSeeds[] = { 3, 6, 14, 8, 27, 65, 23, 46, 18, 90 };