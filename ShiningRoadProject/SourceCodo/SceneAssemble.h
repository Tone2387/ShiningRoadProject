#pragma once

#include "SceneClass.h"
#include "CameraAssemble.h"




//================================//
//========== 組み換えクラス ==========//
//================================//
//ステージ選択も兼ねる?兼ねない?.
class clsASSEMBLE : public clsSCENE_BASE
{
public:
	clsASSEMBLE( clsPOINTER_GROUP* const ptrGroup );
	~clsASSEMBLE();

	void Create() final;
	void Update( enSCENE &nextScene ) final;
	void Render(
		const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight ) const final;

private:

	clsSPRITE2D_CENTER* m_pSprite;

	clsCharaStatic* m_pTestChara;
	clsCharaStatic* m_pParts;


};



