#include "CRenderer.h"




using namespace std;


CRenderer::CRenderer() {
	_render = 1;

}


bool CRenderer::initD3D() {
	//Variables

	fstream init;
	D3DDISPLAYMODE disp;
	UINT device;
	string temp;
	UINT mode;

	//Création de l'objet DD8
	_dd3d = Direct3DCreate8(D3D_SDK_VERSION);
	if (!_dd3d) {
		MessageBox(mainHwnd,"Erreur", "Impossible de créer l'objet DD3D", MB_OK);
		return false;
	}

	//Recupère les modes depuis un fichier

	init.open(file,ios::in);
	if (init.fail()) {
		MessageBox(NULL, "Impossible d'ouvrir le fichier", "erreur", MB_OK);
		return false;
	}
	init >> device;
	init >> mode;
	init.close();


	_dd3d->EnumAdapterModes(device, mode, &disp);

	//Definition du backbuffer



	parms.BackBufferCount = 1;
	parms.BackBufferWidth = disp.Width;
	parms.BackBufferHeight= disp.Height;
	parms.BackBufferFormat = D3DFMT_A8R8G8B8;
	//multisampling 

	parms.MultiSampleType = D3DMULTISAMPLE_NONE;

	//Defition de la méthode de swaping

	parms.SwapEffect = D3DSWAPEFFECT_FLIP;

	//Rattachement de la fenêtre

	parms.hDeviceWindow = mainHwnd;

	//Mode fullscreen

	parms.Windowed = false;

	//depth buffer information
	parms.EnableAutoDepthStencil = true ;
	parms.AutoDepthStencilFormat = D3DFMT_D16 ;

	//flags
	parms.Flags = 0 ;

	//rafraichissement et temps de présentation
	parms.FullScreen_RefreshRateInHz = disp.RefreshRate ; //use mode's refresh rate
	parms.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT ;

	HRESULT hr;

	hr = _dd3d->CreateDevice ( device , D3DDEVTYPE_HAL , mainHwnd , D3DCREATE_HARDWARE_VERTEXPROCESSING , &parms , &_dd3dInter ) ;

	if ( FAILED ( hr ) )
	{
		//impossible de créer un device en HALL.. donc on essaie en REF
		hr = _dd3d->CreateDevice ( D3DADAPTER_DEFAULT , D3DDEVTYPE_REF , mainHwnd , D3DCREATE_SOFTWARE_VERTEXPROCESSING , &parms , &_dd3dInter ) ;

		if ( FAILED ( hr ) )
		{
			//Impossible de crééer en REF donc on quitte
			MessageBox(NULL, "erreur", "erreur", MB_OK);
			return false;

		}
		else
		{
			//on a reussi a le creer en REF, on sauvegarde cette information
			_devType = D3DDEVTYPE_REF ;	
		}
	}
	else
	{
		//le device est créé en HAL, on sauvegarde cette information
		_devType = D3DDEVTYPE_HAL ;
	}

	//Definition du viewport

	_viewPort.X = 0;
	_viewPort.Y = 0;
	_viewPort.Height = disp.Height;
	_viewPort.Width = disp.Width;
	_viewPort.MinZ = 0.0f;
	_viewPort.MaxZ = 1.0f;
	if (FAILED(_dd3dInter->SetViewport(&_viewPort))){
		MessageBox(NULL, "Impossible de créer le viewPort", "erreur", MB_OK);
		return false;
	}

	//set vertex shader
	_dd3dInter->SetVertexShader ( CUSTOM_VERTEX_FVF ) ;

	//turn off lighting
	_dd3dInter->SetRenderState ( D3DRS_LIGHTING , FALSE ) ;
	//set render to wireframe
	//_dd3dInter->SetRenderState(D3DRS_FILLMODE,D3DFILL_WIREFRAME);
	_dd3dInter->SetRenderState (D3DRS_CLIPPING, false);
	// active l'alpha testing
	_dd3dInter->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	_dd3dInter->SetRenderState(D3DRS_ALPHAREF, 0x01);
	_dd3dInter->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	// set scrolling to 0
	//CGod::setWorld(*_tile);
	loadMap();

	return true;
}

bool CRenderer::loadMap() {

	// chargement de la texture
	POINT ms;
	vertexCoord vect;
	srand( (unsigned)time( NULL ) );
	short alea;
	//Chargement du texture set

	_texture = new CTileSet("terreset1.bmp", parms.BackBufferFormat);
	_units = new CTileSet("grunt.bmp", parms.BackBufferFormat);
	short count = _texture->getCount();
	// initialisation des vertex
	CGos *temp;

	for (int i = 0; i<MAPSIZE; i++)
		for (int j = 0; j<MAPSIZE ; j++){

			ms.x = i;
			ms.y = j;
			vect = tilePlotter(ms);
			temp = new CGos(vect);
			_tile[i][j]._current = temp;
			alea = (short)((rand()%4)) ;
			temp->setSurf(_texture->getTexture(alea));
			temp = NULL;
		}
		CGod *grunt;
		vertexCoord pos;
		pos.x = 2;
		pos.y = 2;
		grunt = new CGod(pos);
		grunt->setAnimeSet(_units);
		_tile[0][0]._next = new CGoList();
		_tile[0][0]._next->_current = grunt;
		
		
		ptScreenAnchor.x = -TILEWIDTH*(MAPSIZE>>1);
		ptScreenAnchor.y = -TILEHEIGHT*(MAPSIZE>>1);



		return true;
}


void CRenderer::clearD3D() {
	if (_texture) {
		_texture->Destroy();
		_texture = NULL;
	}
	if (_units) {
		_units->Destroy();
		_units = NULL;
	}
	if (_dd3d) {
		_dd3d->Release();
		_dd3d = NULL;
	}
	if (_dd3dInter)
	{
		_dd3dInter->Release();
		_dd3dInter=NULL;
	}



}

bool CRenderer::changeResolution(UINT mode) {
	D3DDISPLAYMODE disp;
	_dd3d->EnumAdapterModes(D3DADAPTER_DEFAULT, mode, &disp);
	parms.BackBufferCount = 1;
	parms.BackBufferWidth = disp.Width;
	parms.BackBufferHeight= disp.Height;
	parms.BackBufferFormat = disp.Format;
	//multisampling 

	parms.MultiSampleType = D3DMULTISAMPLE_NONE;

	//Defition de la méthode de swaping

	parms.SwapEffect = D3DSWAPEFFECT_COPY;

	//Rattachement de la fenêtre

	parms.hDeviceWindow = mainHwnd;

	//Mode fullscreen

	parms.Windowed = false;

	//depth buffer information
	parms.EnableAutoDepthStencil = FALSE ;
	parms.AutoDepthStencilFormat = D3DFMT_UNKNOWN ;

	//flags
	parms.Flags = 0 ;

	//rafraichissement et temps de présentation
	parms.FullScreen_RefreshRateInHz = disp.RefreshRate ; //use mode's refresh rate
	parms.FullScreen_PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	if (FAILED(_dd3dInter->Reset(&parms)))
		return false;


	return true;
}

void CRenderer::renderFrame(const RECT *src, const RECT *dest)
{

	if (_render) {
		// quelques variables nécessaires
		POINT temp;
		POINT topleft;
		POINT bottomleft;
		POINT bottomright;
		POINT topright;
		short alignX, alignY;
		temp.x = 0;
		temp.y = 0;
		// on calcule la map position de chaque coin de l'écran
		topleft = mouseMapper(temp);
		temp.x = parms.BackBufferWidth;
		topright = mouseMapper(temp);
		temp.y = parms.BackBufferHeight;
		bottomright = mouseMapper(temp);
		temp.x = 0;
		bottomleft = mouseMapper(temp);
		// on aligne chaque coin s'ils le sont pas
		alignX = topleft.x-topright.x;
		alignY = topleft.y-topright.y;
		alignX+=alignY;
		topright.y+=alignX;
		alignX = bottomright.x-topright.x;
		alignY = bottomright.y-topright.y;
		alignX-=alignY;
		bottomright.y+=alignX;
		for (int i =0; i<3; i++) {
			tileWalker(&topleft,ISO_NORTHWEST);
			tileWalker(&topright,ISO_NORTHEAST);
			tileWalker(&bottomright, ISO_SOUTHEAST);
			tileWalker(&bottomleft, ISO_SOUTHWEST);
		}
		_dd3dInter->Clear(0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_XRGB ( 0, 0 , 0 ) , 1.0f, 0 );
		temp = topleft;
		short rowCount=0;
		// pointeurs pour parcourir la liste, récupérer l'entrée et la convertir en objet graphique convenable
		CGo* toDraw = NULL;
		CGos* toDrawCGos = NULL;
		CGod* toDrawCGod = NULL;
		CGoList* parse = NULL;
		for (;;) {
			for (;;) {

				if ((temp.x>=0 && temp.y>=0) && (temp.x<MAPSIZE && temp.y<MAPSIZE)) {
					parse  = &_tile[temp.x][temp.y];
					while (parse){
						toDraw = parse->_current;
						switch(toDraw->getType()){
							case 0: {
								toDrawCGos = (CGos*)toDraw;
								toDrawCGos->render(ptScreenAnchor);
								toDrawCGos = NULL;
									}
									break;
							case 1: {
								toDrawCGod = (CGod*)toDraw;
								toDrawCGod->render(ptScreenAnchor, false);
								toDrawCGod = NULL;
									}
									break;
						}
						parse = parse->_next;
					}
				}

				tileWalker(&temp, ISO_EAST);
				if((temp.x == topright.x) && (temp.y == topright.y))
					break;
			}
			if (rowCount%2 == 0) {
				tileWalker(&topleft, ISO_SOUTHEAST);
				tileWalker(&topright, ISO_SOUTHEAST);
			}
			else {
				tileWalker(&topleft, ISO_SOUTHWEST);
				tileWalker(&topright, ISO_SOUTHWEST);
			}
			rowCount++;
			if (((topleft.x == bottomleft.x+1)&&(topleft.y == bottomleft.y+1)) || ((topright.x == bottomright.x+1)&&(topright.y == bottomright.y+1)))
				break;
			temp = topleft;
		}
		toDraw = NULL;

		if (_dd3dInter->Present(src, dest, mainHwnd, NULL)==D3DERR_DEVICELOST)
		{
			while (_dd3dInter->TestCooperativeLevel()==D3DERR_DEVICELOST);
			if (_dd3dInter->TestCooperativeLevel()==D3DERR_DEVICENOTRESET)
				_dd3dInter->Reset(&parms);
			else PostQuitMessage(0);
		}
		_render=0;
	}

}


vertexCoord CRenderer::tilePlotter(POINT mouse){
	vertexCoord retour;
	retour.x = (TILEWIDTH >> 1)*(mouse.x - mouse.y + MAPSIZE);
	retour.y = (TILEHEIGHT >> 1)*(mouse.x + mouse.y +1);
	return retour;
}


void CRenderer::tileWalker(POINT* depart, short dir) {
	switch(dir) {
		case (ISO_NORTH):
			{
				depart->y-=1;
				depart->x-=1;
			}
			break;
		case (ISO_SOUTH):
			{
				depart->y++;
				depart->x++;
			}
			break;
		case (ISO_EAST):
			{
				depart->x++;
				depart->y--;
			}
			break;
		case (ISO_WEST):
			{
				depart->x-=1;
				depart->y++;
			}
			break;
		case (ISO_SOUTHEAST):
			{
				depart->x++;
			}
			break;
		case (ISO_SOUTHWEST) :
			{
				depart->y++;
			}
			break;
		case (ISO_NORTHWEST):
			{
				depart->x--;
			}
			break;
		case (ISO_NORTHEAST):
			{
				depart->y--;
			}
			break;
	}

}






POINT CRenderer::mouseMapper(POINT mouse) {
	mouse.x-= ptScreenAnchor.x;
	mouse.y-= ptScreenAnchor.y;
	POINT retour;
	POINT temp;
	retour.x = 0;
	retour.y = 0;
	temp.x = 0;
	temp.y = 0;
	vertexCoord origine = tilePlotter(retour);
	temp.x = mouse.x - origine.x;
	temp.y = mouse.y - origine.y+(TILEHEIGHT>>1);
	retour.x  = temp.y +(temp.x>>1);
	retour.y =temp.y -(temp.x>>1);	
	retour.x =retour.x/TILEHEIGHT;
	retour.y =retour.y/TILEHEIGHT;
	if (retour.x>=0 && retour.y >=0 && retour.x<MAPSIZE && retour.y<MAPSIZE){
		CGos* temp = (CGos*)_tile[retour.x][retour.y]._current;
		temp->setColor(D3DCOLOR_XRGB(255, 0, 0));
	}
	return retour;
}


CRenderer::~CRenderer() {

	clearD3D();


}
