#ifndef SCENE_CLEAR_H_
#define SCENE_CLEAR_H_

class clsStage;
class clsROBO_TAKEOFF;

#include "SceneMovieBase.h"
//#include "CameraTakeoff.h"

class clsSCENE_MOVIE_CLEAR : public clsSCENE_MOVIE_BASE
{
public:
	clsSCENE_MOVIE_CLEAR( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_MOVIE_CLEAR();

private:
	void CreateProduct() final;
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
		enCUT_START = 0,//�G���߂�.
		enCUT_END,		//�G�唚��.

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

	float m_fBomberEffectOffsetFrame;

	//���̈���.
//	enum enBGM : int
//	{
////		enBGM_MAFIA0 = 0,
//	};

	enum enSE : int
	{
		enSE_KNEE = 0,	//�G��.( �j�[ ).
		enSE_BOMBER,	//����.
		enSE_ARM_DOWN,	//���������.
	};

};



#endif//#ifndef SCENE_CLEAR_H_