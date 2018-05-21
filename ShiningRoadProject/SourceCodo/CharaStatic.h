#ifndef CHARA_STATIC_H_
#define CHARA_STATIC_H_


#include "GameObject.h"
#include "DX9Mesh.h"


class clsCharaStatic : public clsGameObject
{
public:
	clsCharaStatic();
	virtual ~clsCharaStatic();

	virtual void Init(){};



	//モデルデータ関連付け関数clsDX9Mesh
	virtual void AttachModel( clsDX9Mesh* const pModel );

	//モデルデータ関連付け解除関数.
	virtual void DetatchModel();

	//レンダリング関数.
	virtual void Render( 
				const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
				const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye,
				const D3DXVECTOR4 vColor = D3DXVECTOR4( 1.0f,1.0f,1.0f,1.0f ),
				const bool bAlpha = false );

	//位置更新関数.
	void UpdatePos();
	enDIRECTION		m_enDir;	//方向.
	float			m_fSpd;


	clsDX9Mesh*		m_pModel;

//	LPD3DXMESH	m_pMeshForRay;	//レイとメッシュ用.






protected:




	//座標や回転値を更新する.
	virtual void UpDateModel();

};


#endif//#ifndef CHARA_STATIC_H_