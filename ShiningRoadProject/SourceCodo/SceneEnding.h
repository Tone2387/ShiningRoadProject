#ifndef SCENE_ENDING_H_
#define SCENE_ENDING_H_


#include "SceneBase.h"
#include "CameraEnding.h"

//#include "Stage.h"


//================================//
//========== �G���f�B���O�N���X ==========//
//================================//
class clsSCENE_ENDING : public clsSCENE_BASE
{
public:
	clsSCENE_ENDING( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_ENDING();


private:
	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//�u UI�� �vRender.


	bool isPressButtonAccel();
	bool isPressButtonEnd();

	//�e�L�X�g�̖���.
	void TextAlphaUpdate();


	struct TEXT_STATE
	{
		D3DXVECTOR3 vPos;
		float		fScale;
		float		fAlpha;
	};
	//�͈͂�over�����false���Ԃ��Ă���.
	bool AddAlphaState( TEXT_STATE* const pTextState, const float fAlpha )const;

#ifdef _DEBUG
	//�f�o�b�N�J�e�L�X�g�̕\��.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

private:


	//���̂΂œ��߂��镶���p�̃X�e�[�^�X.
	std::vector< std::unique_ptr< TEXT_STATE > > m_vecupTextStateAlpha;
	//�X�N���[�����镶���p�̃X�e�[�^�X.
	std::vector< std::unique_ptr< TEXT_STATE > > m_vecupTextStateScroll;

	unsigned int	m_uiSpriteCnt;
	bool			m_isSpriteAlphaAppear;
	int				m_iIntervalCnt;//�n�܂�܂ł̎��ԃJ�E���g.
	bool			m_isScroll;

	//�Ō�ɕ\������Y����.
	int m_iGoScrollIndex;

	//�X�N���[�����I������猈��{�^���Ń^�C�g���ɖ߂鋖���o��.
	bool m_isCanGoTitle;

	//�X�^�b�t���[�����ߕ����̓����\����.
	unsigned int m_uiRenderTextNum;

	//�{�^���������Ă˂ɕK�v.
	enum class encTEXT_ALPHA_MODE : UCHAR
	{
		PLUS = 0,
		NEXT_MINUS,//�}�C�i�X�̏���.
		MINUS,
		NEXT_PLUS
	}	m_encTextAlphaMode;
	int	m_iTextAlphaStopFrame;




	std::unique_ptr< clsSprite2D > m_upBack;
	std::unique_ptr< clsSPRITE2D_CENTER > m_upLogo;


	//���̈���.
	enum enBGM : int
	{
		enBGM_MAFIA0 = 0,
		enBGM_MAFIA1,
		enBGM_MUS0,
		enBGM_POKET0,
		enBGM_POKET1,
		enBGM_RENGOKU0,
		enBGM_HART0,
		enBGM_NOVA0,
	};

	enum enSE : int
	{
		enSE_CURSOL_MOVE = 0,
		enSE_ENTER,
		enSE_EXIT,
	};

};
#endif//#ifndef SCENE_ENDING_H_