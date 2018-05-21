#ifndef MY_MACRO_H_	//インクルードガード.
#define MY_MACRO_H_


#include <assert.h>

//============================================================
//	マクロ.
//============================================================

//エラーメッセージ.
#define ERR_MSG(str,title)	{MessageBox(NULL,str,title,MB_OK);}

//解放.
#define SAFE_RELEASE(x)		{ if(x) {(x)->Release();(x)=nullptr; } }
#define SAFE_DELETE(p)		{ if(p) { delete (p);   (p)=nullptr; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p); (p)=nullptr; } }

//キーボード入力.
//押している間.
#define GETKEY_STAY(x) if (GetAsyncKeyState(x) & 0x8000)
//押したときだけ.
#define GETKEY_DOWN(x) if (GetAsyncKeyState(x) & 0x0001)


//----- Tahara 2018 04/18 11:38 -----.
////変数名を文字列に.
//#define TO_STRING(VariableName) # VariableName



//nullにアサートを出す.
#define ASSERT_IF_NULL(p){ \
	if( p == nullptr ){ \
		assert( p ||! "はnullですよ" "( ||! は気にしないでください)");\
	}\
}
//	{ assert( !( p == nullptr ) ); }//nullの時に引っかかる.

//nullじゃない場合にアサートを出す.
#define ASSERT_IF_NOT_NULL(p){ \
	if( p != nullptr ){ \
		assert( p ||! "はnullじゃないですよ" "( ||! は気にしないでください)");\
	}\
}
//	{ assert( !( p != nullptr ) ); }//nullじゃない時に引っかかる.



//nullチェック.
#define RETURN_IF_NULL(p) \
	{ ASSERT_IF_NULL( p ); return; }//nullならアサート出してリターン.

//nullじゃないかチェック.
#define RETURN_IF_NOT_NULL(p) \
	{ ASSERT_IF_NOT_NULL( p ); return; }//nullじゃないならアサート出してリターン.
//----- end -----.



#endif//#ifndef MY_MACRO_H_