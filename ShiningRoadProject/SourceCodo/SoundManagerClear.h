#ifndef SOUND_MANAGER_CLEAR_H_
#define SOUND_MANAGER_CLEAR_H_

#include "SoundManagerBase.h"

class clsSOUND_MANAGER_CLEAR : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_CLEAR( const HWND hWnd );
	~clsSOUND_MANAGER_CLEAR();

private:
	//�e�p���N���X�̃N���X����m_sSceneName�ɓ����.
	void CreateSceneName() final;

};


#endif//#ifndef SOUND_MANAGER_CLEAR_H_