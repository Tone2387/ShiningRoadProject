#ifndef SOUND_MANAGER_NOISE_H_
#define SOUND_MANAGER_NOISE_H_

#include "SoundManagerBase.h"

//ノイズの効果音を扱う( シーンは関係ない ).
class clsSOUND_MANAGER_NOISE : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_NOISE( const HWND hWnd );
	~clsSOUND_MANAGER_NOISE();

private:

	//各継承クラスのクラス名をm_sSceneNameに入れる.
	void CreateSceneName() final;

};
#endif//#ifndef SOUND_MANAGER_NOISE_H_