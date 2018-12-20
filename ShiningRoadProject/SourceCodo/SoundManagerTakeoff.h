#ifndef SOUND_MANAGER_TAKEOFF_H_
#define SOUND_MANAGER_TAKEOFF_H_

#include "SoundManagerBase.h"

class clsSOUND_MANAGER_TAKEOFF : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_TAKEOFF( const HWND hWnd );
	~clsSOUND_MANAGER_TAKEOFF();

private:

	//各継承クラスのクラス名をm_sSceneNameに入れる.
	void CreateSceneName() final;

};



#endif//#ifndef SOUND_MANAGER_TAKEOFF_H_