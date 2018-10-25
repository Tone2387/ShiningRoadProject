#ifndef BLACK_SCREEN_H_
#define BLACK_SCREEN_H_


#include "Sprite2D.h"

const float fBLACK_SCREEN_DEFAULT_SPD = 1.0f / ( 60.0f * 2.0f ) * 2.0f;


class clsBLACK_SCREEN : public clsSprite2D
{
public:
	clsBLACK_SCREEN();
	~clsBLACK_SCREEN();

	//暗明転の実行( 毎フレーム ).
	void Update();

	//0.0f ～ 1.0fで与えてね( はみ出ても、わざとなら問題はないが ).
	void SetChangeSpd( const float fSpd );

	//----- 暗明転の指示 -----.
	//暗転指示.//Alpha Per Frame.
	void GetDark();
	//明転指示.//Alpha Per Frame.
	void GetBright();


	//暗転が完了するとtrueを返す( その瞬間だけ ).
	bool isDarkEnd();
	//明転が完了するとtrueを返す.
	bool isBrightEnd();


private:

	//フラグ用列挙体.
	enum enFLG_GROUP : UCHAR
	{
		DARK_END = 0,//暗転が終わった瞬間.
		DARK_ROUTE	,//暗転中.
		BRIGHT_END	,//明転が終わった瞬間.
		BRIGHT_ROUTE,//明転中.
	};

	//暗転中ですか?、のフラグ.
	enum class enSTATE : UCHAR
	{
		NEUTRAL = 0,//何もしていない.
		DARK,		//暗転.
		BRIGHT		//明転.
	}	m_enState;


	//ビットフラグ更新.
	void UpdateBitFlg( const enFLG_GROUP enFlg, const bool bStand );
	//フラグチェック.
	bool isBitFlg( const enFLG_GROUP enFlg ) const;

	//暗明転速度.
	float m_fChangeSpd;

	//各種フラグまとめ.
	unsigned int m_uiFlgGroup;

};

#endif//#ifndef BLACK_SCREEN_H_