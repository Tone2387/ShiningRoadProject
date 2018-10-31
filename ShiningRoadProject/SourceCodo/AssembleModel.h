#ifndef ASSEMBLE_MODEL_H_
#define ASSEMBLE_MODEL_H_


#include "FactoryParts.h"

#include "Resource.h"

#include "RoboStatus.h"



//アセンブルシーンのモデルさん.
class clsASSEMBLE_MODEL
{
public:
	clsASSEMBLE_MODEL();
	virtual ~clsASSEMBLE_MODEL();

	//選択肢のあるパーツの種類( 配列の添え字になる ).
	enum enPARTS_TYPES : UCHAR
	{
		LEG = 0,
		CORE,
		HEAD,
		ARMS,
		WEAPON_L,
		WEAPON_R,

		ENUM_SIZE
	};

	//m_vpPartsの添え字.
	enum enPARTS_INDEX : int
	{
		enPARTS_INDEX_LEG = 0,
		enPARTS_INDEX_CORE,
		enPARTS_INDEX_HEAD,
		enPARTS_INDEX_ARM_L,
		enPARTS_INDEX_ARM_R,
		enPARTS_INDEX_WEAPON_L,
		enPARTS_INDEX_WEAPON_R,

		enPARTS_INDEX_size
	};


	//アセンブルシーンの各関数内で使います.
	void Create( clsResource* const pResource, clsROBO_STATUS* const pStatus, const bool isTitleScene = false );
	void UpDate();
	virtual void Render(
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const enPARTS_TYPES AlphaParts = enPARTS_TYPES::ENUM_SIZE );

	//モデルの初期セット.
	void Init( clsROBO_STATUS* const pStatus );

	//モデルつけ変え.
	void AttachModel( const enPARTS enParts, const SKIN_ENUM_TYPE PartsNum );


	//直前のフレームでの、指定パーツの指定ボーンの座標を返す.
	D3DXVECTOR3 GetBonePosPreviosFrame( 
		const enPARTS_INDEX enParts, 
		const int enBoneName,
		int iVecNum = 0 ) const;



	//トランスフォーム.
	void SetPos( const D3DXVECTOR3 &vPos );
	void AddPos( const D3DXVECTOR3 &vVec );
	D3DXVECTOR3 GetPos() const;

	void SetRot( const D3DXVECTOR3 &vRot );
	void AddRot( const D3DXVECTOR3 &vRot );
	D3DXVECTOR3 GetRot() const;

	void SetScale( const float fScale );

	void SetAnimSpd( const double &dSpd );

	int GetPartsNum( const enPARTS_TYPES enParts );

	//パーツのアニメーション変更.
	bool PartsAnimChange( const enPARTS enParts, const int iIndex );

	//パーツのボーンの座標を取得.
	D3DXVECTOR3 GetBonePos( const enPARTS enParts, const char* sBoneName );

	//ボーンが存在するか.
	bool ExistsBone( const enPARTS enParts, const char* sBoneName );


	//パーツの色指定.
	void SetPartsColor( const D3DXVECTOR4 &vColor, const unsigned int uiMaskNum );
	D3DXVECTOR4 GetPartsColor( const unsigned int uiMaskNum );

	//可能なら( 範囲内なら )trueを返す.
	bool IncrementColor( const clsROBO_STATUS::enCOLOR_GAGE enColorGage );
	bool DecrementColor( const clsROBO_STATUS::enCOLOR_GAGE enColorGage );

	//0.0f～1.0fで返す.
	float GetColorGradation( const clsROBO_STATUS::enCOLOR_GAGE enColorGage );
	std::vector< D3DXVECTOR4 > GetColor();

	//0~16で返す.
	int GetColorRank( const clsROBO_STATUS::enCOLOR_GAGE enColorGage );


#if _DEBUG
	//各パーツのpos.
	D3DXVECTOR3 GetPartsPos( const UCHAR ucParts ) const;

#endif//#if _DEBUG

protected:

	//継承先で使ってね.
	virtual void CreateProduct();
	virtual void UpdateProduct();


	//パーツをボーンの位置( 正式な場所 )に合わせる( +武器の角度を腕に合わせる ).
	void SetPartsFormalPos(){
		SetPos( GetPos() );
	}

	//腕の角度を武器も模写する.
	void FitJointModel( 
		clsPARTS_BASE *pMover, clsPARTS_BASE *pBace,
		const char *RootBone, const char *EndBone );

	//色を吐き出す.
	D3DXVECTOR4 CreateColor( 
		const enPARTS_TYPES AlphaParts, 
		const UINT uiIndex,
		const unsigned int uiMaskNum );

	//アニメーションリセット.
	void AnimReSet();

	//回転値抑制.
	float GuardDirOver( float &outTheta ) const;

	void ModelUpdate();

	//色.
	void UpdateColor( const clsROBO_STATUS::enCOLOR_GAGE enColorGage );

protected:

	double m_dAnimSpd;

	TRANSFORM m_Trans;

	clsResource* m_wpResource;

	std::unique_ptr< clsFACTORY_PARTS >	m_upPartsFactory;

	//パーツの数分のポインタ.
//	clsPARTS_BASE**	m_wppParts;
	std::vector< clsPARTS_BASE* >	m_vecpParts;


	//色の配列.
	std::vector< D3DXVECTOR4 >	m_vecvColor;

private:

	//GetPartsNum関数の為.
	enPARTS_TYPES m_enPartsNum[ enPARTS_TYPES::ENUM_SIZE ];


	//色の段階.
	int m_iColorRank[ clsROBO_STATUS::enCOLOR_GAGE_size ];

};

#endif//#ifndef ASSEMBLE_MODEL_H_