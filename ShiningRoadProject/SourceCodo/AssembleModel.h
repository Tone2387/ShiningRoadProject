#pragma once

#include "FactoryParts.h"

#include "Resource.h"

#include "RoboStatus.h"



//アセンブルシーンのモデルさん.
class clsASSEMBLE_MODEL
{
public:
	clsASSEMBLE_MODEL();
	~clsASSEMBLE_MODEL();


	//アセンブルシーンの各関数内で使います.
	void Create( clsResource* const pResource, clsROBO_STATUS* const pStatus );
	void UpDate();
	void Render(
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR4 &vColor = { 1.0f, 1.0f, 1.0f, 1.0f },
		const bool isAlpha = false );

	//モデルの初期セット.
	void Init( clsROBO_STATUS* const pStatus );

	//モデルつけ変え.
	void AttachModel( const enPARTS enParts, const SKIN_ENUM_TYPE PartsNum );

	//トランスフォーム.
	void SetPos( const D3DXVECTOR3 &vPos );
	void AddPos( const D3DXVECTOR3 &vVec );
	D3DXVECTOR3 GetPos() const;

	void SetRot( const D3DXVECTOR3 &vRot );
	void AddRot( const D3DXVECTOR3 &vRot );

	void SetScale( const float fScale );

	void SetAnimSpd( const double &dSpd );

	//パーツのアニメーション変更.
	bool PartsAnimChange( const enPARTS enParts, const int iIndex );

#if _DEBUG
	//各パーツのpos.
	D3DXVECTOR3 GetPartsPos( const UCHAR ucParts ) const;
#endif//#if _DEBUG

private:

	//アニメーションリセット.
	void AnimReSet();

	//回転値抑制.
	float GuardDirOver( float &outTheta ) const;

	double m_dAnimSpd;

	TRANSFORM m_Trans;

	clsResource* m_wpResource;

	clsFACTORY_PARTS*	m_pPartsFactory;
	//パーツの数分のポインタ.
	clsPARTS_BASE**	m_wppParts;
	


	//腕の角度を武器も模写する.
	void FitJointModel( 
		clsPARTS_BASE *pMover, clsPARTS_BASE *pBace,
		char *RootBone, char *EndBone );

};

