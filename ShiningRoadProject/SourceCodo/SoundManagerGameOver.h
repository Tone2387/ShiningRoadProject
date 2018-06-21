#pragma once

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

