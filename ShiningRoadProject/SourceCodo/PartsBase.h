#pragma once



#include "Global.h"
#include "SkinMesh.h"

#include "Object.h"



//コンストラクタでの配列初期化の為.
#pragma warning( disable : 4351 )


//ステータス列挙体の型( 継承クラスで定義している列挙体の型 ).
#define PARTS_STATUS_TYPE UCHAR

//========== パーツの基底クラス ==========//.
class clsPARTS_BASE : public clsSkinMesh , public clsObject
{
public:
	clsPARTS_BASE();
	virtual ~clsPARTS_BASE();
	
	//作成時.
	void Create();
	//毎フレーム.
	void Update();
	//描画.
	void Render(
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR4 &vColor,
		const bool isAlpha );

	//アニメーション変更.//変更できるならtrue, 変更できないならfalseが返る.
	bool PartsAnimChange( const int iIndex );

protected:
	//----- 各パーツごとの関数 -----//.
	virtual void CreateProduct() = 0;//各シーンのCreate.
	virtual void UpdateProduct() = 0;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.



private:

};

