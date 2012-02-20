#include "god.h"


//CGos* CGod::_world = NULL;

CGod::CGod(void)
{
	_state = 0;
	_animeSet = NULL;
	_currentImg = 0;
	_type = 1;
}

CGod::CGod(vertexCoord vect) {
	_state = 0; 
	_animeSet = NULL;
	_currentImg = 0;
	_type = 1;
	setPos(vect);
}

vertexCoord CGod::blockPlotter() {
	vertexCoord plot;
	plot.x = (TILEWIDTH >> 3)*(_pos.x -_pos.y+ (100*4));
	plot.y = (TILEHEIGHT >>3)*(_pos.x+ _pos.y+1);
	return plot;
}


void CGod::render(POINT offset, bool behind) {
	D3DCOLOR diffuse;
	vertexCoord center;
	CustomVertex *poly = new CustomVertex[4];
	center = blockPlotter();
	short length = _animeSet->getSize();
	poly[0].x=center.x-(length>>1)+offset.x;
	poly[0].y=center.y-(length>>1)+offset.y;
	poly[1].x = center.x+(length>>1)+offset.x;
	poly[1].y = center.y-(length>>1)+offset.y;;
	poly[2].x =center.x-(length>>1)+offset.x;
	poly[2].y =center.y+(length>>1)+offset.y;
	poly[3].x = center.x+(length>>1)+offset.x;
	poly[3].y = center.y+(length>>1)+offset.y;
	if (behind) 
		diffuse = D3DCOLOR_XRGB(255, 0, 0);
	else diffuse = D3DCOLOR_XRGB(255, 255, 255);

	for(int i=0; i<4; i++) {
		poly[i].rhw = 1.0f;
		poly[i].z = 0.0f;
		poly[i].diffuse = diffuse;
	}
	poly[0].u = 0.0;
	poly[0].v = 0.0;
	poly[1].u = 1.0;
	poly[1].v = 0.0;
	poly[2].u = 0.0;
	poly[2].v = 1.0;
	poly[3].u = 1.0;
	poly[3].v = 1.0;
	_dd3dInter->SetTexture(0, _animeSet->getTexture(_currentImg));
	_dd3dInter->BeginScene();
	_dd3dInter->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2 ,poly , sizeof ( CustomVertex ) );
	_dd3dInter->EndScene();
	_currentImg++;
	_currentImg%=_animeSet->getCount();
	delete[] poly;

}

void CGod::setAnimeSet(CTileSet* anime){
	_animeSet = anime;
}
	
CGod::~CGod(void)
{

}
