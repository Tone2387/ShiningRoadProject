#ifndef MY_MACRO_H_	//インクルードガード.
#define MY_MACRO_H_

//============================================================
//	マクロ.
//============================================================

//エラーメッセージ.
#define ERR_MSG(str,title)	{MessageBox(NULL,str,title,MB_OK);}

//解放.
#define SAFE_RELEASE(p)		{ if(p) {(p)->Release(); /*delete (p)*/; (p)=nullptr; } }
#define SAFE_DELETE(p)		{ if(p) { delete (p);   (p)=nullptr; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p); (p)=nullptr; } }

//キーボード入力.
//押している間.
#define GETKEY_STAY(x) if (GetAsyncKeyState(x) & 0x8000)
//押したときだけ.
#define GETKEY_DOWN(x) if (GetAsyncKeyState(x) & 0x0001)


//----- Tahara 2018 04/18 11:38 -----.
////変数名を文字列に.
#define TO_STRING(VariableName) # VariableName



#endif//#ifndef MY_MACRO_H_