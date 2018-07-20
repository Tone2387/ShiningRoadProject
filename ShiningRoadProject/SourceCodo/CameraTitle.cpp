#include "CameraTitle.h"
#define _USE_MATH_DEFINES
#include <math.h>

struct INIT_DATA
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vLook;
	D3DXVECTOR3 vRot;
};

//----- 最初 -----//.
const INIT_DATA START_INIT_DATA = 
{
	{ -120.0f, 5.0f, -400.0f },
	{ -120.0f, 30.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f }
};
const float fSTART_MOVE_SPD = 1.5f;
const float fSTART_END_POS_Z = 70.0f;
const float fSTART_ROT_Y = -0.29f;

const float fSTART_END_POS_Y = 47.0f;
const float fSTART_END_LOOK_Y = -53.0f;

//----- 最初 -----//.


clsCAMERA_TITLE::clsCAMERA_TITLE()
	:m_enMode( enMODE::START )
	,m_fDistance()
{
	Init( enMODE::START );
}

clsCAMERA_TITLE::~clsCAMERA_TITLE()
{
}

void clsCAMERA_TITLE::Create()
{
}

void clsCAMERA_TITLE::Update()
{
	//二点間の距離( 2線 ).
	float fX, fZ;
	fX = m_vPos.x - m_vLook.x;
	fZ = m_vPos.z - m_vLook.z;

	//現状の角度取得.
//	m_vRot.y = atan2f( -fX, -fZ );
////	m_vRot.y = atan2f( fZ, -fX );

	//二点間の距離( 斜線 ).
	fX *= fX;
	fZ *= fZ;
	m_fDistance = sqrtf( fX + fZ );

	//モード別動作.
	switch( m_enMode )
	{
	case enMODE::START:
		Advancing( fSTART_MOVE_SPD );
		if( abs( m_vPos.z ) < abs( fSTART_END_POS_Z )){
			Init( enMODE::IDLE );
		}
		break;
	case enMODE::IDLE:
		break;
	case enMODE::UP_1:
		break;
	case enMODE::SPN_L:
		break;
	case enMODE::SPN_R:
		break;
	default:
		m_vPos.x += 0.1f;
		break;
	}

	float fRotSpd = 1.0f / 360.0f * M_PI;
	float fMove = 1.0f;

	if( GetAsyncKeyState( 'D' ) & 0x8000 )	Turn(-fRotSpd );
	if( GetAsyncKeyState( 'A' ) & 0x8000 )	Turn( fRotSpd );
	if( GetAsyncKeyState( 'W' ) & 0x8000 )	Advancing( fMove );
	if( GetAsyncKeyState( 'S' ) & 0x8000 )	Advancing(-fMove );
	if( GetAsyncKeyState( 'E' ) & 0x8000 )	Spn(-fRotSpd );
	if( GetAsyncKeyState( 'Q' ) & 0x8000 )	Spn( fRotSpd );

	if( GetAsyncKeyState( 'H' ) & 0x8000 )	AddPos( { fMove, 0.0F, 0.0F } );
	if( GetAsyncKeyState( 'F' ) & 0x8000 )	AddPos( {-fMove, 0.0F, 0.0F } );
	if( GetAsyncKeyState( 'T' ) & 0x8000 )	AddPos( { 0.0F, fMove, 0.0F } );
	if( GetAsyncKeyState( 'G' ) & 0x8000 )	AddPos( { 0.0F,-fMove, 0.0F } );
	if( GetAsyncKeyState( 'R' ) & 0x8000 )	AddPos( { 0.0F, 0.0F, fMove } );
	if( GetAsyncKeyState( 'Y' ) & 0x8000 )	AddPos( { 0.0F, 0.0F,-fMove } );

	if( GetAsyncKeyState( 'L' ) & 0x8000 )	AddLookPos( { fMove, 0.0F, 0.0F } );
	if( GetAsyncKeyState( 'J' ) & 0x8000 )	AddLookPos( {-fMove, 0.0F, 0.0F } );
	if( GetAsyncKeyState( 'I' ) & 0x8000 )	AddLookPos( { 0.0F, fMove, 0.0F } );
	if( GetAsyncKeyState( 'K' ) & 0x8000 )	AddLookPos( { 0.0F,-fMove, 0.0F } );
	if( GetAsyncKeyState( 'U' ) & 0x8000 )	AddLookPos( { 0.0F, 0.0F, fMove } );
	if( GetAsyncKeyState( 'O' ) & 0x8000 )	AddLookPos( { 0.0F, 0.0F,-fMove } );

}

//各モードの初期化.
void clsCAMERA_TITLE::Init( const enMODE enMode )
{
	INIT_DATA InitData = { m_vPos, m_vLook, m_vRot, };
	float fRotY = 0.0f;
	m_enMode = enMode;

	switch( m_enMode )
	{
	case enMODE::START:
		InitData = START_INIT_DATA;
		fRotY = fSTART_ROT_Y;
		break;
	case enMODE::UP_1:
		break;
	case enMODE::SPN_L:
		break;
	case enMODE::SPN_R:
		break;
	}

	SetPos		( InitData.vPos );
	SetLookPos	( InitData.vLook );
	SetRot		( InitData.vRot );

	//Turnに必要.
	//二点間の距離( 2線 ).
	float fX, fZ;
	fX = m_vPos.x - m_vLook.x;
	fZ = m_vPos.z - m_vLook.z;
	fX *= fX;
	fZ *= fZ;
	m_fDistance = sqrtf( fX + fZ );

	Turn( fRotY );
}


//監視対象を中心に旋回する.
void clsCAMERA_TITLE::Spn( const float fSpn )
{
	m_vRot.y -= fSpn;

	m_vPos.x = m_vLook.x + cosf( m_vRot.y - M_PI_2 ) * m_fDistance;
	m_vPos.z = m_vLook.z + sinf( m_vRot.y - M_PI_2 ) * m_fDistance;
}

//カメラ位置を中心にして見回す.
void clsCAMERA_TITLE::Turn( const float fTurn )
{
	m_vRot.y += fTurn;

	m_vLook.x = m_vPos.x + cosf( m_vRot.y + M_PI_2 ) * m_fDistance;
	m_vLook.z = m_vPos.z + sinf( m_vRot.y + M_PI_2 ) * m_fDistance;

}



//今のカメラ向きに前進.advancing
void clsCAMERA_TITLE::Advancing( const float fMove )
{
	D3DXMATRIX mYaw;
	D3DXMatrixRotationY( &mYaw, m_vRot.y );

	D3DXVECTOR3 vAxisZ( 0.0f, 0.0f, 1.0f );

	//Z軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord(
		&vAxisZ,
		&vAxisZ,
		&mYaw );


	m_vPos.z  += vAxisZ.z * fMove;
	m_vLook.z += vAxisZ.z * fMove;

	m_vPos.x  -= vAxisZ.x * fMove;
	m_vLook.x -= vAxisZ.x * fMove;

}


