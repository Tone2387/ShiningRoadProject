#ifndef PARTS_BASE_H_
#define PARTS_BASE_H_


#include "Global.h"
#include "SkinMesh.h"

#include "Object.h"





//�X�e�[�^�X�񋓑̂̌^( �p���N���X�Œ�`���Ă���񋓑̂̌^ ).
//#define PARTS_STATUS_TYPE UCHAR
using PARTS_STATUS_TYPE = UCHAR;

//========== �p�[�c�̊��N���X ==========//.
class clsPARTS_BASE : public clsSkinMesh , public clsObject
{
public:
	clsPARTS_BASE();
	virtual ~clsPARTS_BASE();
	
	//���t���[��.
	void Update();
	//�`��.
	void Render(
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR4& vColorBase = { 1.0f, 1.0f, 1.0f, 1.0f },
		const D3DXVECTOR4& vColorArmor = { 1.0f, 1.0f, 1.0f, 1.0f },
		const bool isAlpha = false )
	{
		ModelRender( mView, mProj, vLight, vEye, vColorBase, vColorArmor, isAlpha );
	};

	//�{�[�����W�����݂̃L�����N�^�[�ʒu�ōX�V.
	void UpdateBonePos() const {
		if( m_pMesh ){
			m_pMesh->UpdateBonePos();
		}
	};

	//Attach����Ɏg��( Attach�������f���̃{�[���̐���p�[�c���Ȃǂ��擾 ).
	void Init(){ InitProduct(); };


	//���O�̃t���[���̃{�[���̍��W�������Ă���.
	virtual D3DXVECTOR3 GetBonePosPreviosFrame( const int enBoneName, int iVecNum = 0 ) const = 0;
	//���Ŏg�����߂Ƀ{�[�����W���L�^����( Render�̒���Ɏg�� ).
	virtual void UpdateBonePosPreviosFrame() = 0;


	//���̃{�[�����W�������Ă���.
	D3DXVECTOR3 GetBonePos( const char* sBoneName, const bool isLocalPos = false ) override;


	//�A�j���[�V�����ύX.//�ύX�ł���Ȃ�true, �ύX�ł��Ȃ��Ȃ�false���Ԃ�.
	bool PartsAnimChange( const int iIndex ){ return SetAnimChange( iIndex ); };

	//�p�[�c�̖��O���o����.
	void SetPartsName( const std::string &sPartsName ){ m_sPartsName = sPartsName; };

	//�{�[�������݂���Ȃ�true.
	bool ExistsBone( const char* sBoneName ) const { return m_pMesh->ExistsBone( sBoneName ); };

	void AddPosition( const D3DXVECTOR3& vPos ){ SetPosition( GetPosition() + vPos ); }

	void AddRotation( const D3DXVECTOR3& vRot ){ SetRotation( GetRotation() + vRot ); };

protected:
	//----- �e�p�[�c���Ƃ̊֐� -----//.
	virtual void InitProduct() = 0;//�e�V�[����Init.
	virtual void UpdateProduct() = 0;//�e�V�[����Update.
	//----- �e�p�[�c���Ƃ̊֐� -----//.

	void IntOverGuird( int* const i, const int start, const int end ) const {
		if( *i >= end ){
			*i = end - 1;
		}
		else if( *i < start ){
			*i = start;
		}
	}

	//�p�[�c�̖��O.
	std::string m_sPartsName;

	//----- �{�[�����̌��� -----//.
	//�u�[�X�^�[.
	const int m_iDIGIT_BOOSTER_NUM;
	//���̗�.
	const int m_iDIGIT_SOLE_NUM;
	//�֐�.
	const int m_iDIGIT_JOINT_NUM;

private:

};

#endif//#ifndef PARTS_BASE_H_