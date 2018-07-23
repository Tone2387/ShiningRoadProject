#include "GameObject.h"
#include "Main.h"




clsGameObject::clsGameObject()
		: m_vPos( 0.0f, 0.0f, 0.0f )
		, m_vRot( 0.0f, 0.0f, 0.0f )
		, m_vScale( 1.0f, 1.0f, 1.0f )
{
//	m_vPos = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
//	m_vRot = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
//	m_fScale = 1.0f;
}


clsGameObject::~clsGameObject()
{
}



//============================================================
//位置設定.
//============================================================
//設定.
void clsGameObject::SetPosition( const D3DXVECTOR3 &vPos )
{	
	m_vPos = vPos;
}
void clsGameObject::SetPositionX( const float fPosX )
{
	m_vPos.x = fPosX;
}
void clsGameObject::SetPositionY( const float fPosY )
{
	m_vPos.y = fPosY;
}
void clsGameObject::SetPositionZ( const float fPosZ )
{
	m_vPos.z = fPosZ;
}
//取得.
D3DXVECTOR3 clsGameObject::GetPosition() const
{
	return m_vPos;
}
float clsGameObject::GetPositionX() const
{
	return m_vPos.x;
}
float clsGameObject::GetPositionY() const
{
	return m_vPos.y;
}
float clsGameObject::GetPositionZ() const
{
	return m_vPos.z;
}
//足しこみ.
void clsGameObject::AddPosition( const D3DXVECTOR3 &vMove )
{
	SetPosition( GetPosition() + vMove );
}
void clsGameObject::AddPositionX( const float fMoveX )
{
	SetPositionX( GetPositionX() + fMoveX );
}
void clsGameObject::AddPositionY( const float fMoveY )
{
	SetPositionY( GetPositionY() + fMoveY );
}
void clsGameObject::AddPositionZ( const float fMoveZ )
{
	SetPositionZ( GetPositionZ() + fMoveZ );
}


//============================================================
//回転設定.
//============================================================
//設定.
void clsGameObject::SetRotation( const D3DXVECTOR3 &vRot )
{ 
	m_vRot = vRot;
}
void clsGameObject::SetRotationY( const float fRotY )
{
	m_vRot.y = fRotY;
}
void clsGameObject::SetRotationZ( const float fRotZ )
{
	m_vRot.z = fRotZ;
}
//取得.
D3DXVECTOR3 clsGameObject::GetRotation() const
{
	return m_vRot;
}
float clsGameObject::GetRotationX() const
{
	return m_vRot.x;
}
float clsGameObject::GetRotationY() const
{
	return m_vRot.y;
}
float clsGameObject::GetRotationZ() const
{
	return m_vRot.z;
}
//足しこみ.
void clsGameObject::AddRotation( const D3DXVECTOR3 &vMove )
{
	D3DXVECTOR3 theta = GetRotation() + vMove;
	ThetaOverGuard( theta.x );
	ThetaOverGuard( theta.y );
	ThetaOverGuard( theta.z );
	SetRotation( theta );
}
void clsGameObject::AddRotationY( const float fMoveY )
{
	float theta = GetRotationY() + fMoveY;
	ThetaOverGuard( theta );
	SetRotationY( theta );
}
void clsGameObject::AddRotationZ( const float fMoveZ )
{
	float theta = GetRotationZ() + fMoveZ;
	ThetaOverGuard( theta );
	SetRotationZ( theta );
}

//============================================================
//拡縮設定.
//============================================================
void clsGameObject::SetScale( const float fScale )
{
	m_vScale.x = fScale;
	m_vScale.y = fScale;
	m_vScale.z = fScale;
}
void clsGameObject::SetScale( const D3DXVECTOR3 &vScale )
{
	m_vScale = vScale;
}
D3DXVECTOR3 clsGameObject::GetScale() const
{
	return m_vScale;
}


//============================================================
//	360～0に納める.
//============================================================
void clsGameObject::ThetaOverGuard( float& theta )
{
#define THETA_MAX (float)( M_PI * 2.0 )
	if( theta > THETA_MAX ){
		theta -= THETA_MAX;
	}
	else if( theta < 0.0f ){
		theta += THETA_MAX;
	}
}
void clsGameObject::ThetaOverGuard( double& theta )
{
#define THETA_MAX ( M_PI * 2.0 )
	if( theta > THETA_MAX ){
		theta -= THETA_MAX;
	}
	else if( theta < 0.0 ){
		theta += THETA_MAX;
	}
}


//==================================================
//	距離で音量を変える.
//==================================================
int clsGameObject::ChangeVolumeDistance( const float fEarPosZ, const float fThisPosZ )
{
	const float VOL_DEAD_RANGE_NEAR = 24.0f;//音が消える距離.
	const float VOL_DEAD_RANGE_BACK = -8.0f;//音が消える距離.
	//音が小さくなる距離（奥に行っている時の小さくなる速さ）.
	const float VOL_BACK_RATE = VOL_DEAD_RANGE_NEAR / VOL_DEAD_RANGE_BACK;//マイナスはつけなくてよい.
	//									（奥側の時は距離計算の過程でマイナスがつくから打ち消しあう）.

	const int iVOL_MAX = 1000;

	//距離を出す.
	float fRange = fThisPosZ - fEarPosZ;

	//離れていると音を出さない.
	//手前すぎる(Playerが).
	if( fRange > VOL_DEAD_RANGE_NEAR ){
		return 0;
	}
	//奥すぎる(Playerが).
	else if( fRange < VOL_DEAD_RANGE_BACK ){
		return 0;
	}
	//間近だと最大.
	else if( fRange == 0.0f ){
		return iVOL_MAX;
	}

	
	//音量変化.
	int vol;
	vol = (int)( (float)iVOL_MAX / fRange );//反比例.//intの計算するとVSが壊れるから(float)にしてから(int)に戻す.

	//奥側なら小さくなるのを速くする.
	if( fRange < 0.0f ){
		vol = (int)( (float)vol / VOL_BACK_RATE );//反比例.
	}

	return vol;
}
