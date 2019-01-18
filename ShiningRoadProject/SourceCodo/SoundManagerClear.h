#ifndef SOUND_MANAGER_CLEAR_H_
#define SOUND_MANAGER_CLEAR_H_

#include "SoundManagerBase.h"

class clsSOUND_MANAGER_CLEAR : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_CLEAR( const HWND hWnd );
	~clsSOUND_MANAGER_CLEAR();

private:
	//各継承クラスのクラス名をm_sSceneNameに入れる.
	void CreateSceneName() final;

};


#endif//#ifndef SOUND_MANAGER_CLEAR_H_