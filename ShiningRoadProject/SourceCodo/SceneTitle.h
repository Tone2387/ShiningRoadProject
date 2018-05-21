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


	void Create() final;
	void Update( enSCENE &nextScene ) final;
	void Render(
		const D3DXMATRIX &mView, 
		const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight ) const final;



private:


	//テスト用モデル( これは消しても良いです、いらないです ).
	clsCharaStatic* m_pTestChara;


};
