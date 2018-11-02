#ifndef PARTS_BASE_H_
#define PARTS_BASE_H_


#include "Global.h"
#include "SkinMesh.h"

#include "Object.h"





//ステータス列挙体の型( 継承クラスで定義している列挙体の型 ).
//#define PARTS_STATUS_TYPE UCHAR
using PARTS_STATUS_TYPE = UCHAR;

//========== パーツの基底クラス ==========//.
class clsPARTS_BASE : public clsSkinMesh , public clsObject
{
public:
	clsPARTS_BASE();
	virtual ~clsPARTS_BASE();
	
	//毎フレーム.
	void Update();
	//描画.
	void Render(
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR4& vColorBase = { 1.0f, 1.0f, 1.0f, 1.0f },
		const D3DXVECTOR4& vColorArmor = { 1.0f, 1.0f, 1.0f, 1.0f },
		const bool isAlpha = false );


	//Attach直後に使う( Attachしたモデルのボーンの数やパーツ名などを取得 ).
	void Init();


	//直前のフレームのボーンの座標を持ってくる.
	virtual D3DXVECTOR3 GetBonePosPreviosFrame( const int enBoneName, int iVecNum = 0 ) = 0;
	//↑で使うためにボーン座標を記録する( Renderの直後に使う ).
	virtual void UpdateBonePosPreviosFrame() = 0;


	//今のボーン座標を持ってくる.
	D3DXVECTOR3 GetBonePos( const char* sBoneName, const bool isLocalPos = false ) override;


	//アニメーション変更.//変更できるならtrue, 変更できないならfalseが返る.
	bool PartsAnimChange( const int iIndex );

	//パーツの名前を覚える.
	void SetPartsName( const std::string &sPartsName );

	//ボーンが存在するならtrue.
	bool ExistsBone( const char* sBoneName );

	void AddPosition( const D3DXVECTOR3& vPos ){
		SetPosition( GetPosition() + vPos );
	}

	void AddRotation( const D3DXVECTOR3& vRot ){
		SetRotation( GetRotation() + vRot );
	};

protected:
	//----- 各パーツごとの関数 -----//.
	virtual void InitProduct() = 0;//各シーンのInit.
	virtual void UpdateProduct() = 0;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.

	void IntOverGuird( int* const i, const int start, const int end ){
		if( *i >= end ){
			*i = end - 1;
		}
		else if( *i < start ){
			*i = start;
		}
	}

	//パーツの名前.
	std::string m_sPartsName;

	//----- ボーン名の桁数 -----//.
	//ブースター.
	const int m_iDIGIT_BOOSTER_NUM;
	//足の裏.
	const int m_iDIGIT_SOLE_NUM;
	//関節.
	const int m_iDIGIT_JOINT_NUM;

private:

};

#endif//#ifndef PARTS_BASE_H_