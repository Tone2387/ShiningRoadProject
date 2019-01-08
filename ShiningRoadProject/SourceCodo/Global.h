#ifndef GLOBAL_H_	//�C���N���[�h�K�[�h.
#define GLOBAL_H_


//�x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable : 4005 )
//�R���X�g���N�^�ł̔z�񏉊����̈�.
#pragma warning( disable : 4351 )

//���KyoStar.
#define Inoue

//����.
#define Tahara


//============================================================
//	�C���N���[�h.
//============================================================
#include "MyMacro.h"
	    
#include <Windows.h>
	    
#include <D3DX11.h>
#include <D3D11.h>
	    
#include <D3DX10.h>//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>

#include <assert.h>

#include <memory>

#include <string>
#include <vector>

#include <sstream>

//============================================================
//	���C�u����.
//============================================================
#pragma comment( lib, "winmm.lib" )

#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )

#pragma comment( lib, "d3dx10.lib" )//�uD3DX�`�v�̒�`�g�p���ɕK�v.



//============================================================
//	�萔.
//============================================================
#define WND_W 1280	//�E�B���h�E��.
#define WND_H 720	//�E�B���h�E����.

const float g_fFPS = 60.0f;

//�i�s�����񋓑�.
enum class enDIRECTION : UCHAR
{
	STOP	= 0,//��~.
	FOWARD,		//�O�i.
	BACK,	//���.
//	enDirection_LeftTurn,	//����].
//	enDirection_RightTurn	//�E��].
};


//���̃Q�[���̊e�V�[���̗񋓑�( �t���O�p ).
enum class enSCENE : UCHAR
{
	EXIT_APP = 0,	//�Q�[�������.

	TITLE,		//�^�C�g�����.
	ASSEMBLE,	//�p�[�c�g�݊������.
	TAKEOFF,	//�o�����[�r�[.
	MISSION,	//�A�N�V�����V�[��( �Q�[���̃��C�� ).
	ENDING,		//�N���A��.
	GAMEOVER,		//�Q�[���I�[�o�[.			

	NOTHING		//�uSwitchScene�v�֐��̃t���O�p.
				//�Ȃ�̃V�[�����w�肵�Ă��Ȃ�.
};

//���{�̍\���ɕK�v�ȃp�[�c��.
enum class enPARTS : UCHAR
{
	LEG = 0,
	CORE,
	HEAD,
	ARM_L,
	ARM_R,
	WEAPON_L,
	WEAPON_R,

	MAX
};

//�u�����h�X�e�[�g�̓Y�����p.
enum enBLEND_STATE : unsigned char
{
	enBLEND_STATE_ALPHA_ON = 0,
	enBLEND_STATE_ALPHA_OFF,

	enBLEND_STATE_size
};

struct TRANSFORM
{
	D3DXVECTOR3 vScale;
	float		fYaw;		//��](Y��)
	float		fPitch;	//��](X��)
	float		fRoll;	//��](Z��)
	D3DXVECTOR3	vPos;		//�ʒu(X,Y,Z)
};


#endif//#ifndef GLOBAL_H_