#ifndef MY_MACRO_H_	//�C���N���[�h�K�[�h.
#define MY_MACRO_H_

//============================================================
//	�}�N��.
//============================================================

//�G���[���b�Z�[�W.
#define ERR_MSG(str,title)	{MessageBox(NULL,str,title,MB_OK);}

//���.
#define SAFE_RELEASE(p)		{ if(p) {(p)->Release(); /*delete (p)*/; (p)=nullptr; } }
#define SAFE_DELETE(p)		{ if(p) { delete (p);   (p)=nullptr; } }
#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p); (p)=nullptr; } }

//�L�[�{�[�h����.
//�����Ă����.
#define GETKEY_STAY(x) if (GetAsyncKeyState(x) & 0x8000)
//�������Ƃ�����.
#define GETKEY_DOWN(x) if (GetAsyncKeyState(x) & 0x0001)


//----- Tahara 2018 04/18 11:38 -----.
////�ϐ����𕶎����.
#define TO_STRING(VariableName) # VariableName



#endif//#ifndef MY_MACRO_H_