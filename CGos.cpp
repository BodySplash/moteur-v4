#include "CGos.h"


CGos::CGos(){
	_pos.x = 0;
	_pos.y = 0;
	_selected = D3DCOLOR_XRGB(255, 255, 255);
	_img = NULL;
	_type = 0x00;
}

CGos::CGos(vertexCoord pos) {
	setPos(pos);
	_type = 0x00;
}


void CGos::render(POINT offset){
	CustomVertex* retour = new CustomVertex[4];
	//défition de la position
	retour[0].x=_pos.x+offset.x;
	retour[0].y=_pos.y+TILEHEIGHT+offset.y;;
	retour[1].x = _pos.x+offset.x;
	retour[1].y = _pos.y+offset.y;;
	retour[2].x =_pos.x+TILEWIDTH+offset.x;
	retour[2].y =_pos.y+TILEHEIGHT+offset.y;
	retour[3].x = _pos.x+TILEWIDTH+offset.x;
	retour[3].y = _pos.y+offset.y;
	//défition du RHW et du Z et du diffuse color
	for(int i=0; i<4; i++) {
		retour[i].rhw = 1.0f;
		retour[i].z = 1.0f;
		retour[i].diffuse = _selected;
	}
	// remplissage des coordonnées de textures
	retour[0].u = 0.0;
	retour[0].v = 0.0;
	retour[1].u = 0.0;
	retour[1].v = 1.0;
	retour[2].u = 1.0;
	retour[2].v = 0.0;
	retour[3].u = 1.0;
	retour[3].v = 1.0;
	// début du rendu
	_dd3dInter->SetTexture(0, _img);
	_dd3dInter->BeginScene();
	_dd3dInter->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP , 2 ,retour , sizeof ( CustomVertex ) );
	_dd3dInter->EndScene();
	_selected= D3DCOLOR_XRGB(255, 255, 255);
	delete[] retour;
}



void CGos::setColor(D3DCOLOR color) {
	_selected = color;
}
vertexCoord CGos::getPoint() {
	
	return _pos;
}

CGos::~CGos(){
	
}