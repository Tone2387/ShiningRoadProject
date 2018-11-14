#ifndef FACTORY_SCENE_H_
#define FACTORY_SCENE_H_

class clsSCENE_BASE;
class clsPOINTER_GROUP;
#include "Global.h"


//シーンのnewで使う.
class clsFACTORY_SCENE
{
public:

	//シーンのポインタに「new」の代わりに使う.
	clsSCENE_BASE* Create( 
		const enSCENE enScene,
		clsPOINTER_GROUP* const ptrGroup );

};


#endif//#ifndef FACTORY_SCENE_H_