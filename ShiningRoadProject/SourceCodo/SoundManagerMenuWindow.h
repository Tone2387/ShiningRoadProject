#ifndef SOUND_MANAGER_MENUWINDOW_H_
#define SOUND_MANAGER_MENUWINDOW_H_

#include "SoundManagerBase.h"



//メニューウィンドウの効果音を扱う( シーンは関係ない ).
//ファクトリの作成リストに追加しない.
class clsSOUND_MANAGER_MENUWINDOW : public clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_MENUWINDOW( const HWND hWnd );
	~clsSOUND_MANAGER_MENUWINDOW();

	enum enSE : int
	{
		enSE_CURSOL = 0,
		enSE_ENTER,
		enSE_EXIT,
		enSE_WIN_APP,
		enSE_WIN_DISAPP,
	};

private:

	//各継承クラスのクラス名をm_sSceneNameに入れる.
	void CreateSceneName() final;

private:

};

#endif//#ifndef SOUND_MANAGER_MENUWINDOW_H_