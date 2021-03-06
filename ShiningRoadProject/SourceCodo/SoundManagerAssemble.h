#ifndef SOUND_MANAGER_ASSEMBLE_H_
#define SOUND_MANAGER_ASSEMBLE_H_

#include "SoundManagerBase.h"


class clsSOUND_MANAGER_ASSEMBLE : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_ASSEMBLE( const HWND hWnd );
	~clsSOUND_MANAGER_ASSEMBLE();

private:

	//各継承クラスのクラス名をm_sSceneNameに入れる.
	void CreateSceneName() final;

};

#endif//#ifndef SOUND_MANAGER_ASSEMBLE_H_