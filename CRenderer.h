/////////////////////////////////////////////////////////////////////////////////////////////////
//				class world qui se charge d'initialiser le monde							   //
//				et de faire les rendus, les calculs...										   //
//					2003	Jibe DUSSEAUT													   //
/////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "ressources.h"
#include "CGos.h"
#include "d3dtools.h"
#include "TileSet.h"
#include "God.h"

#include "CGoList.h"

#define ISO_TURNRIGHT(dir, turn) (ISODIRECTION)(((int)(dir)+turn))&7)
#define ISO_TURNLEFT(dir, turn) (ISODIRECTION)(((int)(dir)+(turn)*7)&7)

const short MAPSIZE = 100;
 

class CRenderer {
private:
	
	CGoList _tile[MAPSIZE][MAPSIZE];
	CTileSet* _texture;
	CTileSet* _units;
	
	//IDirect3DSurface8* _texture[4];
	//IDirect3DSurface8* _ground;
public:
	// bool to test if we must render
	unsigned char _render;
	// the scrolling anchor
	POINT ptScreenAnchor;
	CRenderer();
	//init DIRECT3D
	bool initD3D();
	//clear DIRECT3D
	void clearD3D();
	//render the next frame
	void renderFrame(const RECT *src, const RECT *dest);
	//change the resiolution
	bool changeResolution(UINT mode);
	// the tilwalker to walk around the map
	void tileWalker(POINT *depart, short dir);
	//loading a map
	bool loadMap();
	//mouse mapper: from a given mouse coordinate returns the map coordinate
	POINT mouseMapper(POINT mouse);
	~CRenderer();
	//convertion from map coordinate to world coordinate
	vertexCoord tilePlotter(POINT mouse);
};


