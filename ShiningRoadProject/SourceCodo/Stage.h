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

	//����Ƃ���̐F���߂�.
	void SetColorTarget( const D3DXVECTOR4& vColor );
	void SetColor( const D3DXVECTOR4& vColor );

	void SetScale( const float fScale );

	//�����p�񋓑�.
	enum enDOOR : UCHAR
	{
		enDOOR_DOOR_0 = 0,
		enDOOR_DOOR_1,
		enDOOR_Lia,

		enDOOR_size
	};
	enum enDOOR_ANIM : int
	{
		enDOOR_ANIM_CLOSED = 0,
		enDOOR_ANIM_OPENING,
		enDOOR_ANIM_OPENED,
		enDOOR_ANIM_CLOSING,
	};
	void SetAnimDoor( const enDOOR enDoor, const enDOOR_ANIM enAnimNo );

private:

	//���̖���.
	void UpdateLight();

	//���̐��l�𓮂���.
	void Lighting( 
		float* const fpColor, 
		const float fTarget, 
		bool* const ispLiting );

	//�h�A���J������󂫂��ςȂ�.
	void SafeDoorAnimation();

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
	D3DXVECTOR4 m_vLightColorTarget;

	//���̃t���O.
	struct LIGHT_LIGHTING_FLG
	{
		//true:���邭�Ȃ�.
		bool isLightingR;
		bool isLightingG;
		bool isLightingB;

		LIGHT_LIGHTING_FLG()
		{
			Init();
		}
		void LIGHT_LIGHTING_FLG::Init()
		{
			isLightingR = false;
			isLightingG = false;
			isLightingB = false;
		}
		void LIGHT_LIGHTING_FLG::SetFlg( const D3DXVECTOR4& vColor, const D3DXVECTOR4& vTarget ){
			if( vColor.x > vTarget.x )	{ isLightingR = false; }
			else						{ isLightingR = true;  }
			if( vColor.y > vTarget.y )	{ isLightingG = false; }
			else						{ isLightingG = true;  }
			if( vColor.z > vTarget.z )	{ isLightingB = false; }
			else						{ isLightingB = true;  }
		}
	}	m_LightLightingFlg;

};
