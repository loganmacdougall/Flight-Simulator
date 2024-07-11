#include "consts.h"

float worldRadius = 700.0f;

Vector4 ambientLightIntensity = { 0.3, 0.3, 0.3, 1 };
Vector4 textureEmission = { 0.5, 0.5, 0.5, 1 };

Vector4 ambientLightColor = { 0.3, 0.3, 0.3, 1 };
Vector4 diffuseLightColor = { 0.4, 0.5, 0.2, 1 };
Vector4 specularLightColor = { 0.8, 0.6, 0.8, 1 };

Vector4 ambientMaterialColor = { 1, 1, 1, 1 };
Vector4 diffuseMaterialColor = { 1, 1, 1, 1 };
Vector4 specularMaterialColor = { 1, 1, 1, 1 };
Vector4 V0001 = { 0, 0, 0, 1 };

Vector4 fogColor = { 0.75, 0.5, 0.6, 0.05 };

Vector3 planeOffset = { 3.6, -0.5, 0 };
Vector3 propOffset = { 0, 0, -0.7 };

Vector4 light0Pos = { 1, 1, 1, 0 };

unsigned int mountainCount = 12;
Vector3 mountainPos[] = {
	{300, -0.25, 60},
	{150, -0.25, 40},
	{120, -0.25, -60},
	{-50, -0.25, 40},
	{70, -0.25, -200},
	{80, -0.25, -100},
	{-60, -0.25, 100},
	{140, -0.25, 30},
	{-10, -0.25, 130},
	{270, -0.25, -150},
	{-280, -0.25, 150},
	{-180, -0.25, -100},
};
unsigned int mountainSeeds[] = { 28, 24, 14, 8, 27, 65, 23, 46, 18, 90, 5, 45 };