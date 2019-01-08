#ifndef SCENE_TAKEOFF_H_
#define SCENE_TAKEOFF_H_

class clsStage;
class clsRobo;

#include "SceneBase.h"
#include "CameraTakeoff.h"

//#include "Stage.h"


//================================//
//========== �G���f�B���O�N���X ==========//
//================================//
class clsSCENE_TAKEOFF : public clsSCENE_BASE
{
public:
	clsSCENE_TAKEOFF( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_TAKEOFF();


private:
	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//�u UI�� �vRender.

	//�w�肵���s�̃t�@�C���f�[�^���J�����ɗ^����.
	void SetCamPosFromFile( const int iFileRow );
	//�w�肵���s�̃t�@�C���f�[�^���M�K�|�������ɗ^����.
	void SetGigaponPosFromFile( const int iFileRow );

	void SetOtherDataFromFile( const int iFileRow );

	//�J�����̃J�b�g����̒i�K.
	enum enCUT : int
	{
//		enCUT_START = 0,//�ʘH��ʂ��l��.
//		enCUT_GO_YOU,	//�L��ɏo����l��.
//		enCUT_LOOK_UP,	//�V������グ��.
//		enCUT_ENEMY_APP,//�V�䂩��G�o��.
//		enCUT_ENEMY_LANDING,//�G���n.

		enCUT_START = 0,//�G���A�S�̂��ʂ�.
		enCUT_RED_1,	//�X�e�[�W���Ԃ��Ȃ�.
		enCUT_RED_2,		
		enCUT_RED_3,	//( �V����ʂ� ).
		enCUT_LIA_OPEN,	//�V�䂪�J��.
		enCUT_ENEMY_APP,//�V�䂩��G�o��.
		enCUT_PLAYER_UP,//�ʘH���s�����@�̃A�b�v( ���� ).
		enCUT_PLAYER_ROAD,//�ʘH���s�����@( �w�� ).
		enCUT_PLAYER_APP,//�L��Ɍ���鎩�@.
		enCUT_ENCOUNT,	//�����ďo���@�̃M�K�|��.
		enCUT_ENEMY_LANDING,//�G���n.
		enCUT_VS,		//���@�̊�̃A�b�v.
		enCUT_END,		//�G�̊�̃A�b�v.

		enCUT_size
	};
	//�J�b�g�ϐ����X�V.
	void AddCut( enCUT* const penCut );

	//���[�r�[���ۂ����������߂ׂ̈̊֐�.
	void InitMovie();
	void UpdateMovie();

	//�t���[�����������Ă��Ȃ��Ă����̃J�b�g�֔��.
	void NextCut();

#ifdef _DEBUG
	//�f�o�b�N�J�e�L�X�g�̕\��.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

private:

	//�J�b�g����.
	enCUT m_enCut;
	//.
	int m_iCountCameraCutChange;

	//���̃J�b�g�̃t���[���J�E���g.
	float m_fMovieFrame;
	//�S�J�b�g�̃t���[����.
	float m_fMovieFrameNextArray[ enCUT_size ];


	std::unique_ptr< clsStage >	m_upStage;
	std::unique_ptr< clsRobo >	m_upPlayer;
	std::unique_ptr< clsRobo >	m_upEnemy;

	//�J�����̈ړ����x�Ȃ�.
	std::vector< float > m_vecfOtherData;

	//��M�˔T�e�J�b�g��񂫂�̃t���O�g���K�[.
	bool m_isTrigger;

	//���̈���.
	enum enBGM : int
	{
//		enBGM_MAFIA0 = 0,
	};

	enum enSE : int
	{
//		enSE_CURSOL_MOVE = 0,
	};

};


#endif//#ifndef SCENE_TAKEOFF_H_