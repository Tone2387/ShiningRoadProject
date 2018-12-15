#pragma once

#include "Global.h"

class clsPOINTER_GROUP;
class clsBUILDING;
class clsObjStaticMesh;
class clsObjSkinMesh;
class clsDX9Mesh;



class clsStage
{
public:
	clsStage( clsPOINTER_GROUP* const pPtrGroup );
	~clsStage();

	void Render(				
		const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye );

	//ビルの内側描画.
	void RenderInside(				
		const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye );

	std::vector<clsDX9Mesh*> GetStageMeshArray();
	
	//指定したオブジェク位置回転拡縮を更新.
	void SetStageObjTransform(const unsigned int uiObjNo);

	//光るところの色.
	void SetColor( const D3DXVECTOR4& vColor ){ m_vLightColor = vColor; }

	void SetScale( const float fScale );

private:

	//ステージ土台の当たり判定.
	std::unique_ptr< clsObjStaticMesh > m_pStageCollision;
	
	//ステージ土台の見た目.
	std::unique_ptr< clsObjStaticMesh > m_pStageFloor;	//床と壁.
	std::unique_ptr< clsObjStaticMesh > m_pStageCelling;//天井.

	//スキンメッシュモデル.
	enum enDOOR_NUM : int
	{
		enDOOR_NUM_0 = 0,
		enDOOR_NUM_1,

		enDOOR_NUM_size
	};
	//壁のドア.
	std::unique_ptr< clsObjSkinMesh > m_pDoorArray[ enDOOR_NUM_size ];
	//天井のドア.
	std::unique_ptr< clsObjSkinMesh > m_pLia;						

	//ビル.
	std::vector< std::unique_ptr< clsBUILDING > > m_vpBuilding;

	D3DXVECTOR4 m_vLightColor;

};
