#ifndef SCENE_TAKEOFF_H_
#define SCENE_TAKEOFF_H_

class clsStage;
class clsROBO_TAKEOFF;

#include "SceneMovieBase.h"

//#include "Stage.h"


//================================//
//========== �G���f�B���O�N���X ==========//
//================================//
class clsSCENE_MOVIE_TAKEOFF : public clsSCENE_MOVIE_BASE
{
public:
	clsSCENE_MOVIE_TAKEOFF( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_MOVIE_TAKEOFF();


private:
	void CreateProduct() final;
	//�p�����̂��̂��g��.
//	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//�u UI�� �vRender.

	void InitMovieProduct() final;
	void UpdateMovieProduct( int iOtherDataIndex ) final;

	//�w�肵���s�̃t�@�C���f�[�^���J�����ɗ^����.
	void SetCamPosFromFile( const int iFileRow );
	//�w�肵���s�̃t�@�C���f�[�^���M�K�|�������ɗ^����.
	void SetGigaponPosFromFile( const int iFileRow );

	void SetOtherDataFromFile( const int iFileRow );

	//�J�����̃J�b�g����̒i�K.
	enum enCUT : int
	{
		enCUT_START = 0,//�G���A�S�̂��ʂ�.
		enCUT_RED_1,	//�X�e�[�W���Ԃ��Ȃ�.
		enCUT_RED_2,		
		enCUT_RED_3,	//( �V����ʂ� ).
		enCUT_LIA_OPEN,	//�V�䂪�J��.
		enCUT_ENEMY_APP,//�V�䂩��G�o��.
		enCUT_ROAD,		//�L�������@���ʂ�߂���.
		enCUT_PLAYER_UP,//�ʘH���s�����@�̃A�b�v( ���� ).
		enCUT_PLAYER_ROAD,//�ʘH���s�����@( �w�� ).
		enCUT_PLAYER_APP,//�L��Ɍ���鎩�@.
		enCUT_ENCOUNT,	//�����ďo���@�̃M�K�|��.
		enCUT_ENEMY_LANDING,//�G���n.
		enCUT_VS,		//���@�̊�̃A�b�v.
		enCUT_END,		//�G�̊�̃A�b�v.

		enCUT_size
	};

#ifdef _DEBUG
	//�f�o�b�N�J�e�L�X�g�̕\��.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

private:


	std::unique_ptr< clsStage >	m_upStage;
	std::unique_ptr< clsROBO_TAKEOFF >	m_upPlayer;
	std::unique_ptr< clsROBO_TAKEOFF >	m_upEnemy;



	//���̈���.
	enum enBGM : int
	{
//		enBGM_MAFIA0 = 0,
	};

	enum enSE : int
	{
		enSE_SIREN = 0,		//Siren.
		enSE_DOOR_OPEN,		//DoorOpen.
		enSE_DOOR_CLOSE,	//DoorClose.
		enSE_PASS,			//�ʂ�߂���.
		enSE_FIGHT_LEADY,	//FightLeady.
		enSE_BOOSTER,		//Booster.
		enSE_LANDING,		//Landing.
		enSE_ENVIRONMENTAL,	//����.
	};

};



#endif//#ifndef SCENE_TAKEOFF_H_