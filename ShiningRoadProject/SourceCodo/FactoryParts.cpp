#include "FactoryParts.h"


//�p�[�c��f���o��.
clsPARTS_BASE* Create( const enPARTS enParts )
{
	switch(enParts)
	{
	case enPARTS::LEG:
		return new clsPARTS_LEG();
		break;
	case enPARTS::CORE:
		return new clsPARTS_CORE();
		break;
	case enPARTS::HEAD:
		return new clsPARTS_HEAD();
		break;
	case enPARTS::ARM_L:
		return new clsPARTS_ARM_L();
		break;
	case enPARTS::ARM_R:
		return new clsPARTS_ARM_R();
		break;
	case enPARTS::WEAPON_L:
	case enPARTS::WEAPON_R:
		return new clsPARTS_WEAPON();
		break;
	default:
		assert( !"�s���ȃp�[�c���w�肳��܂���" );
		break;
	}

	return nullptr;
}