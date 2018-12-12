#ifndef BUILDING_H_
#define BUILDING_H_




#include "Global.h"
#include"ObjStaticMesh.h"
#include "Sprite.h"
#include <vector>

//�X�e�[�W�ɐݒu����r���N���X.
//�傫����ς��Ă��e�N�X�`���������L�΂���邱�Ƃ͂Ȃ�.
//����ȃu���b�N��������.
class clsBUILDING
{
public:
	clsBUILDING( 
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11,
		 clsDX9Mesh* const pModel );
	~clsBUILDING();

	void UpdateModel() const;


	//���t���[���g���Ă͂����Ȃ�.
	//TransForm��ύX�����Ƃ��ɂ����g��.
	void UpdateTile();

	void Render(
		const D3DXMATRIX &mView, 
		const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight, 
		const D3DXVECTOR3 &vEye,
		const D3DXVECTOR4& vColor = { 1.0f, 1.0f, 1.0f, 1.0f } ) const;

	void RenderInside(
		const D3DXMATRIX &mView, 
		const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight, 
		const D3DXVECTOR3 &vEye ) const;



	//���C�p.
	clsDX9Mesh* GetModelPtr() const {
#ifdef _DEBUG
		clsDX9Mesh* pReturn = m_upBox->GetStaticMesh();
		return pReturn;
#else//#ifdef _DEBUG
		return m_upBox->GetStaticMesh();
#endif//#ifdef _DEBUG
	}

	//���W.
	D3DXVECTOR3 GetPos() const				{ return m_Trans.vPos; }
	void SetPos( const D3DXVECTOR3& vPos )	{ m_Trans.vPos = vPos;	this->UpdateTile(); }
	void AddPos( const D3DXVECTOR3& vPos )	{ m_Trans.vPos += vPos; this->UpdateTile(); }
	//��].
	D3DXVECTOR3 clsBUILDING::GetRot() const				{ return m_Trans.vRot; }
	void clsBUILDING::SetRot( const D3DXVECTOR3& vRot )	{ m_Trans.vRot = vRot;	this->UpdateTile(); }
	void clsBUILDING::AddRot( const D3DXVECTOR3& vRot )	{ m_Trans.vRot += vRot; this->UpdateTile(); }


	D3DXVECTOR3 GetScale() const				{ return m_Trans.vScale; }
	void SetScale( const D3DXVECTOR3& vScale )	{ m_Trans.vScale = vScale;	this->UpdateTile(); }
	void AddScale( const D3DXVECTOR3& vScale )	{ m_Trans.vScale += vScale; this->UpdateTile(); }


private:

	struct TRANSFORM
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vRot;
		D3DXVECTOR3 vScale;
	};

	//�^�C���̖ڕW�������.
	void SetTileNumTargetTop( unsigned int& puiRow, unsigned int& puiCol );
	//�ڕW�̐��ɍ��킹�ă^�C���𑝌�����.
	void SetTileNumTop( const unsigned int uiROW, const unsigned int uiCOL );
	//�^�C������ׂ�.
	void SetTransformTop();

	//Z=�k�Ɠ�.//�����͎�����.
	void SetTileNumTargetSide( 
		unsigned int& puiRowZ, unsigned int&  puiColZ,
		unsigned int& puiRowX, unsigned int&  puiColX );
	//�ڕW�̐��ɍ��킹�ă^�C���𑝌�����.
	void SetTileNumSide( 
		const unsigned int uiROW_Z, const unsigned int uiCOL_Z,
		const unsigned int uiROW_X, const unsigned int uiCOL_X );
	//�^�C������ׂ�.
	void SetTransformSide();

	//�����ɓ��ꂽ�^�C�������S���猩�ĉ��x�̈ʒu�ɂ��邩.
	float GetTileTheta( 
		const TRANSFORM& Tile, const TRANSFORM& Center,
		float* const pfTheta, float* const pfDistance ) const;

	//��]�ɉ����č��W���X�V����.
	D3DXVECTOR3 GetTilePosForRotation( 
		D3DXVECTOR3* const vTilePos,
		const D3DXVECTOR3& vCenterPos,
		const float fTileTheta, 
		const float fTileDistance ) const;

private:

	enum enWALL_DIRECTION : int
	{
		enWD_SOUTH = 0,
		enWD_EAST,
		enWD_NORTH,
		enWD_WEST,

		enWALL_DIRECTION_size
	};

	//�{��.
	TRANSFORM m_Trans;
	//���.
	TRANSFORM m_TopTrans;
	//����.
	TRANSFORM m_SideTransArray[ enWALL_DIRECTION_size ];


	std::unique_ptr< clsObjStaticMesh > m_upBox;

	//�e�N�X�`��.
	std::unique_ptr< clsSprite > m_upTop;
	std::unique_ptr< clsSprite > m_upSide;
	std::unique_ptr< clsSprite > m_upSideInside;//����( �J�������r���̒��ɓ��������̈� ).

	ID3D11Device*  m_wpDevice;
	ID3D11DeviceContext* m_wpContext;

};
#endif//#ifndef BUILDING_H_