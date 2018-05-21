#pragma once


#include "Camera.h"
#include "CameraTitle.h"
#include "CameraAssemble.h"
#include "CameraMission.h"
#include "CameraEnding.h"

//フラグに応じてシーンごとにカメラを吐き出すクラス.
class clsFACTORY_CAMERA
{
public:
//	clsFACTORY_CAMERA();
//	~clsFACTORY_CAMERA();

	//カメラを吐き出す.
	clsCAMERA_BASE* Create( const enSCENE enScene );


};

