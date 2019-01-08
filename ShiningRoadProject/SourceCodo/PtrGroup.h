#ifndef PTR_GROUP_H_
#define PTR_GROUP_H_

class clsDxInput;
class clsXInput;
class clsResource;
class clsEffects;
class clsSOUND_MANAGER_BASE;
class clsCAMERA_BASE;
class clsROBO_STATUS_PLAYER;
class clsBLACK_SCREEN;
class clsFont;


#include "Global.h"
#include "RoboStatusPlayer.h"
#include "DxInput.h"
#include "CXInput.h"
#include "SoundManagerBase.h"
#include "Effects.h"


//�e�V�[���ɕK�v�Ȃ��̂̂܂Ƃ�.
class clsPOINTER_GROUP
{
public:
	//�����͐�Ες��Ȃ��̂ŃR���X�g���N�^�ŏ���������.
	clsPOINTER_GROUP( 
		ID3D11Device* const pDevice,
		ID3D11DeviceContext* const pContext,
		D3D10_VIEWPORT* const pViewPort10,
		D3D11_VIEWPORT* const pViewPort11,
		clsDxInput* const pDxInput,
		clsXInput* const pXInput,
		clsResource* const pResource,
		clsEffects* const pEffects,
		clsSOUND_MANAGER_BASE* const pSound,
		clsROBO_STATUS_PLAYER* const pRoboStatus,
		clsBLACK_SCREEN* const pBlack,
		clsFont* const pFont );
	~clsPOINTER_GROUP();

	//�T�E���h�̃A�h���X�擾.
	void UpdateSoundPtr( clsSOUND_MANAGER_BASE* const pSound );
	//�J�����̃A�h���X�擾.
	void UpdateCameraPtr( clsCAMERA_BASE* const pCamera );

	//----- �|�C���^�[��n�� -----//.
	ID3D11Device*			GetDevice()		const;
	ID3D11DeviceContext*	GetContext()	const;
	D3D10_VIEWPORT*			GetViewPort10() const;
	D3D11_VIEWPORT*			GetViewPort11() const;
	clsDxInput*				GetDxInput()	const;
	clsXInput*				GetXInput()		const;
	clsResource*			GetResource()	const;
	clsEffects*				GetEffects()	const;
	clsSOUND_MANAGER_BASE*	GetSound()		const;
	clsCAMERA_BASE*			GetCamera()		const;
	clsROBO_STATUS_PLAYER*	GetRoboStatus() const;
	clsBLACK_SCREEN*		GetBlackScreen()const;
	clsFont*				GetFont()		const;

private:
	//�f�o�C�X�I�u�W�F�N�g.
	ID3D11Device*			m_wpDevice;
	ID3D11DeviceContext*	m_wpContext;
	//2DSp�p.
	D3D10_VIEWPORT* m_wpViewPort10;
	//�����p.
	D3D11_VIEWPORT* m_wpViewPort11;

	//�W���C�X�e�B�b�N.
	clsDxInput*		m_wpDxInput;
	clsXInput*		m_wpXInput;
	//���\�[�X.
	clsResource* m_wpResource;
	//�G�t�F�N�g�N���X.
	clsEffects*	m_wpEffects;
	//BGM,SE.
	clsSOUND_MANAGER_BASE*	m_wpSound;

	//�J����( ���������V�[���؂�ւ����Ƃɕς�� ).
	clsCAMERA_BASE*		m_wpCamera;

	//( �V�[�����܂��� )���{�b�g�̃X�e�[�^�X.
	clsROBO_STATUS_PLAYER*		m_wpRoboStatus;

	//�Ó]�p���X�v���C�g.
	clsBLACK_SCREEN*	m_wpBlackScreen;

	//�t�H���g.
	clsFont*			m_wpFont;
};

#endif//#ifndef PTR_GROUP_H_