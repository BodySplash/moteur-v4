/////////////////////////////////////////////////////////////////////////////////////////////////
//				Tools pour le moteur d'affichage											   //
//					2003	Jibe DUSSEAUT													   //
/////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef D3DTOOLS_H
#define D3DTOOLS_H

#include "ressources.h"

//fonction pour charger une image au format bitmap retournant la surface créée
IDirect3DSurface8* loadBitmap(LPCSTR filename, D3DFORMAT format);

//fonction qui convertit le format COLORREF en n'importe quel format D3D
COLORREF convertColorRef(COLORREF crSrc, DWORD rBit, DWORD gBit, DWORD bBit);


//get bits for components

UINT GetFormatRBits ( D3DFORMAT Format ) ;	//red

UINT GetFormatGBits ( D3DFORMAT Format ) ;	//green

UINT GetFormatBBits ( D3DFORMAT Format ) ;	//blue

UINT GetFormatABits ( D3DFORMAT Format ) ;	//alpha

UINT GetFormatDBits ( D3DFORMAT Format ) ;	//depth

UINT GetFormatSBits ( D3DFORMAT Format ) ;	//stencil

UINT GetFormatXBits ( D3DFORMAT Format ) ;  //unused bits

#endif