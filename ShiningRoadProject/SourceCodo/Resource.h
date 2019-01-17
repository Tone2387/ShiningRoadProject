#ifndef RESOURCE_H_
#define RESOURCE_H_

#define _CRT_SECURE_NO_WARNINGS


////�V���O���g���ɂ���Ȃ�΂���.
////#define RESOURCE_CLASS_SINGLETON


//�e�X�g���͂���( �p�[�c�̓ǂݍ��ݐ����Œ艻 ).
#define RESOURCE_READ_PARTS_MODEL_LOCK


#ifdef RESOURCE_READ_PARTS_MODEL_LOCK
	//�e�X�g���̃p�[�c�ő吔.
	const int iTEST_ROBO_PARTS_MODEL_MAX = 1;
#endif//#ifndef RESOURCE_READ_PARTS_MODEL_LOCK

//
////�e�X�g���f���ɑ���3�Ԃ̃��f�������蓖�Ă��.
//	m_pMesh->AttachModel(
//		m_wpResource->GetPartsModels( enPARTS::LEG, 3 ) );
//
//
////���܂�.
//	m_pMesh->AttachModel(
//		m_wpResource->GetSkinModels( clsResource::enSkinModel_Leg ) );
//

#include "Global.h"
#include "DX9Mesh.h"
#include "CD3DXSKINMESH.h"


//�X�L�����b�V���񋓑̂̌^.
using SKIN_ENUM_TYPE = UCHAR;


//3D���f���̂��ƃf�[�^���i�[����N���X.
//�V���O���g���ɂ��Ăǂ�����ł��Q�Ƃł���.
class clsResource
{
public:
	//�X�^�e�B�b�N���f�����.
	enum enSTATIC_MODEL : UCHAR
	{
		enStaticModel_StageCollision = 0,//�X�e�[�W�̓����蔻��.
		enStaticModel_StageFloor,	//�X�e�[�W�̏��ƕ�.
		enStaticModel_StageCelling,	//�X�e�[�W�̓V��.
		enStaticModel_Building,		//�X�e�[�W�̏�Q��.
		enStaticModel_Shpere,		//�����蔻��`�F�b�N�p.

		enStaticModel_Max
	};

	//�X�L�����f�����.
	enum enSKIN_MODEL : SKIN_ENUM_TYPE
	{
		enSkinModel_Door = 0,//�X�e�[�W�̃h�A.
		enSkinModel_Lia,	//�X�e�[�W�̓V��̃h�A.

		enSkinModel_Max//���Œ胂�f����max.
	};




#ifdef RESOURCE_CLASS_SINGLETON
	//�C���X�^���X�擾(�B��̃A�N�Z�X�o�H).
	static clsResource* GetInstance()
	{
		//�B��̃C���X�^���X���쐬����.
		//(static�ō쐬���ꂽ�̂�2��ڂ͖��������).
		static clsResource s_Instance;
		return &s_Instance;
	}
#else//#ifdef RESOURCE_CLASS_SINGLETON
	clsResource();
#endif//#ifdef RESOURCE_CLASS_SINGLETON
	~clsResource();

	//new����Ɏg��.
	void Create( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext );


	//�X�^�e�B�b�N���f��( Attach�̈��� ).
	clsDX9Mesh* GetStaticModels( const enSTATIC_MODEL enModel ) const;


	//�X�L�����f��( Attach�̈��� )( �p�[�c�͂���ł�Attach�o���Ȃ� ).
	clsD3DXSKINMESH*	GetSkinModels( const enSKIN_MODEL enModel ) const;

	//���{�̃p�[�c��Attach����֐�.
	//������ : ���̃p�[�c?.
	//������ : ���̃p�[�c�̉��Ԗ�?.
	clsD3DXSKINMESH* GetPartsModels(
		const enPARTS enParts, 
		const SKIN_ENUM_TYPE PartsNum );



private:

#ifdef RESOURCE_CLASS_SINGLETON
	//������R�s�[���֎~����.
	clsResource();
	clsResource( const clsResource& rhs );
	clsResource& operator = ( const clsResource& rhs );
#endif//#ifdef RESOURCE_CLASS_SINGLETON

	//�X�^�e�B�b�N���f��.
	HRESULT		InitStaticModel( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext );
	HRESULT		CreateStaticModel( LPSTR const fileName, const enSTATIC_MODEL enModel );
	HRESULT		ReleaseStaticModel( const enSTATIC_MODEL enModel );


	//�X�L�����f��.
	HRESULT		InitSkinModel( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext );
	HRESULT		CreateSkinModel( LPSTR const fileName, const enSKIN_MODEL enModel );
	HRESULT		ReleaseSkinModel( const enSKIN_MODEL enModel );


	//�p�[�c�쐬.
	void CreatePartsGroup();//CreateParts�̏W����.
	void CreateParts( const enPARTS enParts );//�����Ƃ���enPARTS::WEAPON_R�͎g��Ȃ�(�������̂�2����Ă��܂�).
	//CreateParts�ŕK�v�ȕϐ�����������.
	std::string SetVarToCreateParts(
		SKIN_ENUM_TYPE &ucStart,	//(out)���̃p�[�c�̎n�܂�ԍ�.
		SKIN_ENUM_TYPE &ucMax,	//(out)���̃p�[�c�̍ő�ԍ�.
		std::string &sModelName,//(out)�p�X�ɂ������郂�f����.
		const enPARTS enParts );
		

	//GetSkinModels()�̈������ǂ̃p�[�c���Ƃ��̃p�[�c�̔ԍ���������o���֐�.
	//������ : ���̃p�[�c?.
	//������ : ���̃p�[�c�̉��Ԗ�?.
	enSKIN_MODEL GetPartsResourceNum( 
		const enPARTS enParts, SKIN_ENUM_TYPE PartsNum ) const;

	//SetVarToCreateParts()��GetPartsResourceNum()�̕⏕.
	//���̃p�[�c�̍ŏ��̃i���o�[�����\�[�X�ԍ��ɂ��ċ����Ă����.
	SKIN_ENUM_TYPE GetPartsResourceStart( const enPARTS enParts ) const;


	//�͈͓����`�F�b�N����֐�.
	bool IsRangeStaticModel( const enSTATIC_MODEL enModel ) const;
	bool IsRangeSkinModel( const enSKIN_MODEL enModel ) const;


	//�p�[�c�̐���f���o��.
	//�ǂݍ��ރp�[�c���.
	enum enPARTS_READ : UCHAR
	{
		LEG = 0,
		CORE,
		HEAD,
		ARMS,
		WEAPON,

		enPARTS_READ_SIZE
	};
	SKIN_ENUM_TYPE GetPartsNum( const enPARTS_READ enPartsRead );

private:

	SKIN_ENUM_TYPE m_PartsNum[ enPARTS_READ_SIZE ];
	SKIN_ENUM_TYPE m_ucSkinModelMax;


	HWND					m_hWnd;
	ID3D11Device*			m_wpDevice11;
	ID3D11DeviceContext*	m_wpCotext11;
	CD3DXSKINMESH_INIT		m_Si;

	clsDX9Mesh**			m_ppStaticModels;
	clsD3DXSKINMESH**		m_ppSkinModels;

};

#endif//#ifndef RESOURCE_H_