#include "FactoryCamera.h"

//#include "Camera.h"
#include "CameraTitle.h"
#include "CameraAssemble.h"
#include "CameraTakeoff.h"
#include "CameraMission.h"
#include "CameraClear.h"
#include "CameraEnding.h"
#include "CameraGameOver.h"

clsCAMERA_BASE* clsFACTORY_CAMERA::Create( const enSCENE enScene ) const
{
	//�w�肳�ꂽ�V�[��������ēf���o��.
	switch( enScene )
	{
	case enSCENE::TITLE:
		return new clsCAMERA_TITLE();

	case enSCENE::ASSEMBLE:
		return new clsCAMERA_ASSEMBLE();

	case enSCENE::TAKEOFF:
		return new clsCAMERA_TAKEOFF();

	case enSCENE::MISSION:
		return new clsCAMERA_MISSION();

	case enSCENE::CLEAR:
		return new clsCAMERA_CLEAR();

	case enSCENE::ENDING:
		return new clsCAMERA_ENDING();

	case enSCENE::GAMEOVER:
		return new clsCAMERA_GAMEOVER();


	case enSCENE::EXIT_APP:
		return nullptr;

	default:
		assert( !"�s���ȃV�[�����w�肳��܂���" );
		break;
	}

	return nullptr;
}
