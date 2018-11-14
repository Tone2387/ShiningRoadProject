#ifndef FACTORY_SOUND_MANAGER_H_
#define FACTORY_SOUND_MANAGER_H_


class clsSOUND_MANAGER_BASE;
#include "Global.h"


class clsFACTORY_SOUND_MANAGER
{
public:

	clsSOUND_MANAGER_BASE* Create( 
		const enSCENE enNextScene,
		HWND const hWnd );

};

#endif//#ifndef FACTORY_SOUND_MANAGER_H_