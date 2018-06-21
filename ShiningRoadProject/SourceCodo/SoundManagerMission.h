#pragma once

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

