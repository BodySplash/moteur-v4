#pragma once


#include "ressources.h"
#include "CGo.h"


class CGos : public CGo{
private:
	IDirect3DTexture8* _img;
	D3DCOLOR _selected;
	//ListCGod _toDraw;
public:
	CGos();
	CGos(vertexCoord pos);
	void render(POINT offset);
	vertexCoord getPoint();
	IDirect3DTexture8* getSurf(){return _img;}
	void setSurf(IDirect3DTexture8* surf){_img = surf;}
	void setColor(D3DCOLOR color);
	~CGos();


};
