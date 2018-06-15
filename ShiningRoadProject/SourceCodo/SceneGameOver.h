#pragma once

#include "SceneBase.h"
#include "CameraGameOver.h"


//================================//
//========== ゲームオーバークラス ==========//
//================================//
class clsSCENE_GAME_OVER : public clsSCENE_BASE
{
public:
	clsSCENE_GAME_OVER( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_GAME_OVER();


private:
	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;

#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG

};
