#ifndef SOUND_MANAGER_TAKEOFF_H_
#define SOUND_MANAGER_TAKEOFF_H_

#include "SoundManagerBase.h"

class clsSOUND_MANAGER_TAKEOFF : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_TAKEOFF( const HWND hWnd );
	~clsSOUND_MANAGER_TAKEOFF();

private:

	//�e�p���N���X�̃N���X����m_sSceneName�ɓ����.
	void CreateSceneName() final;

};



#endif//#ifndef SOUND_MANAGER_TAKEOFF_H_