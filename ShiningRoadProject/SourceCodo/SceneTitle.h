#pragma once

#include "SceneClass.h"
#include "CameraTitle.h"



//================================//
//========== タイトルクラス ==========//
//================================//
class clsSCENE_TITLE : public clsSCENE_BASE
{
public:
	clsSCENE_TITLE( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_TITLE();





private:

	void CreateProduct() final;
	void UpdateProduct( enSCENE &nextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) const final;

#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG

	//テスト用モデル( これは消しても良いです、いらないです ).
	clsCharaStatic* m_pTestChara;


};
