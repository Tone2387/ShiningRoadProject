#pragma once

#include "SceneClass.h"
#include "CameraMission.h"


#include"Robo.h"


//================================//
//========== ミッション中クラス ==========//
//================================//
class clsMISSION : public clsSCENE_BASE
{
public:
	//引数はリソースなどのポインタのまとめ( 引数は基底クラスで取り扱っているので特に意識しなくて大丈夫です ).
	clsMISSION( clsPOINTER_GROUP* const ptrGroup );
	~clsMISSION();

	//「SceneManager.cpp」の「SwitchScene」関数内でシーン作成直後に使用される.
	void Create() final;

	//ループ内の処理( 引数のフラグをを関数内で変更すると今のシーンが破棄され、.
	//				  指定したシーンが生成される ).
	//このフラグの詳細は「SceneClass.h」の「enSCENE」を見てください.
	void Update( enSCENE &nextScene ) final;

	//シーン内のオブジェクトの描画関数のまとめ.
	void Render(
		const D3DXMATRIX &mView, 
		const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight ) const final;

private:


	//テスト用モデル( これは消しても良いです、いらないです ).
	clsRobo* m_pTestRobo;
	clsCharaStatic* m_pTestChara;

	//基底クラスに以下があります.
	//カメラ.
	//リソース.
	//エフェクト.
	//サウンド.

};

