#ifndef SOUND_MANAGER_GAME_OVER_H_
#define SOUND_MANAGER_GAME_OVER_H_

#include "SoundManagerBase.h"


class clsSOUND_MANAGER_GAME_OVER : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_GAME_OVER( const HWND hWnd );
	~clsSOUND_MANAGER_GAME_OVER();

private:

	//各継承クラスのクラス名をm_sSceneNameに入れる.
	void CreateSceneName() final;

};

#endif//#ifndef SOUND_MANAGER_GAME_OVER_H_