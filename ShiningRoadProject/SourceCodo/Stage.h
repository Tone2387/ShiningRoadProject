#pragma once

#include "Global.h"

class clsPOINTER_GROUP;
class clsBUILDING;
class clsObjStaticMesh;
class clsObjSkinMesh;
class clsDX9Mesh;



class clsStage
{
public:
	clsStage( clsPOINTER_GROUP* const pPtrGroup );
	~clsStage();

	void Render(				
		const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye );

	//�r���̓����`��.
	void RenderInside(				
		const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye );

	std::vector<clsDX9Mesh*> GetStageMeshArray();
	
	//�w�肵���I�u�W�F�N�ʒu��]�g�k���X�V.
	void SetStageObjTransform(const unsigned int uiObjNo);

	//����Ƃ���̐F.
	void SetColor( const D3DXVECTOR4& vColor ){ m_vLightColor = vColor; }

	void SetScale( const float fScale );

private:

	//�X�e�[�W�y��̓����蔻��.
	std::unique_ptr< clsObjStaticMesh > m_pStageCollision;
	
	//�X�e�[�W�y��̌�����.
	std::unique_ptr< clsObjStaticMesh > m_pStageFloor;	//���ƕ�.
	std::unique_ptr< clsObjStaticMesh > m_pStageCelling;//�V��.

	//�X�L�����b�V�����f��.
	enum enDOOR_NUM : int
	{
		enDOOR_NUM_0 = 0,
		enDOOR_NUM_1,

		enDOOR_NUM_size
	};
	//�ǂ̃h�A.
	std::unique_ptr< clsObjSkinMesh > m_pDoorArray[ enDOOR_NUM_size ];
	//�V��̃h�A.
	std::unique_ptr< clsObjSkinMesh > m_pLia;						

	//�r��.
	std::vector< std::unique_ptr< clsBUILDING > > m_vpBuilding;

	D3DXVECTOR4 m_vLightColor;

};
