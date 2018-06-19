#include "CXInput.h"
#define _USE_MATH_DEFINES
#include <math.h>




//更新.
bool clsXInput::UpdateStatus(){

	m_stateOld = m_state;//判断用.

	//振動制限時間.
	VibTimerCnt();

	//振動減衰.
	VibDecrease( m_iVibL, m_iVibDecL );
	VibDecrease( m_iVibR, m_iVibDecR );

	//オーバーしない.
	VibSafe( m_iVibL, m_iVibDecL );
	VibSafe( m_iVibR, m_iVibDecR );

	//振動.
	SetVibration( m_iVibL, m_iVibR );

	if( ERROR_SUCCESS == XInputGetState(
		m_padId,
		&m_state )) 
	{
		return true;
	}
	return false;
}
//更新( 使わない? ).
bool clsXInput::UpdateKeyStatus(){
	if( ERROR_SUCCESS == XInputGetKeystroke(
		m_padId,
		0,
		&m_keystroke ) )
	{
		return true;
	}
	return false;
}

bool clsXInput::isPressStay( const WORD _padKey ) const
{
	if( m_state.Gamepad.wButtons & _padKey ){
		return true;
	}
	return false;
}
bool clsXInput::isPressEnter( const WORD _padKey ) const
{
	if( m_state.Gamepad.wButtons & _padKey &&
		!( m_stateOld.Gamepad.wButtons & _padKey ) )
	{
		return true;
	}
	return false;
}
bool clsXInput::isPressExit( const WORD _padKey ) const
{
	if( !( m_state.Gamepad.wButtons & _padKey ) &&
		m_stateOld.Gamepad.wButtons & _padKey )
	{
		return true;
	}
	return false;
}

//LRトリガーをボタンのように扱おう.
bool clsXInput::isLTriggerEnter() const
{
	if( GetLTrigger() >= INPUT_TRIGGER_MAX &&
		m_stateOld.Gamepad.bLeftTrigger < INPUT_TRIGGER_MAX )
	{
		return true;
	}
	return false;
}
bool clsXInput::isLTriggerStay() const
{
	if( GetLTrigger() > 0.0f ){
		return true;
	}
	return false;
}
bool clsXInput::isLTriggerExit() const
{
	if( GetLTrigger() < INPUT_TRIGGER_MAX &&
		m_stateOld.Gamepad.bLeftTrigger >= INPUT_TRIGGER_MAX )
	{
		return true;
	}
	return false;
}

bool clsXInput::isRTriggerEnter() const
{
	if( GetRTrigger() >= INPUT_TRIGGER_MAX &&
		m_stateOld.Gamepad.bRightTrigger < INPUT_TRIGGER_MAX )
	{
		return true;
	}
	return false;
}

bool clsXInput::isRTriggerStay() const
{
	if( GetRTrigger() > 0.0f ){
		return true;
	}
	return false;
}

bool clsXInput::isRTriggerExit() const
{
	if( GetRTrigger() < INPUT_TRIGGER_MAX &&
		m_stateOld.Gamepad.bRightTrigger >= INPUT_TRIGGER_MAX )
	{
		return true;
	}
	return false;
}




bool clsXInput::SetVibration( WORD LMotorSpd, WORD RMotorSpd )
{
	m_vibration.wLeftMotorSpeed		= LMotorSpd;
	m_vibration.wRightMotorSpeed	= RMotorSpd;

	if( ERROR_SUCCESS == XInputSetState(
		m_padId, &m_vibration )){
		return true;
	}
	return false;
}


//スティックの角度.
float clsXInput::GetStickTheta( const SHORT lY, const SHORT lX ) const
{
	double Theta = 0.0;
	Theta = atan2( (double)lY, (double)lX );

	//微調整.
	Theta += M_PI;

	//オーバーしない.
	if( Theta > M_PI * 2.0 )Theta -= M_PI * 2.0;
	else if( Theta < 0.0 )	Theta += M_PI * 2.0;

	return (float)Theta;
}
//スティックの傾き.
float clsXInput::GetStickSlope( const SHORT lY, const SHORT lX ) const
{
	LONG LX = lX * lX;
	LONG LY = lY * lY;

	float slope = (float)sqrt( (double)LY + (double)LX );

	return slope;
}


//左スティックの角度.
float clsXInput::GetLStickTheta() const
{
	return GetStickTheta( GetLThumbX(), GetLThumbY() );
}
//右スティックの角度.
float clsXInput::GetRStickTheta() const
{
	return GetStickTheta( GetRThumbX(), GetRThumbY() );
}

//各スティックの倒し具合.
float clsXInput::GetLStickSlope() const
{
	//斜めの長さが出る.
	float fSlope = GetStickSlope( GetLThumbY(), GetLThumbX() ); 

	fSlope = fSlope / INPUT_THUMB_MAX;

	return fSlope;
}
float clsXInput::GetRStickSlope() const
{
	float fSlope = GetStickSlope( GetRThumbY(), GetRThumbX() ); 

	fSlope = fSlope / INPUT_THUMB_MAX;

	return fSlope;
}


//範囲内に収める.
void clsXInput::VibSafe( int &iVibPower, int &iVibDec ) const
{
	if( iVibPower > INPUT_VIBRATION_MAX ){
		iVibPower = INPUT_VIBRATION_MAX;
	}
	else if( iVibPower < INPUT_VIBRATION_MIN ){
		iVibPower = INPUT_VIBRATION_MIN;
		iVibDec = 0;
	}
}

//振動衰弱.
void clsXInput::VibDecrease( int &iVibPower, const int iDecPower ) const
{
	iVibPower -= iDecPower;
}

//振動を与えよう.
void clsXInput::SetVibPowerL( int iVibL, const int iTime, int iVibDecL )
{
	if( iVibDecL < 0 ){ iVibDecL = 0; }
	m_iVibDecL = iVibDecL;//減衰する?.
	m_iVibTimerL = iTime;
	m_iVibL = iVibL;
	VibSafe( m_iVibL, iVibDecL );
	SetVibration( m_iVibL, m_iVibR );
}
void clsXInput::SetVibPowerR( int iVibR, const int iTime, int iVibDecR )
{
	if( iVibDecR < 0 ){ iVibDecR = 0; }
	m_iVibDecR = iVibDecR;//減衰する?.
	m_iVibTimerR = iTime;
	m_iVibR = iVibR;
	VibSafe( m_iVibR, iVibDecR );
	SetVibration( m_iVibL, m_iVibR );
}

//タイマーカウント.
void clsXInput::VibTimerCnt()
{
	//マイナスは減らさない(ずっと震える).
	if( m_iVibTimerL >= 0 ){
		if( m_iVibTimerL == 0 ){
			m_iVibL = 0;
		}
		m_iVibTimerL --;
	}

	if( m_iVibTimerR >= 0 ){
		if( m_iVibTimerR == 0 ){
			m_iVibR = 0;
		}
		m_iVibTimerR --;
	}
}
