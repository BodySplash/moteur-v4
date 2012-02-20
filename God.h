#pragma once



/////////////////////////////////////////////////////////////////
// DEFINE
/////////////////////////////////////////////////////////////////

#define SHOOTING 0x01
#define MOOVING 0x02
#define DYING 0x03
#define RAS 0x00


/////////////////////////////////////////////////////////////////
// INCLUDE
/////////////////////////////////////////////////////////////////

#include "ressources.h"
#include "TileSet.h"
#include "CGo.h"
/////////////////////////////////////////////////////////////////
// DEFINITION
/////////////////////////////////////////////////////////////////


class CGod : public CGo
{
private:
	
	short _currentImg;
	CTileSet *_animeSet;
	BYTE _state;
	vertexCoord blockPlotter();
	//static CGos* _world;

public:
	
	//static void setWorld(CGos* world){CGod::_world = world;}
	CGod(void);	// constructeur
	CGod(vertexCoord vect); // constructeur par valeur
	void setDir(BYTE dir); // set unit's direction
	void setMooving(bool moove); // make the unit moove
	void setShooting(bool shoot); // make the unit shoot
	void setDying(bool dying); // make the unit die
	void setRAS(); // make the unit wait
	bool isMooving(); // mooving or not
	bool isShooting(); // shooting or not
	bool isDying(); // dying or not
	bool isWaiting(); // waiting or not
	void render(POINT offset, bool behind); // draw the unit
	void setAnimeSet(CTileSet *anime); // set the anime set
	~CGod(void); // destructor
};
