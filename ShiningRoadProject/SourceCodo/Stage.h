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
	void SetColorTarget( const D3DXVECTOR4& vColor ){ 
		m_vLightColorTarget = m_vLightColor = vColor;
		m_LightLightingFlg.Init();
	}

	void SetScale( const float fScale );

private:

	//光の明滅.
	void UpdateLight();

	//光の描く数値.
	void Lighting( float* const fpColor, const float fTarget, bool* const ispFlg );

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
	D3DXVECTOR4 m_vLightColorTarget;

	//光のフラグ.
	struct LIGHT_LIGHTING_FLG
	{
		//true:明るくなる.
		bool isLightingR;
		bool isLightingG;
		bool isLightingB;

		LIGHT_LIGHTING_FLG()
		{
			Init();
		}
		void LIGHT_LIGHTING_FLG::Init()
		{
			isLightingR = false;
			isLightingG = false;
			isLightingB = false;
		}
	}	m_LightLightingFlg;

};
