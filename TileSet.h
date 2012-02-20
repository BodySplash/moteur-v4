#pragma once

#include "ressources.h"
#include "d3dtools.h"

class CTileSet
{
private:
	IDirect3DTexture8** _textures;
	short _count;
	short _size;
public:
	CTileSet(void);
	CTileSet(LPCSTR file, D3DFORMAT format);
	void load(LPCSTR file, D3DFORMAT format);
	short getCount() { return _count;}
	IDirect3DTexture8* getTexture(short nb);
	void Destroy();
	~CTileSet(void);
	short getSize(){return _size;}
};
