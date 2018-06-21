#pragma once

#include "DxInput.h"
#include "CXInput.h"
#include "Resource.h"
#include "Effects.h"
#include "SoundManagerBase.h"
#include "Camera.h"
#include "RoboStatus.h"
#include "BlackScreen.h"

class clsPOINTER_GROUP
{
public:
	//�����͐�Ες��Ȃ��̂ŃR���X�g���N�^�ŏ���������.
	clsPOINTER_GROUP( 
		ID3D11Device* const pDevice,
		ID3D11DeviceContext* const pContext,
		D3D10_VIEWPORT* const pViewPort,
		ID3D11DepthStencilState* const pDepthState,
		clsDxInput* const pDxInput,
		clsXInput* const pXInput,
		clsResource* const pResource,
		clsEffects* const pEffects,
		clsSOUND_MANAGER* const pSound,
		clsROBO_STATUS* const pRoboStatus,
		clsBLACK_SCREEN* const pBlack );
	~clsPOINTER_GROUP();

	//�J�����̃A�h���X�擾.
	void UpdateCameraPtr( clsCAMERA_BASE* const pCamera );

	//----- �|�C���^�[��n�� -----//.
	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetContext() const;
	D3D10_VIEWPORT* GetViewPort() const;
	ID3D11DepthStencilState* GetDepthState() const;
	clsDxInput* GetDxInput() const;
	clsXInput* GetXInput() const;
	clsResource* GetResource() const;
	clsEffects*	GetEffects() const;
	clsSOUND_MANAGER*	GetSound() const;
	clsCAMERA_BASE*	GetCamera() const;
	clsROBO_STATUS* GetRoboStatus() const;
	clsBLACK_SCREEN* GetBlackScreen() const;

private:
	//�f�o�C�X�I�u�W�F�N�g.
	ID3D11Device*			m_wpDevice;
	ID3D11DeviceContext*	m_wpContext;
	//2DSp�p.
	D3D10_VIEWPORT* m_wpViewPort;
	//�[�x(Z)�e�X�g�ݒ�.
	ID3D11DepthStencilState* m_wpDepthStencilState;

	//�W���C�X�e�B�b�N.
	clsDxInput*		m_wpDxInput;
	clsXInput*		m_wpXInput;
	//���\�[�X.
	clsResource* m_wpResource;
	//�G�t�F�N�g�N���X.
	clsEffects*	m_wpEffects;
	//BGM,SE.
	clsSOUND_MANAGER*	m_wpSound;

	//�J����( ���������V�[���؂�ւ����Ƃɕς�� ).
	clsCAMERA_BASE*		m_wpCamera;

	//( �V�[�����܂��� )���{�b�g�̃X�e�[�^�X.
	clsROBO_STATUS*		m_wpRoboStatus;

	//�Ó]�p���X�v���C�g.
	clsBLACK_SCREEN*	m_wpBlackScreen;
};

