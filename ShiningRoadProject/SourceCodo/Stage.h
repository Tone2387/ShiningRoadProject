#pragma once

#include "Global.h"

class clsPOINTER_GROUP;
class clsBUILDING;
class clsObjStaticMesh;
class clsDX9Mesh;



class clsStage
{
public:
	clsStage( clsPOINTER_GROUP* const pPtrGroup );
	~clsStage();

	void Render(				
		const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye );

	std::vector<clsDX9Mesh*> GetStageMeshArray();
	
	//指定したオブジェク位置回転拡縮を更新.
	void SetStageObjTransform(const int iObjNo);

private:

	std::unique_ptr< clsObjStaticMesh > m_pStageGround;
	std::vector< std::unique_ptr< clsBUILDING > > m_vpBuilding;


};
