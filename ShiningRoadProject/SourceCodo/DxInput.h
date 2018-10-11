#ifndef _DINPUT_H_
#define _DINPUT_H_

#include <dinput.h>	// DirectInput用.

// DirectX 関係.
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "dinput8.lib" )	// コントローラ操作で使用.

//---------------
//グローバル変数
//---------------
const int g_iAxisMax = 1000;
const int g_iAxisMin = g_iAxisMax / 4;

const float g_fPushMax = 1.0f;
const float g_fPushMin = 0.6f;
const float g_fNoPush = 0.0f;

//========================================================
//	列挙体宣言.
//========================================================
//ｷｰ情報.
enum enPKey
{
	enPKey_L,

	enPKey_LUp,
	enPKey_LDown,
	enPKey_LLeft,
	enPKey_LRight,

	enPKey_R,

	enPKey_RUp,
	enPKey_RDown,
	enPKey_RLeft,
	enPKey_RRight,

	enPKey_ZLeft,
	enPKey_ZRight,

	enPKey_00,
	enPKey_01,
	enPKey_02,
	enPKey_03,
	enPKey_04,
	enPKey_05,
	enPKey_06,
	enPKey_07,
	enPKey_08,
	enPKey_09,
	enPKey_10,
	enPKey_11,

	enPKey_Max,//ｷｰ最大数.
};

//========================================================
//	ﾒｲﾝｸﾗｽ.
//========================================================

class clsDxInput
{
public:
	clsDxInput();
	~clsDxInput();

	bool initDI(HWND hWnd);

	//入力情報更新関数.
	HRESULT UpdataInputState();

	//入力情報を追加する関数.
	void AddInputState(enPKey enKey);

	//入力情報を初期化する関数.
	void InitInputState();

	//入力ﾁｪｯｸする関数.
	bool IsPressKey(enPKey enKey);

	float GetLSDir();

	float GetLSPush();
	float GetHorLSPush();
	float GetVerLSPush();

	const bool IsLSUpEnter();
	const bool IsLSDownEnter();
	const bool IsLSLeftEnter();
	const bool IsLSRightEnter();

	const bool IsLSUpStay();
	const bool IsLSDownStay();
	const bool IsLSLeftStay();
	const bool IsLSRightStay();

	float GetRSDir();

	float GetRSPush();
	float GetHorRSPush();
	float GetVerRSPush();

	const bool IsRSUpEnter();
	const bool IsRSDownEnter();
	const bool IsRSLeftEnter();
	const bool IsRSRightEnter();
				 
	const bool IsRSUpStay();
	const bool IsRSDownStay();
	const bool IsRSLeftStay();
	const bool IsRSRightStay();

private:
	struct InputState
	{
		//左スティック.
		float fLSDir;
		float fHorLSPush;
		float fVerLSPush;

		//右スティック.
		float fRSDir;
		float fHorRSPush;
		float fVerRSPush;

		unsigned int uiKeyState;//入力情報.
	};

	InputState m_InputNowState;//現在の入力情報.
	InputState m_InputOldState;//1フレーム前の入力情報.

	LPDIRECTINPUT8 m_DI;//DxInputｵﾌﾞｼﾞｪｸﾄ.
	LPDIRECTINPUTDEVICE8 m_Pad;//ﾃﾞﾊﾞｲｽ(ｺﾝﾄﾛｰﾗ)ｵﾌﾞｼﾞｪｸﾄ.
};

#endif //#ifndef _DINPUT_H_

