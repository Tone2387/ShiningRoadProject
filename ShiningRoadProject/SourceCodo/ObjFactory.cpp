#include"ObjFactory.h"

clsObject* clsObjFactory::Create(clsObjFactory::enObj enObjType)
{
	switch (enObjType)
	{
	case clsObjFactory::enObjObject:
		return new clsObject();

		break;
	case clsObjFactory::enObjCharactor:
		return new clsCharactor();

		break;
	case clsObjFactory::enObjRobo:
		return new clsRobo();

		break;
	default:

		assert(!"�s���ȃV�[�����w�肳��܂���");
		return nullptr;

		break;
	}
}