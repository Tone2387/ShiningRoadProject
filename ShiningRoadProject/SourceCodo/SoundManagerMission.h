#ifndef SOUND_MANAGER_MISSION_H_
#define SOUND_MANAGER_MISSION_H_

#include "SoundManagerBase.h"


class clsSOUND_MANAGER_MISSION : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_MISSION( const HWND hWnd );
	~clsSOUND_MANAGER_MISSION();

private:

	//各継承クラスのクラス名をm_sSceneNameに入れる.
	void CreateSceneName() final;

};

#endif//#ifndef SOUND_MANAGER_MISSION_H_