#include "tileset.h"


CTileSet::CTileSet(void)
{
	_count = 0;
	_textures = NULL;
}

CTileSet::CTileSet(LPCSTR file, D3DFORMAT format) {
	_count= 0;
	load(file, format);
}

void CTileSet::load(LPCSTR file, D3DFORMAT format) {
	// destruction des éventuels textures courantes
	Destroy();
	// Création d'un DC pour charger une image par win32
	HDC hdc = CreateCompatibleDC(NULL);
	// chargement de l'image dans le dc
	HBITMAP hBmNew = (HBITMAP) LoadImage(NULL, file, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// récupération des paramètres de l'image
	BITMAP bmp;
	GetObject(hBmNew, sizeof(BITMAP), &bmp);
	// sélection de l'image dans ld DC
	HBITMAP hBmOld = (HBITMAP) SelectObject(hdc, hBmNew);
	// récupération des clefs de couleur
	COLORREF transparent = GetPixel(hdc, bmp.bmWidth-1, 0);
	COLORREF border = GetPixel(hdc, bmp.bmWidth-1, 1);
	// récupération des colonnes et des lignes
	int nbCols = 0;
	int nbRows = 0;
	int x, y;
	for (x = 0; x<bmp.bmWidth; x++) 
		if (GetPixel(hdc, x, 0) == transparent)
			nbCols++;
	for (y = 0; y<bmp.bmHeight; y++)
		if (GetPixel(hdc, 0, y) == transparent)
			nbRows++;
	// instanciation du tableau de colonnes et lignes
	int * colDiv = new int[nbCols];
	int * rowDiv = new int[nbRows];

	//rescan de l'image pour enregistrer les positions de lignes et colonnes
	nbCols = 0;
	nbRows = 0;

	for (x = 0; x<bmp.bmWidth; x++)
	{
		if (GetPixel(hdc, x, 0)== transparent){
			colDiv[nbCols]=x;
			nbCols++;
		}
	}

	for (y = 0; y<bmp.bmHeight; y++) {
		if (GetPixel(hdc, 0, y) == transparent) {
			rowDiv[nbRows] = y;
			nbRows++;
		}
	}
	// création des textures et des surfaces temporaires
	IDirect3DSurface8 *pSurface;
	//pSurface = loadBitmap(file, format);
	
	_textures = new PDIRECT3DTEXTURE8 [(nbCols-1)*(nbRows-1)];
	
	// calcul des formats de couleur et des bits par pixel
	
	DWORD dwRShiftL = 8 - GetFormatRBits ( format ) ;
	DWORD dwGShiftL = 8 - GetFormatGBits ( format ) ;
	DWORD dwBShiftL = 8 - GetFormatBBits ( format ) ;
	DWORD dwAShiftL = 8 - GetFormatABits(format);
	DWORD dwRShiftR = GetFormatGBits ( format ) + GetFormatBBits ( format ) ;
	DWORD dwGShiftR = GetFormatBBits ( format ) ;
	DWORD dwAShiftR = GetFormatRBits ( format ) + GetFormatGBits ( format ) + GetFormatBBits ( format ) ;
	

	//nous pouvons maintenant calculer les bits par pixel
	DWORD dwBytesPerPixel = ( GetFormatXBits ( format ) + GetFormatABits ( format ) + GetFormatRBits ( format ) + GetFormatGBits ( format ) + GetFormatBBits ( format ) ) / 8 ;
	//quelques variables...
	COLORREF crColor ;
	DWORD dwColor ;
	DWORD dwRed ;
	DWORD dwGreen ;
	DWORD dwBlue ;
	DWORD dwPosition;
	DWORD dwTransparent;
	D3DLOCKED_RECT lockrect ;
	
	_dd3dInter->CreateImageSurface(bmp.bmWidth, bmp.bmHeight, format, &pSurface);
	//_dd3dInter->CreateTexture(colDiv[x+1]-colDiv[x]-2, rowDiv[y+1]-rowDiv[y]-2, 1, 0, format, D3DPOOL_DEFAULT, &_textures[count]);
	pSurface->LockRect(&lockrect, NULL, 0);
	BYTE* pbyBuffer = ( BYTE* ) lockrect.pBits ;
	//scan complet de l'image pour la copier dans l'ImageSurface
	for (  y = 0 ; y < bmp.bmHeight ; y ++ )
	{
		for ( x = 0 ; x < bmp.bmWidth ; x ++ )
		{
			//récupération du pixel
			crColor = GetPixel ( hdc , x , y ) ;

			//calcule de sa position
			dwPosition = y * lockrect.Pitch + x * dwBytesPerPixel ;

			//récupération des couleurs
			
			dwRed = GetRValue ( crColor ) ;
			dwGreen = GetGValue ( crColor ) ;
			dwBlue = GetBValue ( crColor ) ;
			if (crColor == transparent)
				dwTransparent = 0;
			else dwTransparent = 255;
			//convertions..
			
			dwRed >>= dwRShiftL ;
			dwGreen >>= dwGShiftL ;
			dwBlue >>= dwBShiftL ;
			dwTransparent >>=dwAShiftL;
			
			dwRed <<=dwRShiftR ;
			dwGreen <<=dwGShiftR ;
			dwTransparent <<=dwAShiftR;

			dwColor = dwTransparent | dwRed | dwGreen | dwBlue ;
			

			//nous pouvons enfin copier le pixel
			memcpy ( &pbyBuffer [ dwPosition ] , &dwColor , dwBytesPerPixel ) ;
	

		}
	}
		pSurface->UnlockRect();
		
		RECT rcSrc;
		POINT ptDst ;
		ptDst.x = 0;
		ptDst.y = 0;
		IDirect3DSurface8 *imgTmp;
		short count = 0;
		_size = colDiv[1]-colDiv[0]+1;
		for (x=0; x<nbCols-1; x++){
			for (y=0; y<nbRows-1; y++) {
				
				_dd3dInter->CreateTexture(colDiv[x+1]-(colDiv[x]+1), rowDiv[y+1]-(rowDiv[y]+1),1,0, format, D3DPOOL_DEFAULT, &_textures[count]);
				_textures[count]->GetSurfaceLevel(0, &imgTmp);
				SetRect(&rcSrc, colDiv[x]+1, rowDiv[y]+1, colDiv[x+1], rowDiv[y+1]);
				_dd3dInter->CopyRects(pSurface, &rcSrc, 1, imgTmp, &ptDst);
				imgTmp->Release();
				count++;
			}
		}
		_count = count;
		pSurface->Release();
		SelectObject ( hdc , hBmOld ) ;

		//destruction de la bitmap
		DeleteObject ( hBmNew ) ;

		//destruction du dc
		DeleteDC ( hdc ) ;


	}

	IDirect3DTexture8* CTileSet::getTexture(short nb) {
			return _textures[nb];
	}

	void CTileSet::Destroy()
	{
		if (_textures) {
			
			for (int i =0; i<_count; i++){
				_textures[i]->Release();
				_textures[i] = NULL;
			}
			_textures = NULL;

		}
	}

	CTileSet::~CTileSet(void)
	{
		Destroy();
	}
