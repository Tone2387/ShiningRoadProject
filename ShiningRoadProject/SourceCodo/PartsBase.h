#pragma once


//
//	ステータスがこいつに必要なのかが非常に怪しい。.
//
//	アセンブルシーンに各パーツのファイルクラスを持たせよう.
//
//
//
//
//
//

#include "Global.h"
#include "SkinMesh.h"

#include "Object.h"

#include <vector>
#include <string>

//ステータス列挙体の型( 継承クラスで定義している ).
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


	//パーツのステータスを吐き出す( 添え字は各クラスの列挙体を参照されたし ).
	int GetStatus( PARTS_STATUS_TYPE Num );
	//ステータスセット( 第一引数:ステータスに入れる数  第二引数:for文のカウント ).
	template<class T>
	void SetStatus( const int iStatus, const T tNum );

protected:
	//----- 各パーツごとの関数 -----//.
	virtual void CreateProduct() = 0;//各シーンのCreate.
	virtual void UpdateProduct() = 0;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.


	//ステータス.
	void SetStatusMax( PARTS_STATUS_TYPE Num );
	PARTS_STATUS_TYPE GetStatusMax();


	//ステータス.
	//わざわざパーツがステータスを持つのは、.
	//アセンブル画面の都合である。.
	std::vector<int> m_viStatus;
	//パーツ名.
	std::string		m_sName;

private:

	//ステータスが範囲オーバーしていないかのチェック.
	bool isWithInRange( PARTS_STATUS_TYPE Num );

	//各継承クラスのステータスの最大数.
	PARTS_STATUS_TYPE m_StatusMax;

};

