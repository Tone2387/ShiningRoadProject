#ifndef SOUND_MANAGER_MENUWINDOW_H_
#define SOUND_MANAGER_MENUWINDOW_H_

#include "SoundManagerBase.h"



//���j���[�E�B���h�E�̌��ʉ�������( �V�[���͊֌W�Ȃ� ).
//�t�@�N�g���̍쐬���X�g�ɒǉ����Ȃ�.
class clsSOUND_MANAGER_MENUWINDOW : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_MENUWINDOW( const HWND hWnd );
	~clsSOUND_MANAGER_MENUWINDOW();

	enum enSE : int
	{
		enSE_CURSOL = 0,
		enSE_ENTER,
		enSE_EXIT,
		enSE_WIN_APP,
		enSE_WIN_DISAPP,
	};

private:

	//�e�p���N���X�̃N���X����m_sSceneName�ɓ����.
	void CreateSceneName() final;

private:

};

#endif//#ifndef SOUND_MANAGER_MENUWINDOW_H_