#pragma once

#include "Global.h"
#include "SkinMesh.h"

#include "Object.h"

//========== パーツの基底クラス ==========//.
class clsPARTS_BASE : public clsSkinMesh , public clsObject
{
public:
	clsPARTS_BASE();
	virtual ~clsPARTS_BASE();
	
//	void SetPos( const D3DXVECTOR3 &vPos );
//	void AddPos( const D3DXVECTOR3 &vPos );
//	D3DXVECTOR3 GetPos();
//
//	void SetRot( const D3DXVECTOR3 &vRot );
//	void AddRot( const D3DXVECTOR3 &vRot );
//	D3DXVECTOR3 GetRot();
//
//	void SetScale( const float &vRot );
//	void AddScale( const float &vRot );
//	D3DXVECTOR3 GetScale();

protected:

private:


};

