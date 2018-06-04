#include "DxInput.h"
#include<math.h>

LPDIRECTINPUT8 g_pDI2 = NULL;			// DxInputオブジェクト.
LPDIRECTINPUTDEVICE8 g_pPad2=NULL;	// デバイス(コントローラ)オブジェクト.


// ジョイスティック列挙関数.
BOOL CALLBACK EnumJoysticksCallBack( const DIDEVICEINSTANCE *pdidInstance, VOID *pContext )
{
	HRESULT hRlt;	// 関数復帰値.

	// デバイス(コントローラ)の作成.
	hRlt = g_pDI2->CreateDevice(
		pdidInstance->guidInstance,	// デバイスの番号.
		&g_pPad2,	// (out)作成されるデバイスオブジェクト.
		NULL );
	if( hRlt != DI_OK ){
		return DIENUM_CONTINUE;	// 次のデバイスを要求.
	}
	return DIENUM_STOP;	// 列挙停止.
}

// オブジェクトの列挙関数.
BOOL CALLBACK EnumObjectsCallBack( const DIDEVICEOBJECTINSTANCE *pdidoi, VOID *pContext )
{
	// 軸(スティック)を持っているか？
	if( pdidoi->dwType & DIDFT_AXIS )
	{
		// 入力情報の範囲を設定する
		DIPROPRANGE diprg;	// 範囲設定構造体

		diprg.diph.dwSize = sizeof( DIPROPRANGE );			// 構造体のサイズ
		diprg.diph.dwHeaderSize = sizeof( DIPROPHEADER );	// ヘッダー情報のサイズ
		diprg.diph.dwObj = pdidoi->dwType;					// 軸(オブジェクト)
		diprg.diph.dwHow = DIPH_BYID;						// dwObjに設定されるものが
															// オブジェクトの種類か？インスタンス番号か？

		diprg.lMax = +g_iAxisMax;	// 最大値
		diprg.lMin = -g_iAxisMax;	// 最小値

		// 範囲を設定
		if( FAILED( g_pPad2->SetProperty(
					DIPROP_RANGE,	// 範囲
					&diprg.diph ) ) )// 範囲設定構造体
		{
			return DIENUM_STOP;
		}
	}
	return DIENUM_CONTINUE;
}

//ｺﾝｽﾄﾗｸﾀ.
clsDxInput::clsDxInput() : m_DI(NULL), m_Pad(NULL)
{
	
}

//ﾃﾞｽﾄﾗｸﾀ.
clsDxInput::~clsDxInput()
{

}

// DirectInputの初期設定。
bool clsDxInput::initDI( HWND hWnd)
{
	HRESULT hRlt;	// 関数復帰値
	hRlt = DirectInput8Create(
			GetModuleHandle(NULL),	// DxInputを作成するアプリのハンドル
			DIRECTINPUT_VERSION,	// 固定：DxInputのバージョン
			IID_IDirectInput8,		// 固定：UnicodeとAnsiの区別用
			(VOID**)&m_DI,			// (out)DxInputオブジェクト
			NULL);					// NULL固定
	if( hRlt != DI_OK ){
		MessageBox(NULL, "DxInputの作成に失敗",	"エラー", MB_OK);
		return false;
	}

	g_pDI2 = m_DI;

	// 利用可能なコントローラを探す(列挙する)
	hRlt = (*m_DI).EnumDevices(
			DI8DEVCLASS_GAMECTRL,	// 全てのゲームコントローラ
			EnumJoysticksCallBack,	// コントローラの列挙関数
			NULL,					// コールバック関数からの値
			DIEDFL_ATTACHEDONLY);	// 繋がっているﾓﾉのみ
	if( hRlt != DI_OK ){
		MessageBox(NULL, "ｺﾝﾄﾛｰﾗの確認に失敗", "エラー", MB_OK);
	}

	m_Pad = g_pPad2;

	// コントローラの接続確認
	if( g_pPad2 == NULL ){
//		MessageBox( NULL, "ｺﾝﾄﾛｰﾗが接続されていません", "エラー", MB_OK );
	}
	else {
		// コントローラ構造体のデータフォーマットを作成
		hRlt = g_pPad2->SetDataFormat(
				&c_dfDIJoystick2);	//固定
		if( hRlt != DI_OK ){
			MessageBox( NULL, "ﾃﾞｰﾀﾌｫｰﾏｯﾄの作成失敗", "エラー", MB_OK );
		}
		// (他のデバイスとの)協調レベルの設定
		hRlt = g_pPad2->SetCooperativeLevel(
				hWnd,
				DISCL_EXCLUSIVE |	// 排他アクセス
				DISCL_FOREGROUND );	// フォアグラウンドアクセス権
		if( hRlt != DI_OK ){
			MessageBox( NULL, "協調ﾚﾍﾞﾙの設定失敗", "エラー", MB_OK );
		}
		// 使用可能なオブジェクト(ボタンなど)の列挙
		hRlt = g_pPad2->EnumObjects(
			EnumObjectsCallBack,	// オブジェクト列挙関数
			(VOID*)hWnd,			// コールバック関数に送る情報
			DIDFT_ALL );			// 全てのオブジェクト
		if( hRlt != DI_OK ){
			MessageBox( NULL, "ｵﾌﾞｼﾞｪｸﾄの列挙に失敗", "エラー", MB_OK );
		}
	}

	return true;
}

//入力情報更新関数.
HRESULT clsDxInput::UpdataInputState()
{
	HRESULT hRslt;//関数復帰値.
	DIJOYSTATE2 js;//ｼﾞｮｲｽﾃｨｯｸ情報構造体.

	//ｼﾞｮｲｽﾃｨｯｸの接続確認.
	if (m_Pad == NULL)
	{
		return E_FAIL;
	}

	//ｼﾞｮｲｽﾃｨｯｸが変更されているかを確認.
	hRslt = m_Pad->Poll();
	if (hRslt != DI_OK && hRslt != DI_NOEFFECT)//DI_NOEFFCT:確認不要なﾃﾞﾊﾞｲｽ.
	{
		//ｺﾝﾄﾛｰﾗｰへのｱｸｾｽ権を取得する.
		hRslt = m_Pad->Acquire();
		while (hRslt == DIERR_INPUTLOST)
		{
			hRslt = m_Pad->Acquire();
		}
		return S_OK;
	}

	//ｺﾝﾄﾛｰﾗｰから直接ﾃﾞｰﾀを取得する.
	hRslt = m_Pad->GetDeviceState(
		sizeof(DIJOYSTATE2), //取得するｻｲｽﾞ.
		&js);//(out)取得ﾃﾞｰﾀ.
	if (hRslt != DI_OK)
	{
		return hRslt;
	}

	//入力情報の初期化.
	InitInputState();

	const int iAxisPushMin = g_iAxisMax / 4;

	//左ｱﾅﾛｸﾞｽﾃｨｯｸ(ｽﾃｨｯｸの傾き具合(遊び)の値を500,-500として考える)
	if (js.lX > iAxisPushMin)
	{
		AddInputState(enPKey_LRight);
	}

	else if (js.lX < -iAxisPushMin)
	{
		//左ｷｰ.
		AddInputState(enPKey_LLeft);
	}

	if (js.lY < -iAxisPushMin)
	{
		//上ｷｰ.
		AddInputState(enPKey_LUp);
	}

	else if (js.lY > iAxisPushMin)
	{
		//上ｷｰ.
		AddInputState(enPKey_LDown);
	}

	if (abs(js.lX) > iAxisPushMin)
	{
		m_fHorLSPush = (float)js.lX;
		m_fHorLSPush /= g_iAxisMax;
	}

	if (abs(js.lY) > iAxisPushMin)
	{
		m_fVerLSPush = (float)js.lY;
		m_fVerLSPush /= g_iAxisMax;
	}

	if ((abs(js.lX) + abs(js.lY)) / 2 > iAxisPushMin)
	{
		AddInputState(enPKey_L);
		m_fLSDir = atan2f((float)js.lX, (float)-js.lY);
	}

	if (js.lRx > iAxisPushMin)
	{
		AddInputState(enPKey_RRight);
	}

	else if (js.lRx < -iAxisPushMin)
	{
		//左ｷｰ.
		AddInputState(enPKey_RLeft);
	}

	if (js.lRy < -iAxisPushMin)
	{
		//上ｷｰ.
		AddInputState(enPKey_RUp);
	}

	else if (js.lRy > iAxisPushMin)
	{
		//上ｷｰ.
		AddInputState(enPKey_RDown);
	}

	if (abs(js.lRx) > iAxisPushMin)
	{
		m_fHorRSPush = (float)js.lRx;
		m_fHorRSPush /= g_iAxisMax;
	}

	if (abs(js.lRy) > iAxisPushMin)
	{
		m_fVerRSPush = (float)js.lRy;
		m_fVerRSPush /= g_iAxisMax;
	}

	if ((abs(js.lRx) + abs(js.lRy)) / 2 > iAxisPushMin)
	{
		AddInputState(enPKey_R);
		m_fRSDir = atan2f((float)js.lRx, (float)-js.lRy);
	}

	//ﾎﾞﾀﾝ.
	for (int i = enPKey_00; i < enPKey_Max; i++)
	{
		if (js.rgbButtons[i - enPKey_00] & 0x80)
		{
			AddInputState((enPKey)i);
		}
	}
	
	if (js.lZ < -500)
	{
		AddInputState(enPKey_ZLeft);
	}

	if (js.lZ > 500)
	{
		AddInputState(enPKey_ZRight);
	}

	

	switch (js.rgdwPOV[0])
	{
	case 4500://右上.
		AddInputState(enPKey_LUp);
		AddInputState(enPKey_LRight);
		break;
	case 13500://右下.
		AddInputState(enPKey_LDown);
		AddInputState(enPKey_LRight);
		break;
	case 22500://左上.
		AddInputState(enPKey_LDown);
		AddInputState(enPKey_LLeft);
		break;
	case 31500://左下.
		AddInputState(enPKey_LUp);
		AddInputState(enPKey_LLeft);
		break;

	case 0://上.
		AddInputState(enPKey_LUp);
		break;
	case 9000://右.
		AddInputState(enPKey_LRight);
		break;
	case 18000://下.
		AddInputState(enPKey_LDown);
		break;
	case 27000://左.
		AddInputState(enPKey_LLeft);
		break;

	default:
		break;
	}
		
	return S_OK;
}

//入力情報を追加する関数.
void clsDxInput::AddInputState(enPKey enKey)
{
	//<< ｼﾌﾄ演算子:左にｼﾌﾄ.
	m_uInputState |= 1 << enKey;

}

//入力情報を初期化する関数.
void clsDxInput::InitInputState()
{
	m_uInputState = 0;

	m_fHorLSPush = 0.0f;
	m_fVerLSPush = 0.0f;
	m_fLSDir = 0.0f;

	m_fHorRSPush = 0.0f;
	m_fVerRSPush = 0.0f;
	m_fRSDir = 0.0f;
}

//入力ﾁｪｯｸする関数.
// ※UpdateInputState():関数で入力情報が更新されていることが前提.
bool clsDxInput::IsPressKey(enPKey enKey)
{
	// >> ｼﾌﾄ演算子:右にｼﾌﾄ.
	if ((m_uInputState >> enKey) & 1)
	{
		return true;
	}

	return false;
}

float clsDxInput::GetLSDir()
{
	return m_fLSDir;
}

float clsDxInput::GetLSPush()
{
	float fLSPush = abs(m_fHorLSPush) + abs(m_fVerLSPush);

	if (fLSPush >= 1.0f)
	{
		fLSPush = 1.0f;
	}
	return fLSPush;
}

float clsDxInput::GetHorLSPush()
{
	return m_fHorLSPush;
}

float clsDxInput::GetVerLSPush()
{
	return m_fVerLSPush;
}

float clsDxInput::GetRSDir()
{
	return m_fRSDir;
}

float clsDxInput::GetRSPush()
{
	float fRSPush = abs(m_fHorRSPush) + abs(m_fVerRSPush);

	if (fRSPush >= 1.0f)
	{
		fRSPush = 1.0f;
	}

	return fRSPush;
}

float clsDxInput::GetHorRSPush()
{
	return m_fHorRSPush;
}

float clsDxInput::GetVerRSPush()
{
	return m_fVerRSPush;
}