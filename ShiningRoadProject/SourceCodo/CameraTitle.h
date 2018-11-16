#ifndef CAMERA_TITLE_H_
#define CAMERA_TITLE_H_

#include "Camera.h"


class clsCAMERA_TITLE : public clsCAMERA_BASE
{
public:
	clsCAMERA_TITLE();
	~clsCAMERA_TITLE();

	void Create() final {};

	void Update() final;

	//�t���b�V��( ���܂��� )�̃^�C�~���O�������Ă����.
	bool isFlash();

	D3DXVECTOR3 GetSpd() const { return m_vMoveSpd; };

	D3DXVECTOR3 GetAcc() const { return m_vMoveAcc; };


private:

	struct CAM_GHOST
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vLook;
		D3DXVECTOR3 vRot;

		CAM_GHOST(){
			vPos = vLook = vRot = { 0.0f, 0.0f, 0.0f };
		};
	};

	enum class enMODE : UCHAR
	{
		START = 0,	//�^�C�g����ʊJ�n��.

		SPN_L,		//�����ŉ�].
		SPN_L_2,	//�Y�[�����ǂ���������.
	}	m_enMode;

	//�e���[�h�̏�����.
	void Init( const enMODE enMode );

	//�Ď��Ώۂ𒆐S�ɐ��񂷂�.
	void Spn( 
		const float fSpn, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;

	//�J�����ʒu�𒆐S�ɂ��Č���.
	void Turn( const float fTurn, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;

	//���̃J���������ɑO�i.advancing
	void Advancing( 
		const float fMove, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;
	//�����s�ړ�.
	void CrabWalk( 
		const float fMove, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;

	//���_�Ƃ̋�����ύX.
	void AddDistance( const float fAdd, const bool isCamMove );


	//���邭��.
	void CameraSpnFunction();
	//�Y�[��.
	void CameraFakeZoomFunction();

	float m_fDistance;//�J�����Ǝ��_�̋���( x&z�������̓񎟌���ł� ).

	D3DXVECTOR3 m_vMoveSpd;	//���x.
	D3DXVECTOR3 m_vMoveAcc;	//�����x.

	//�ǂ���֓������̃t���O.
	struct BOOL3
	{
		bool x;
		bool y;
		bool z;
		BOOL3(){
			x = y = z = false;
		}
	}m_MoveFlg;

	CAM_GHOST m_CamGhost;

	bool m_isFlash;
};

#endif//#ifndef CAMERA_TITLE_H_