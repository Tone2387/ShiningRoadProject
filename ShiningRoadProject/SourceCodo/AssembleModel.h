#ifndef ASSEMBLE_MODEL_H_
#define ASSEMBLE_MODEL_H_

#include "Resource.h"
#include "PartsLeg.h"
#include "PartsCore.h"
#include "PartsHead.h"
#include "PartsArmL.h"
#include "PartsArmR.h"
#include "PartsWeapon.h"

#include "RoboStatusBase.h"



//�A�Z���u���V�[���̃��f������.
class clsASSEMBLE_MODEL
{
public:
	clsASSEMBLE_MODEL();
	virtual ~clsASSEMBLE_MODEL();

	//�I�����̂���p�[�c�̎��( �z��̓Y�����ɂȂ� ).
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

	//m_vpParts�̓Y����.
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


	//�A�Z���u���V�[���̊e�֐����Ŏg���܂�.
	void Create( clsResource* const pResource, clsROBO_STATUS* const pStatus );
	void UpDate();
	virtual void Render(
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const enPARTS_TYPES AlphaParts = enPARTS_TYPES::ENUM_SIZE,
		ID3D11DeviceContext* const pContext = nullptr );

	//���f���̏����Z�b�g.
	void Init( clsROBO_STATUS* const pStatus );

	//���f�����ς�.
	void AttachModel( const enPARTS enParts, const SKIN_ENUM_TYPE PartsNum );


	//���O�̃t���[���ł́A�w��p�[�c�̎w��{�[���̍��W��Ԃ�.
	D3DXVECTOR3 GetBonePosPreviosFrame( 
		const enPARTS_INDEX enParts, 
		const int enBoneName,
		int iVecNum = 0 ) const;



	//�g�����X�t�H�[��.
	void SetPos( const D3DXVECTOR3 &vPos );//�{�[���ʒu�Ɋe�p�[�c�����킹���Ƃ����Ă���.
	void AddPos( const D3DXVECTOR3 &vVec )	{ SetPos( m_Trans.vPos + vVec ); }
	D3DXVECTOR3 GetPos() const				{ return m_Trans.vPos; }

	void SetRot( const D3DXVECTOR3 &vRot );
	void AddRot( const D3DXVECTOR3 &vRot )	{ SetRot( D3DXVECTOR3( m_Trans.fPitch, m_Trans.fYaw, m_Trans.fRoll ) + vRot ); }
	D3DXVECTOR3 GetRot() const				{ return { m_Trans.fPitch, m_Trans.fYaw, m_Trans.fRoll }; }

	void SetScale( const float fScale );

	void SetAnimSpd( const double &dSpd );

	int GetPartsNum( const enPARTS_TYPES enPartsType ){ return m_enPartsNum[ enPartsType ]; }

	//�p�[�c�̃A�j���[�V�����ύX.
	bool PartsAnimChange( const enPARTS enParts, const int iIndex ) const;

	//�p�[�c�̃{�[���̍��W���擾.
	D3DXVECTOR3 GetBonePos( const enPARTS enParts, const char* sBoneName ) const;

	//�{�[�������݂��邩.
	bool ExistsBone( const enPARTS enParts, const char* sBoneName ) const;


	//�p�[�c�̐F�w��.
	void SetPartsColor( const D3DXVECTOR4 &vColor, const unsigned int uiMaskNum );
	D3DXVECTOR4 GetPartsColor( const unsigned int uiMaskNum ) const;

	//�\�Ȃ�( �͈͓��Ȃ� )true��Ԃ�.
	bool IncrementColor( const clsROBO_STATUS::enCOLOR_GAGE enColorGage );
	bool DecrementColor( const clsROBO_STATUS::enCOLOR_GAGE enColorGage );

	//0.0f�`1.0f�ŕԂ�.
	float GetColorGradation( const clsROBO_STATUS::enCOLOR_GAGE enColorGage ) const;
	std::vector< D3DXVECTOR4 > GetColor() const { return m_vecvColor; };

	//0~16�ŕԂ�.
	int GetColorRank( const clsROBO_STATUS::enCOLOR_GAGE enColorGage ) const;


#if _DEBUG
	//�e�p�[�c��pos.
	D3DXVECTOR3 GetPartsPos( const UCHAR ucParts ) const;

#endif//#if _DEBUG

protected:

	//�p����Ŏg���Ă�.
	virtual void CreateProduct( clsROBO_STATUS* const pStatus );
	virtual void UpdateProduct(){};


	//�p�[�c���{�[���̈ʒu( �����ȏꏊ )�ɍ��킹��( +����̊p�x��r�ɍ��킹�� ).
	void SetPartsFormalPos() { SetPos( GetPos() ); }

	//�r�̊p�x�𕐊���͎ʂ���.
	void FitJointModel( 
		clsPARTS_BASE *pMover, clsPARTS_BASE *pBace,
		const char *RootBone, const char *EndBone ) const;

	//�F��f���o��.
	D3DXVECTOR4 CreateColor( 
		const enPARTS_TYPES AlphaParts, 
		const UINT uiIndex,
		const unsigned int uiMaskNum,
		ID3D11DeviceContext* const pContext ) const;

	//���C���[�t���[���ؑ�.
	void ChangeWireFrame(
		const bool isWire,
		ID3D11DeviceContext* const pContext ) const;

	//�A�j���[�V�������Z�b�g.
	void AnimReSet() const;

	//��]�l�}��.
	float GuardDirOver( float &outTheta ) const;

	void ModelUpdate() const;

	//�F.
	void UpdateColor( const clsROBO_STATUS::enCOLOR_GAGE enColorGage );

protected:

	double m_dAnimSpd;

	TRANSFORM m_Trans;

	clsResource* m_wpResource;


	//�p�[�c�̐����̃|�C���^.
	std::vector< clsPARTS_BASE* >	m_vpParts;


	//�F�̔z��.
	std::vector< D3DXVECTOR4 >	m_vecvColor;

private:

	//GetPartsNum�֐��̈�.
	enPARTS_TYPES m_enPartsNum[ enPARTS_TYPES::ENUM_SIZE ];


	//�F�̒i�K.
	int m_iColorRank[ clsROBO_STATUS::enCOLOR_GAGE_size ];

};

#endif//#ifndef ASSEMBLE_MODEL_H_