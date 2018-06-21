#pragma once

#include "SoundManagerBase.h"


class clsSOUND_MANAGER_TITLE : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_TITLE( const HWND hWnd );
	~clsSOUND_MANAGER_TITLE();

private:

	//各継承クラスのクラス名をm_sSceneNameに入れる.
	void CreateSceneName() final;

};

