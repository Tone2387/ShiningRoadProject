#ifndef FACTORY_PARTS_H_
#define FACTORY_PARTS_H_

class clsPARTS_BASE;

#include "Global.h"

//�t���O�ɉ����ăp�[�c��f���o���N���X.
class clsFACTORY_PARTS
{
public:

	//�p�[�c��f���o��.
	clsPARTS_BASE* Create( const enPARTS enParts ) const;

};

#endif//#ifndef FACTORY_PARTS_H_