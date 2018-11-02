#ifndef PLAYER_ROBO_STATUS_H_
#define PLAYER_ROBO_STATUS_H_

#include "RoboStatusBase.h"

class clsROBO_STATUS_PLAYER : public clsROBO_STATUS
{
public:
	clsROBO_STATUS_PLAYER();
	~clsROBO_STATUS_PLAYER();


	//クリア画面で使う : タイトル用の初期化用のデータを用意する : クリアしたロボを覚えておく.
	void SaveHeroData();

	//タイトル画面での初期化でAssembleModelのnewの前に使う.
	void LodeHeroData();

private:

	//外部からデータを読み込む.
	void LoadFileData( const char* sFilePath ) override;

//	//クリアデータ( タイトルで必要 ).
	ROBO_STATE_DATA m_RoboStateDataHero;

};




#endif//#ifndef PLAYER_ROBO_STATUS_H_