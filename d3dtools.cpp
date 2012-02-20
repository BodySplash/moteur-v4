#include "d3dtools.h"


COLORREF convertColorRef(COLORREF crSrc, DWORD rBit, DWORD gBit, DWORD bBit) {
	DWORD red = GetRValue(crSrc);
	DWORD blue = GetBValue(crSrc);
	DWORD green = GetGValue(crSrc);

	red>>=(8-rBit);
	blue>>=(8-bBit);
	green>>=(8-gBit);

	red<<=(gBit+bBit);
	green<<=(bBit);

	DWORD dwColor=(red|blue|green);
	return dwColor;

}


IDirect3DSurface8* loadBitmap(LPCSTR filename, D3DFORMAT format) {
	//Comme d'habitude on va utiliser GDI, donc on commence par créer un DC

	HDC hdc = CreateCompatibleDC ( NULL ) ;

	//ensuite on peut charger l'image 
	HBITMAP hbmNew = ( HBITMAP ) LoadImage ( NULL , filename, IMAGE_BITMAP , 0 , 0 , LR_LOADFROMFILE ) ;

	//on récupère les dimensions de l'image
	BITMAP bmp ;
	GetObject ( hbmNew , sizeof ( BITMAP ) , &bmp );

	//et on sélectionne l'image dans notre dc
	HBITMAP hbmOld = ( HBITMAP ) SelectObject ( hdc , hbmNew ) ;

	//nous pouvons maintenant créer l'image surface pour D3D en spécifiante la taille de l'image
	IDirect3DSurface8* pSurface ;
	if (FAILED(_dd3dInter->CreateImageSurface ( bmp.bmWidth , bmp.bmHeight , format , &pSurface )))
		MessageBox(NULL, "erreur", "erreur", MB_OK);
	
	//on récupère les formats pour chaque couleur
	DWORD dwRShiftL = 8 - GetFormatRBits ( format ) ;
	DWORD dwGShiftL = 8 - GetFormatGBits ( format ) ;
	DWORD dwBShiftL = 8 - GetFormatBBits ( format ) ;
	DWORD dwRShiftR = GetFormatGBits ( format ) + GetFormatBBits ( format ) ;
	DWORD dwGShiftR = GetFormatBBits ( format ) ;

	//nous pouvons maintenant calculer les bits par pixel
	DWORD dwBytesPerPixel = ( GetFormatXBits ( format ) + GetFormatABits ( format ) + GetFormatRBits ( format ) + GetFormatGBits ( format ) + GetFormatBBits ( format ) ) / 8 ;

	//quelques variables...
	COLORREF crColor ;
	DWORD dwColor ;
	DWORD dwRed ;
	DWORD dwGreen ;
	DWORD dwBlue ;
	DWORD dwPosition;

	//nous avons tout ce qu'il faut maintenant pour remplir notre surface
	D3DLOCKED_RECT lockrect ;
	pSurface->LockRect ( &lockrect , NULL , 0 ) ;
	BYTE* pbyBuffer = ( BYTE* ) lockrect.pBits ;

	//on scanne l'image... 
	for ( int y = 0 ; y < bmp.bmHeight ; y ++ )
	{
		for ( int x = 0 ; x < bmp.bmWidth ; x ++ )
		{
			//récupération du pixel
			crColor = GetPixel ( hdc , x , y ) ;

			//calcule de sa position
			dwPosition = y * lockrect.Pitch + x * dwBytesPerPixel ;

			//récupération des couleurs
			
			dwRed = GetRValue ( crColor ) ;
			dwGreen = GetGValue ( crColor ) ;
			dwBlue = GetBValue ( crColor ) ;
			//convertions..
			
			dwRed >>= dwRShiftL ;
			dwGreen >>= dwGShiftL ;
			dwBlue >>= dwBShiftL ;

			
			dwRed <<=dwRShiftR ;
			dwGreen <<=dwGShiftR ;

			dwColor = dwRed | dwGreen | dwBlue ;
			

			//nous pouvons enfin copier le pixel
			memcpy ( &pbyBuffer [ dwPosition ] , &dwColor , dwBytesPerPixel ) ;
		}
	}
	

	//unlock la surface
	pSurface->UnlockRect ( ) ;

	//restoration du DC
	SelectObject ( hdc , hbmOld ) ;

	//destruction de la bitmap
	DeleteObject ( hbmNew ) ;

	//destruction du dc
	DeleteDC ( hdc ) ;

	//retour du pointeur sur la surface créée
	return ( pSurface ) ;
}


//fonctions pour récupérer les formats
UINT GetFormatRBits ( D3DFORMAT Format ) 	//rouge
{
	//tests en fonction du format
	switch ( Format )
	{
	case D3DFMT_R8G8B8 ://24 bpp, 8 pour r,g,b
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_A8R8G8B8 ://32 bpp, 8 pour a,r,g,b
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_X8R8G8B8 ://32 bpp, 8 pour r, g, b
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_R5G6B5 ://16bpp, 5 pour r,b, 6 pour g
		{
			return ( 5 ) ;
		} break ;
	case D3DFMT_X1R5G5B5 ://16 bpp, 5 pour r,g,b
		{
			return ( 5 ) ;
		} break ;
	case D3DFMT_A1R5G5B5 ://16 bpp, 5 pour r,g,b, 1 pour a
		{
			return ( 5 ) ;
		} break ;
	case D3DFMT_A4R4G4B4 ://16 bpp, 4 pour a,r,g,b
		{
			return ( 4 ) ;
		} break ;
	case D3DFMT_R3G3B2 ://8bpp, 3 pour r,g, 2 pour b
		{
			return ( 3 ) ;
		} break ;
	case D3DFMT_A8R3G3B2 ://16bpp, 8 pour a, 3 pour r,g, 2 pour b
		{
			return ( 3 ) ;
		} break ;
	case D3DFMT_X4R4G4B4 ://16bpp, 4 pour r,g,b
		{
			return ( 4 ) ;
		} break ;
	}
	return ( 0 ) ;
}

UINT GetFormatGBits ( D3DFORMAT Format ) 	//vert
{
	
	switch ( Format )
	{
	case D3DFMT_R8G8B8 :
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_A8R8G8B8 :
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_X8R8G8B8 :
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_R5G6B5 :
		{
			return ( 6 ) ;
		} break ;
	case D3DFMT_X1R5G5B5 :
		{
			return ( 5 ) ;
		} break ;
	case D3DFMT_A1R5G5B5 :
		{
			return ( 5 ) ;
		} break ;
	case D3DFMT_A4R4G4B4 :
		{
			return ( 4 ) ;
		} break ;
	case D3DFMT_R3G3B2 :
		{
			return ( 3 ) ;
		} break ;
	case D3DFMT_A8R3G3B2 :
		{
			return ( 3 ) ;
		} break ;
	case D3DFMT_X4R4G4B4 :
		{
			return ( 4 ) ;
		} break ;
	}
	
	return ( 0 ) ;
}

UINT GetFormatBBits ( D3DFORMAT Format ) 	//bleu
{
	
	switch ( Format )
	{
	case D3DFMT_R8G8B8 :
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_A8R8G8B8 :
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_X8R8G8B8 :
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_R5G6B5 :
		{
			return ( 5 ) ;
		} break ;
	case D3DFMT_X1R5G5B5 :
		{
			return ( 5 ) ;
		} break ;
	case D3DFMT_A1R5G5B5 :
		{
			return ( 5 ) ;
		} break ;
	case D3DFMT_A4R4G4B4 :
		{
			return ( 4 ) ;
		} break ;
	case D3DFMT_R3G3B2 :
		{
			return ( 2 ) ;
		} break ;
	case D3DFMT_A8R3G3B2 :
		{
			return ( 2 ) ;
		} break ;
	case D3DFMT_X4R4G4B4 :
		{
			return ( 4 ) ;
		} break ;
	}
	return ( 0 ) ;
}

UINT GetFormatABits ( D3DFORMAT Format ) 	//alpha
{
	
	switch ( Format )
	{
	case D3DFMT_A8R8G8B8 :
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_A1R5G5B5 :
		{
			return ( 1 ) ;
		} break ;
	case D3DFMT_A4R4G4B4 :
		{
			return ( 4 ) ;
		} break ;
	case D3DFMT_A8 :
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_A8R3G3B2 :
		{
			return ( 8 ) ;
		} break ;
	}
	
	return ( 0 ) ;
}

UINT GetFormatDBits ( D3DFORMAT Format ) 	//depth
{
	
	switch ( Format ) 
	{
	case D3DFMT_D16_LOCKABLE :
		{
			return ( 16 ) ;
		} break ;
	case D3DFMT_D32 :
		{
			return ( 32 ) ;
		} break ;
	case D3DFMT_D15S1 :
		{
			return ( 15 ) ;
		} break ;
	case D3DFMT_D24S8 :
		{
			return ( 24 ) ;
		} break ;
	case D3DFMT_D16 :
		{
			return ( 16 ) ;
		} break ;
	case D3DFMT_D24X8 :
		{
			return ( 24 ) ;
		} break ;
	case D3DFMT_D24X4S4 :
		{
			return ( 24 ) ;
		} break ;
	}
	//not found, return 0
	return ( 0 ) ;
}

UINT GetFormatSBits ( D3DFORMAT Format ) 	//stencil
{
	
	switch ( Format ) 
	{
	case D3DFMT_D15S1 :
		{
			return ( 1 ) ;
		} break ;
	case D3DFMT_D24S8 :
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_D24X4S4 :
		{
			return ( 4 ) ;
		} break ;
	}
	
	return ( 0 ) ;
}

UINT GetFormatXBits ( D3DFORMAT Format )  
{
	switch ( Format )
	{
	case D3DFMT_X8R8G8B8 :
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_X1R5G5B5 :
		{
			return ( 1 ) ;
		} break ;
	case D3DFMT_X4R4G4B4 :
		{
			return ( 4 ) ;
		} break ;
	case D3DFMT_D24X8 :
		{
			return ( 8 ) ;
		} break ;
	case D3DFMT_D24X4S4 :
		{
			return ( 4 ) ;
		} break ;
	}

	return ( 0 ) ;
}
