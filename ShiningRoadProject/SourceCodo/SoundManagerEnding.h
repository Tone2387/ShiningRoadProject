#pragma once

#include "SoundManagerBase.h"


class clsSOUND_MANAGER_ENDING : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_ENDING( const HWND hWnd );
	~clsSOUND_MANAGER_ENDING();

private:

	//各継承クラスのクラス名をm_sSceneNameに入れる.
	void CreateSceneName() final;

};

