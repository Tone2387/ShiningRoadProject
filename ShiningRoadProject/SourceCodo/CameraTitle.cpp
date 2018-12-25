#include "CameraTitle.h"
#define _USE_MATH_DEFINES
#include <math.h>

namespace{
	struct INIT_DATA
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vLook;
		D3DXVECTOR3 vRot;
	};

	//----- 最初 -----//.
	const INIT_DATA START_INIT_DATA = 
	{
	//	{ 8.963961f, 26.0f, 32.168831f },
		{ 8.963961f, 6.0f, 32.168831f },
	//	{ 8.963961f, 27.0f, 50.0f },
		{ 8.963961f, 11.0f, 50.0f },
		{ 0.0f, 0.0f, 0.0f }
	};
	const float fSTART_MOVE_SPD_INIT = -7.5f;
	const float fSTART_MOVE_ACC_INIT = 0.5f + 0.25f + 0.125f + 0.0625f;//0.5 0.25 0.125 0.0625 0.03125
	const float fSTART_ROT_Y = -0.29f;

	const float fSTART_END_SPD = 0.03125f * 2.0f;
	const float fSTART_CRAB_POWER = -24.0f;
	const float fSTART_ADVANC_POWER = 24.0f;
	//----- 最初 -----//.

	//----- 回転開始 -----//.
	const INIT_DATA SPN_START_INIT_DATA = 
	{
		{ -25.0253143f, 6.0f,			-81.9894485f },
		{ -29.6926422f, 30.8750f,		3.60752869f },
		{ 0.0f,			0.0544727743f,	0.0f }
	};
	//----- 回転開始 -----//.

	//----- 回転 -----//.
	const D3DXVECTOR3 vSPN_INIT_LOOK = { 0.0f, 30.875f, 0.0f };
	const D3DXVECTOR3 vSPN_MOVE_SPD = { 0.0f, 0.0f, 0.0f };
	const D3DXVECTOR3 vSPN_MOVE_ACC = { 0.000001f, 0.0f, 0.0005f };//0.0005.
	const float fSPN_SPN_SPD_LIMIT = 0.0008f;
	//----- 回転 -----//.

	//----- 回転中ズームもどき -----//.
	const float fSPN_ZOOM_CHANGE_SPD = 0.1f;//0.1
	//----- 回転中ズームもどき -----//.


	const float fYAW_OFFSET = static_cast<float>( M_PI_4 ) * ( 0.5f - 0.0625f );

}




clsCAMERA_TITLE::clsCAMERA_TITLE()
	:m_enMode( enMODE::START )
	,m_MoveFlg()
	,m_isFlash( false )
{
	Init( enMODE::START );
}

clsCAMERA_TITLE::~clsCAMERA_TITLE()
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
		m_vPos = m_CamGhost.vPos;
		m_vLook = m_CamGhost.vLook;
		m_vRot = m_CamGhost.vRot;

		AdvancingProduct( m_vMoveSpd.z, &m_vPos, &m_vLook, &m_vRot );

		m_CamGhost.vPos = m_vPos;
		m_CamGhost.vLook = m_vLook;
		m_CamGhost.vRot = m_vRot;

		if( abs( m_vMoveSpd.z ) < abs( fSTART_END_SPD )){
			Init( enMODE::SPN_L );
		}
		else{
			TurnProduct( fYAW_OFFSET, &m_vPos, &m_vLook, &m_vRot );
		}

		m_vMoveSpd.z *= m_vMoveAcc.z;
		break;
	case enMODE::SPN_L:
		//くるくる.
		CameraSpnFunction();
		if( m_vRot.y >= static_cast<float>( M_PI ) ){
			m_vRot.y = 0.0f;
//			Init( enMODE::SPN_L_2 );
		}
		break;
	case enMODE::SPN_L_2:
		CameraSpnFunction();
		CameraFakeZoomFunction();
		break;
	}

//	float fRotSpd = 1.0f / 360.0f * M_PI;
//	float fMove = 1.0f;
//
//	if( GetAsyncKeyState( 'D' ) & 0x8000 )	Turn(-fRotSpd );
//	if( GetAsyncKeyState( 'A' ) & 0x8000 )	Turn( fRotSpd );
//	if( GetAsyncKeyState( 'W' ) & 0x8000 )	Advancing( fMove );
//	if( GetAsyncKeyState( 'S' ) & 0x8000 )	Advancing(-fMove );
//	if( GetAsyncKeyState( 'E' ) & 0x8000 )	Spn(-fRotSpd );
//	if( GetAsyncKeyState( 'Q' ) & 0x8000 )	Spn( fRotSpd );
//
//	if( GetAsyncKeyState( 'H' ) & 0x8000 )	AddPos( { fMove, 0.0F, 0.0F } );
//	if( GetAsyncKeyState( 'F' ) & 0x8000 )	AddPos( {-fMove, 0.0F, 0.0F } );
//	if( GetAsyncKeyState( 'T' ) & 0x8000 )	AddPos( { 0.0F, fMove, 0.0F } );
//	if( GetAsyncKeyState( 'G' ) & 0x8000 )	AddPos( { 0.0F,-fMove, 0.0F } );
//	if( GetAsyncKeyState( 'R' ) & 0x8000 )	AddPos( { 0.0F, 0.0F, fMove } );
//	if( GetAsyncKeyState( 'Y' ) & 0x8000 )	AddPos( { 0.0F, 0.0F,-fMove } );
//
//	if( GetAsyncKeyState( 'L' ) & 0x8000 )	AddLookPos( { fMove, 0.0F, 0.0F } );
//	if( GetAsyncKeyState( 'J' ) & 0x8000 )	AddLookPos( {-fMove, 0.0F, 0.0F } );
//	if( GetAsyncKeyState( 'I' ) & 0x8000 )	AddLookPos( { 0.0F, fMove, 0.0F } );
//	if( GetAsyncKeyState( 'K' ) & 0x8000 )	AddLookPos( { 0.0F,-fMove, 0.0F } );
//	if( GetAsyncKeyState( 'U' ) & 0x8000 )	AddLookPos( { 0.0F, 0.0F, fMove } );
//	if( GetAsyncKeyState( 'O' ) & 0x8000 )	AddLookPos( { 0.0F, 0.0F,-fMove } );
//
//	if( GetAsyncKeyState( 'V' ) & 0x8000 )	AddDistance(-fMove, true );
//	if( GetAsyncKeyState( 'B' ) & 0x8000 )	AddDistance( fMove, true );
//	if( GetAsyncKeyState( 'N' ) & 0x8000 )	AddDistance(-fMove, false );
//	if( GetAsyncKeyState( 'M' ) & 0x8000 )	AddDistance( fMove, false );

}


//フラッシュ( ごまかし )のタイミングを教えてくれる.
bool clsCAMERA_TITLE::isFlash()
{
	bool bReturn = false;
	if( m_isFlash ) {
		bReturn = true;
	}
	m_isFlash = false;
	return bReturn;
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
		m_vMoveSpd = { 0.0f, 0.0f, fSTART_MOVE_SPD_INIT };
		m_vMoveAcc = { 0.0f, 0.0f, fSTART_MOVE_ACC_INIT };
		CrabWalkProduct( fSTART_CRAB_POWER, &InitData.vPos, &InitData.vLook, &InitData.vRot );
		AdvancingProduct( fSTART_ADVANC_POWER, &InitData.vPos, &InitData.vLook, &InitData.vRot );
		break;
	case enMODE::SPN_L:
		fRotY = fSTART_ROT_Y;
		InitData = SPN_START_INIT_DATA;
		m_vMoveSpd = vSPN_MOVE_SPD;
		m_vMoveAcc = vSPN_MOVE_ACC;
		m_isFlash = true;
		break;
	case enMODE::SPN_L_2:
		m_MoveFlg.z = true;
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


	TurnProduct( fRotY, &m_vPos, &m_vLook, &m_vRot );

	m_CamGhost.vPos = m_vPos;
	m_CamGhost.vLook = m_vLook;
	m_CamGhost.vRot = m_vRot;

}



//くるくる.
void clsCAMERA_TITLE::CameraSpnFunction()
{
	m_vPos = m_CamGhost.vPos;
	m_vLook = m_CamGhost.vLook;
	m_vRot = m_CamGhost.vRot;

	SpnProduct( -m_vMoveSpd.x, &m_vPos, &m_vLook, &m_vRot );

	m_CamGhost.vRot = m_vRot;
	m_CamGhost.vLook = m_vLook;

	TurnProduct( fYAW_OFFSET, &m_vPos, &m_vLook, &m_vRot );

	m_vMoveSpd.x += m_vMoveAcc.x;
	if( m_vMoveSpd.x > fSPN_SPN_SPD_LIMIT ){
		m_vMoveSpd.x = fSPN_SPN_SPD_LIMIT;
	}
	if( m_vRot.y >= static_cast<float>( M_PI ) * 2.0f ){
		m_vRot.y -= static_cast<float>( M_PI ) * 2.0f;
	}
}

//ズーム.
void clsCAMERA_TITLE::CameraFakeZoomFunction()
{
	//ズームっぽい動き.
	if( m_MoveFlg.z ){
		m_vMoveSpd.z += m_vMoveAcc.z;
		if( m_vMoveSpd.z > fSPN_ZOOM_CHANGE_SPD )
			m_MoveFlg.z = false;
	}
	else{
		m_vMoveSpd.z -= m_vMoveAcc.z;
		if( m_vMoveSpd.z < -fSPN_ZOOM_CHANGE_SPD ) 
			m_MoveFlg.z = true;
	}
	AddDistance( m_vMoveSpd.z, true );
}
