#ifndef FACTORY_SCENE_H_
#define FACTORY_SCENE_H_

class clsSCENE_BASE;
class clsPOINTER_GROUP;
#include "Global.h"


//�V�[����new�Ŏg��.
class clsFACTORY_SCENE
{
public:

	//�V�[���̃|�C���^�Ɂunew�v�̑���Ɏg��.
	clsSCENE_BASE* Create( 
		const enSCENE enScene,
		clsPOINTER_GROUP* const ptrGroup ) const;

};


#endif//#ifndef FACTORY_SCENE_H_