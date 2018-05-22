#pragma once

#include "SceneClass.h"
#include "CameraTitle.h"



//================================//
//========== タイトルクラス ==========//
//================================//
class clsTITLE : public clsSCENE_BASE
{
public:
	clsTITLE( clsPOINTER_GROUP* const ptrGroup );
	~clsTITLE();





private:

	void CreateProduct() final;
	void UpdateProduct( enSCENE &nextScene ) final;
	void RenderProduct() final;


	//テスト用モデル( これは消しても良いです、いらないです ).
	clsCharaStatic* m_pTestChara;


};
