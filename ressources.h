#pragma once

#include "d3d8.h"

#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <math.h>
#include "D3dx8core.h"
#include "Dxerr8.h"
#include <time.h>

#include "listes.h"

#define WIN32_LEAN_AND_MEAN
#define CUSTOM_VERTEX_FVF D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1

// structure de vertex pour D3D
struct CustomVertex
{
	float x , y , z , rhw ;
	D3DCOLOR diffuse ;
	float u, v;
} ;

// structure pour adresser les vertexs
struct vertexCoord {
	int x;
	int y;
};

#define ISO_NORTH  0
#define ISO_NORTHEAST 1
#define ISO_EAST 2
#define ISO_SOUTHEAST 3
#define ISO_SOUTH 4
#define ISO_SOUTHWEST 5
#define ISO_WEST 6
#define ISO_NORTHWEST 7


//ressources nécessaires au moteur d'affichage
__declspec(selectany) HINSTANCE hInstance = NULL;
__declspec(selectany) HWND mainHwnd = NULL;
__declspec(selectany) IDirect3D8 *_dd3d = NULL;
__declspec(selectany) IDirect3DDevice8* _dd3dInter = NULL;
__declspec(selectany) D3DDEVTYPE _devType;
__declspec(selectany) D3DVIEWPORT8 _viewPort;
__declspec(selectany) D3DPRESENT_PARAMETERS parms;
const __declspec(selectany) char* file = "engine.ini";

// globales pour la map

 _declspec(selectany) int TILEWIDTH = 128;
 _declspec(selectany) int TILEHEIGHT = 64;






