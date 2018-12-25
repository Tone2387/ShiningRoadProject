#include "CameraMovieBase.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace{

}

clsCAMERA_MOVIE_BASE::clsCAMERA_MOVIE_BASE()
	:m_fDistance( 0.0f )
{
}

clsCAMERA_MOVIE_BASE::~clsCAMERA_MOVIE_BASE()
{
}







//�Ď��Ώۂ𒆐S�ɐ��񂷂�.
void clsCAMERA_MOVIE_BASE::SpnProduct( 
	const float fSpn,		
	D3DXVECTOR3* const vPos,
	D3DXVECTOR3* const vLook,
	D3DXVECTOR3* const vRot ) const
{
	vRot->y -= fSpn;

	vPos->x = vLook->x + cosf( vRot->y - static_cast<float>( M_PI_2 ) ) * m_fDistance;
	vPos->z = vLook->z + sinf( vRot->y - static_cast<float>( M_PI_2 ) ) * m_fDistance;
}

//�J�����ʒu�𒆐S�ɂ��Č���.
void clsCAMERA_MOVIE_BASE::TurnProduct( 
	const float fTurn, 
	D3DXVECTOR3* const vPos,
	D3DXVECTOR3* const vLook,
	D3DXVECTOR3* const vRot ) const
{
	vRot->y += fTurn;

	vLook->x = vPos->x + cosf( vRot->y + static_cast<float>( M_PI_2 ) ) * m_fDistance;
	vLook->z = vPos->z + sinf( vRot->y + static_cast<float>( M_PI_2 ) ) * m_fDistance;

}



//���̃J���������ɑO�i.advancing
void clsCAMERA_MOVIE_BASE::AdvancingProduct( 
	const float fMove, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const
{
	D3DXMATRIX mYaw;
	D3DXMatrixRotationY( &mYaw, vRot->y );

	D3DXVECTOR3 vAxisZ( 0.0f, 0.0f, 1.0f );

	//Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord(
		&vAxisZ,
		&vAxisZ,
		&mYaw );


	vPos->z  += vAxisZ.z * fMove;
	vLook->z += vAxisZ.z * fMove;

	vPos->x  -= vAxisZ.x * fMove;
	vLook->x -= vAxisZ.x * fMove;

	vPos->y  += vAxisZ.y * fMove;
	vLook->y += vAxisZ.y * fMove;

}
//�����s�ړ�.
void clsCAMERA_MOVIE_BASE::CrabWalkProduct( 
	const float fMove, 
	D3DXVECTOR3* const vPos,
	D3DXVECTOR3* const vLook,
	D3DXVECTOR3* const vRot ) const
{
	D3DXMATRIX mYaw;
	D3DXMatrixRotationY( &mYaw, vRot->y );

	D3DXVECTOR3 vAxisX( 1.0f, 0.0f, 0.0f );

	//Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord(
		&vAxisX,
		&vAxisX,
		&mYaw );


	vPos->z  += vAxisX.z * fMove;
	vLook->z += vAxisX.z * fMove;

	vPos->x  -= vAxisX.x * fMove;
	vLook->x -= vAxisX.x * fMove;
}


//���_�Ƃ̋�����ύX.
void clsCAMERA_MOVIE_BASE::SetDistance( const float fAdd, const bool isCamMove )
{
	D3DXMATRIX mYaw;
	D3DXMatrixRotationY( &mYaw, m_vRot.y );

	D3DXVECTOR3 vAxisZ( 0.0f, 0.0f, 1.0f );

	//Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord(
		&vAxisZ,
		&vAxisZ,
		&mYaw );

	if( isCamMove ){
		m_vPos.z  = vAxisZ.z * fAdd;
		m_vPos.x  = vAxisZ.x * fAdd;
	}
	//�������͖�����.
	else{
		m_vLook.z = vAxisZ.z * fAdd;
		m_vLook.x = vAxisZ.x * fAdd;
	}
}

void clsCAMERA_MOVIE_BASE::AddDistance( const float fAdd, const bool isCamMove )
{
	D3DXMATRIX mYaw;
	D3DXMatrixRotationY( &mYaw, m_vRot.y );

	D3DXVECTOR3 vAxisZ( 0.0f, 0.0f, 1.0f );

	//Z���x�N�g�����̂��̂���]��Ԃɂ��ϊ�����.
	D3DXVec3TransformCoord(
		&vAxisZ,
		&vAxisZ,
		&mYaw );

	if( isCamMove ){
		m_vPos.z  += vAxisZ.z * fAdd;
		m_vPos.x  -= vAxisZ.x * fAdd;
	}
	//�������͖�����.
	else{
		m_vLook.z += vAxisZ.z * -fAdd;
		m_vLook.x -= vAxisZ.x * -fAdd;
	}
}

