#ifndef FACTORY_CAMERA_H_
#define FACTORY_CAMERA_H_

class clsCAMERA_BASE;

#include "Global.h"

//フラグに応じてシーンごとにカメラを吐き出すクラス.
class clsFACTORY_CAMERA
{
public:
//	clsFACTORY_CAMERA();
//	~clsFACTORY_CAMERA();

	//カメラを吐き出す.
	clsCAMERA_BASE* Create( const enSCENE enScene ) const;


};

#endif//#ifndef FACTORY_CAMERA_H_