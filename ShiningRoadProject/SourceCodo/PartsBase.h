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
		const D3DXVECTOR4& vColorBase = { 1.0f, 1.0f, 1.0f, 1.0f },
		const D3DXVECTOR4& vColorArmor = { 1.0f, 1.0f, 1.0f, 1.0f },
		const bool isAlpha = false );

	D3DXVECTOR3 GetBonePos( const char* sBoneName, const bool isLocalPos = false ) override;

	//アニメーション変更.//変更できるならtrue, 変更できないならfalseが返る.
	bool PartsAnimChange( const int iIndex );

	//パーツの名前を覚える.
	void SetPartsName( const std::string &sPartsName );

	//ボーンが存在するか.
	bool ExistsBone( const char* sBoneName );

	void AddPosition( const D3DXVECTOR3& vPos ){
		SetPosition( GetPosition() + vPos );
	}

	void AddRotation( const D3DXVECTOR3& vRot ){
		SetRotation( GetRotation() + vRot );
	};

protected:
	//----- 各パーツごとの関数 -----//.
	virtual void CreateProduct() = 0;//各シーンのCreate.
	virtual void UpdateProduct() = 0;//各シーンのUpdate.
	//----- 各パーツごとの関数 -----//.

	//パーツの名前.
	std::string m_sPartsName;

private:

};

#endif//#ifndef PARTS_BASE_H_