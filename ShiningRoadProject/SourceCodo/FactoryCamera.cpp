#include "FactoryCamera.h"

//#include "Camera.h"
#include "CameraTitle.h"
#include "CameraAssemble.h"
#include "CameraMission.h"
#include "CameraEnding.h"
#include "CameraGameOver.h"

clsCAMERA_BASE* clsFACTORY_CAMERA::Create( const enSCENE enScene ) const
{
	//�w�肳�ꂽ�V�[��������ēf���o��.
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
	case enSCENE::GAMEOVER:
		return new clsCAMERA_GAMEOVER();
		break;

	case enSCENE::EXIT_APP:
		return nullptr;
		break;

	default:
		assert( !"�s���ȃV�[�����w�肳��܂���" );
		break;
	}

	return nullptr;
}
