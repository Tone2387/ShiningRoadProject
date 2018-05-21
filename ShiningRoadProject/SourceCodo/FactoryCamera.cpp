#include "FactoryCamera.h"


clsCAMERA_BASE* clsFACTORY_CAMERA::Create( const enSCENE enScene )
{
	//指定されたシーンを作って吐き出す.
	switch( enScene )
	{
	case enSCENE::TITLE:
		return new clsCAMERA_TITLE();
		break;
	case enSCENE::ASSEMBLE:
		return new clsCAMERA_ASSEMBLE();
		break;
	case enSCENE::MISSION:
		return new clsCAMERA_MISSION();
		break;
	case enSCENE::ENDING:
		return new clsCAMERA_ENDING();
		break;
	default:
		assert( !"不正なシーンが指定されました" );
		break;
	}

	return nullptr;
}
