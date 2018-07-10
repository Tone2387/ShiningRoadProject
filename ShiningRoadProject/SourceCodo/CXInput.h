#ifndef XINPUT_H_
#define XINPUT_H_

//アナログ入力を正規化する.
#define NORMALIZE_ANALOG_INPUT

//日吉君のDxInputとのすり合わせ( スティックの角度を[0～360]->[-180～180]にする ).
#define HIYOSHI_DX_INPUT

/*
	//----- Sceneクラスでの使用例 -----//.

	//ボタン.
	if( m_wpXInput->isPressEnter( XINPUT_B ) ){}//ボタン.
	if( m_wpXInput->isRTriggerEnter() ){}		//トリガーが押し込まれた瞬間.


	//各スティックの倒し具合.
	float fVar = m_wpXInput->GetLStickTheta(); 	//スティックの角度(方向)を返す( -180～180を弧度法の値で返す ).
	if( m_wpXInput->GetLStickSlope() > 0.95f ){}//どれだけ深く倒しているか：0.0f～1.0fで返す.

	//左のモーターの振動( この場合：最大の強さで160フレームの間振動し、1フレーム当たり5ずつ振動が弱くなる ).
	m_wpXInput->SetVibPowerL( XINPUT_VIBRATION_MAX, 160, 5 );	

*/

#include <Windows.h>
#include <Xinput.h>

#pragma comment( lib, "xinput.lib" )

//値.
#define XINPUT_TRIGGER_MIN		(0)		//トリガー.
#define XINPUT_TRIGGER_MAX		(255)
#define XINPUT_THUMB_MIN		(-32768)//スティック.
#define XINPUT_THUMB_MAX		(32767)
#define XINPUT_VIBRATION_MIN	(0)		//振動.
#define XINPUT_VIBRATION_MAX	(65535)




//キー.
#define XINPUT_UP		( XINPUT_GAMEPAD_DPAD_UP )
#define XINPUT_DOWN		( XINPUT_GAMEPAD_DPAD_DOWN )
#define XINPUT_LEFT		( XINPUT_GAMEPAD_DPAD_LEFT )
#define XINPUT_RIGHT	( XINPUT_GAMEPAD_DPAD_RIGHT )
#define XINPUT_START	( XINPUT_GAMEPAD_START )
#define XINPUT_BACK		( XINPUT_GAMEPAD_BACK )
#define XINPUT_LSTICK	( XINPUT_GAMEPAD_LEFT_THUMB )
#define XINPUT_RSTICK	( XINPUT_GAMEPAD_RIGHT_THUMB )
#define XINPUT_LB		( XINPUT_GAMEPAD_LEFT_SHOULDER )
#define XINPUT_RB		( XINPUT_GAMEPAD_RIGHT_SHOULDER )
#define XINPUT_A		( XINPUT_GAMEPAD_A )
#define XINPUT_B		( XINPUT_GAMEPAD_B )
#define XINPUT_X		( XINPUT_GAMEPAD_X )
#define XINPUT_Y		( XINPUT_GAMEPAD_Y )


class clsXInput
{
public:

	clsXInput();
	~clsXInput();

	//毎フレーム回す.
	bool UpdateStatus();
//	bool UpdateKeyStatus();//使わないかも?.

	//ボタン入力.
	bool isPressEnter( const WORD _padKey ) const;	//押した瞬間.
	bool isPressStay( const WORD _padKey ) const;	//押されてる間.
	bool isPressExit( const WORD _padKey ) const;	//離した瞬間.

	//トリガー、スティック入力.
#ifndef NORMALIZE_ANALOG_INPUT
	BYTE GetLTrigger() const;
	BYTE GetRTrigger() const;
	SHORT GetLStickX() const;
	SHORT GetLStickY() const;
	SHORT GetRStickX() const;
	SHORT GetRStickY() const;
#else//##ifndef NORMALIZE_ANALOG_INPUT
	float GetLTrigger() const;
	float GetRTrigger() const;
	float GetLStickX() const;
	float GetLStickY() const;
	float GetRStickX() const;
	float GetRStickY() const;
#endif//##ifndef NORMALIZE_ANALOG_INPUT
	//関数名をスティックにする.

	//LRトリガーをボタンのように扱おう.
	//( 「NORMALIZE_ANALOG_INPUT」 が定義されているか次第で中身が変わる ).
	bool isLTriggerEnter() const;
	bool isLTriggerStay() const;
	bool isLTriggerExit() const;
	bool isRTriggerEnter() const;
	bool isRTriggerStay() const;
	bool isRTriggerExit() const;



	//左スティックの角度.
	float GetLStickTheta() const;
	//右スティックの角度.
	float GetRStickTheta() const;

	//各スティックの倒し具合( どれだけ深く倒しているか：0.0f～1.0fで返す ).
	float GetLStickSlope() const;
	float GetRStickSlope() const;


	//振動を与えよう.//第一引数：振動の強さ, 第二引数：振動時間(何フレーム), 第三引数：減衰量(1フレームあたりの).
	void SetVibPowerL( int iVibL, const int iTime, int iVibDecL = 0 );
	void SetVibPowerR( int iVibR, const int iTime, int iVibDecR = 0 );

	//終了処理.
	void EndProc();

private:
	//振動.
	bool SetVibration( const WORD LMotorSpd, const WORD RMotorSpd );

	//スティックの角度.
	float GetStickTheta( const SHORT lY, const SHORT lX ) const;
	//スティックの傾き.
	float GetStickSlope( const SHORT lY, const SHORT lX ) const;


	//トリガー入力( isLTriggerとisRTriggerの為 ).
	BYTE GetLTriggerInside() const;
	BYTE GetRTriggerInside() const;
	//スティック入力( GetStickSlope()とGetStickTheta()の為 ).
	SHORT GetLThumbXInside() const;
	SHORT GetLThumbYInside() const;
	SHORT GetRThumbXInside() const;
	SHORT GetRThumbYInside() const;


	DWORD				m_padId;
	XINPUT_STATE		m_state;
	XINPUT_STATE		m_stateOld;//判断用.
	XINPUT_KEYSTROKE	m_keystroke;
	XINPUT_VIBRATION	m_vibration;

	//振動補助.
	void VibSafe( int &iVibPower, int &iVibDec ) const;//範囲内に収める.
	void VibDecrease( int &iVibPower, const int iDecPower ) const;//減衰.
	void VibTimerCnt();//タイマーカウント.
	int		m_iVibL;//振動力.
	int		m_iVibR;
	int		m_iVibDecL;//減衰する力.
	int		m_iVibDecR;
	int		m_iVibTimerL;//収まる時間.
	int		m_iVibTimerR;
	

};

#endif//#ifndef XINPUT_H_


