#ifndef SOUND_MANAGER_NOISE_H_
#define SOUND_MANAGER_NOISE_H_

#include "SoundManagerBase.h"

//�m�C�Y�̌��ʉ�������( �V�[���͊֌W�Ȃ� ).
class clsSOUND_MANAGER_NOISE : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_NOISE( const HWND hWnd );
	~clsSOUND_MANAGER_NOISE();

private:

	//�e�p���N���X�̃N���X����m_sSceneName�ɓ����.
	void CreateSceneName() final;

};
#endif//#ifndef SOUND_MANAGER_NOISE_H_