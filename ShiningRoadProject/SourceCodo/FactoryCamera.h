#ifndef FACTORY_CAMERA_H_
#define FACTORY_CAMERA_H_

class clsCAMERA_BASE;

#include "Global.h"

//�t���O�ɉ����ăV�[�����ƂɃJ������f���o���N���X.
class clsFACTORY_CAMERA
{
public:
//	clsFACTORY_CAMERA();
//	~clsFACTORY_CAMERA();

	//�J������f���o��.
	clsCAMERA_BASE* Create( const enSCENE enScene ) const;


};

#endif//#ifndef FACTORY_CAMERA_H_