#pragma once

#include "Global.h"
#include"SkinMesh.h"

//ロボの構成に必要なパーツ数.
enum class enPARTS : UCHAR
{
	LEG = 0,
	CORE,
	HEAD,
	ARM_L,
	ARM_R,
	WEAPON_L,
	WEAPON_R,

	MAX
};


//========== パーツの基底クラス ==========//.
class clsPARTS_BASE : public clsSkinMesh
{
public:
	clsPARTS_BASE();
	virtual ~clsPARTS_BASE();
	
protected:

private:


};

