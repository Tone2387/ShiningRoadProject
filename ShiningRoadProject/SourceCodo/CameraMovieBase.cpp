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







//監視対象を中心に旋回する.
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

//カメラ位置を中心にして見回す.
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



//今のカメラ向きに前進.advancing
void clsCAMERA_MOVIE_BASE::AdvancingProduct( 
	const float fMove, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const
{
	D3DXMATRIX mYaw;
	D3DXMatrixRotationY( &mYaw, vRot->y );

	D3DXVECTOR3 vAxisZ( 0.0f, 0.0f, 1.0f );

	//Z軸ベクトルそのものを回転状態により変換する.
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
//横平行移動.
void clsCAMERA_MOVIE_BASE::CrabWalkProduct( 
	const float fMove, 
	D3DXVECTOR3* const vPos,
	D3DXVECTOR3* const vLook,
	D3DXVECTOR3* const vRot ) const
{
	D3DXMATRIX mYaw;
	D3DXMatrixRotationY( &mYaw, vRot->y );

	D3DXVECTOR3 vAxisX( 1.0f, 0.0f, 0.0f );

	//Z軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord(
		&vAxisX,
		&vAxisX,
		&mYaw );


	vPos->z  += vAxisX.z * fMove;
	vLook->z += vAxisX.z * fMove;

	vPos->x  -= vAxisX.x * fMove;
	vLook->x -= vAxisX.x * fMove;
}


//視点との距離を変更.
void clsCAMERA_MOVIE_BASE::SetDistance( const float fAdd, const bool isCamMove )
{
	D3DXMATRIX mYaw;
	D3DXMatrixRotationY( &mYaw, m_vRot.y );

	D3DXVECTOR3 vAxisZ( 0.0f, 0.0f, 1.0f );

	//Z軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord(
		&vAxisZ,
		&vAxisZ,
		&mYaw );

	if( isCamMove ){
		m_vPos.z  = vAxisZ.z * fAdd;
		m_vPos.x  = vAxisZ.x * fAdd;
	}
	//こっちは未完成.
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

	//Z軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord(
		&vAxisZ,
		&vAxisZ,
		&mYaw );

	if( isCamMove ){
		m_vPos.z  += vAxisZ.z * fAdd;
		m_vPos.x  -= vAxisZ.x * fAdd;
	}
	//こっちは未完成.
	else{
		m_vLook.z += vAxisZ.z * -fAdd;
		m_vLook.x -= vAxisZ.x * -fAdd;
	}
}

