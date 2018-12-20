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

	//�J�����̃J�b�g����̒i�K.
	enum enCUT : int
	{
		enCUT_START = 0,
			
		enCUT_END,//�����ɗ����u�ԂɃ~�b�V�����V�[���ɔ��.

		enCUT_size
	};
	void NextCut( enCUT* const penCut );


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
	float m_fMovieFrame = 0;
	//�S�J�b�g�̃t���[����.
	float m_fMovieFrameNextArray[ enCUT_size ];


	std::unique_ptr< clsStage >	m_upStage;
	std::unique_ptr< clsRobo >	m_upPlayer;
	std::unique_ptr< clsRobo >	m_upEnemy;

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