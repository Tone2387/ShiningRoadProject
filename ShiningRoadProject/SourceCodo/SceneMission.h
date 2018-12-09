#pragma once

#include "SceneBase.h"
#include "CameraMission.h"

#include"Player.h"
#include"TestObject.h"
#include"Stage.h"
#include"Sprite.h"
#include"EnemyFactory.h"
#include"FriendFactory.h"

//================================//
//========== �~�b�V�������N���X ==========//
//================================//
class clsSCENE_MISSION : public clsSCENE_BASE
{
public:
	//�����̓��\�[�X�Ȃǂ̃|�C���^�̂܂Ƃ�( �����͊��N���X�Ŏ�舵���Ă���̂œ��Ɉӎ����Ȃ��đ��v�ł� ).
	clsSCENE_MISSION( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_MISSION();

//	//�uSceneManager.cpp�v�́uSwitchScene�v�֐����ŃV�[���쐬����Ɏg�p�����.
//	void Create() final;
//
//	//���[�v���̏���( �����̃t���O�����֐����ŕύX����ƍ��̃V�[�����j������A.
//	//				  �w�肵���V�[������������� ).
//	//���̃t���O�̏ڍׂ́uSceneClass.h�v�́uenSCENE�v�����Ă�������.
//	void Update( enSCENE &enNextScene ) final;
//
//	//�V�[�����̃I�u�W�F�N�g�̕`��֐��̂܂Ƃ�.
//	void Render() final;

private:

	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//�u UI�� �vRender.

#ifdef _DEBUG
	//�f�o�b�N�J�e�L�X�g�̕\��.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

	clsPlayer* m_pPlayer;
	clsTestObj* m_pTestObj;
	clsCharactor* m_pCamTar;

	bool m_bCamTarChange;
	bool m_bStartFlg;

	std::vector<clsCharactor*> m_v_pFriends;
	std::vector<clsCharactor*> m_v_pEnemys;

	void CreateFriends();
	void CreateEnemys();

	void SetEnemys();//�G�����̍쐬�̌�ɂ��݂��̓G��F��������.

	void Collison();
	
	//���L������Shot�����L������Body�ɓ����锻������邩�͏����̌��ˍ����œ����.
	void ColFShottoFBody();
	bool ColFShottoEBody();

	void ColEShottoFBody();
	void ColEShottoEBody();

	//�d�����̓���.
	void Duplicate();
	void ColFtoFDuplicate();
	void ColFtoEDuplicate();
	void ColEtoFDuplicate();

	bool AllEnemyDead();

	bool m_bEnemyStop;

	//�e�X�g�p���f��( ����͏����Ă��ǂ��ł��A����Ȃ��ł� ).
	void UpdateCamTargetPos(clsCharactor* pChara);
	void GameStart();

	D3DXVECTOR3 m_vCamTargetPos;
	D3DXVECTOR3 m_vLookTargetPos;
	
	//clsCharaStatic* m_pTestChara;

	clsStage* m_pStage;

	float m_fCamMoveSpeed;

	void CreateUI();

	int m_iStartCnt;
	clsUiText* m_pStartText;

	clsSPRITE2D_CENTER* m_pRaderWindowFront;
	clsSPRITE2D_CENTER* m_pRaderWindowBack;
	std::vector<clsSPRITE2D_CENTER*> m_v_pRaderEnemyMark;

	float m_fRaderSizeW;
	float m_fRaderSizeH;

	float m_fRaderMarkSizeW;
	float m_fRaderMarkSizeH;

	float m_fRaderDis;

	clsSPRITE2D_CENTER* m_pLockWindow;
	clsSPRITE2D_CENTER* m_pLWeaponLockMark;
	clsSPRITE2D_CENTER* m_pRWeaponLockMark;
	float m_fHitMarkRaderSizeW;
	float m_fHitMarkRaderSizeH;

	clsSPRITE2D_CENTER* m_pHitMark;
	int iHitDispTime;

	clsSPRITE2D_CENTER* m_pEnelgy;
	clsSPRITE2D_CENTER* m_pEnelgyFrame;

	clsSPRITE2D_CENTER* m_pCursorFrame;
	clsSPRITE2D_CENTER* m_pCursor;
	float m_fCursorSize;

	clsUiText* m_pHP;

	clsSPRITE2D_CENTER* m_pLBulletMark;
	clsSPRITE2D_CENTER* m_pRBulletMark;

	clsUiText* m_pLBulletNum;
	clsUiText* m_pRBulletNum;

	clsUiText* m_pLimitTime;

	clsPlayer* CreatePlayer();
	clsTestObj* CreateEnemy();

	//���N���X�Ɉȉ�������܂�.
	//�J����.
	//���\�[�X.
	//�G�t�F�N�g.
	//�T�E���h.

};