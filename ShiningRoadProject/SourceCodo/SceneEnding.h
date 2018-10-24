#ifndef SCENE_ENDING_H_
#define SCENE_ENDING_H_


#include "SceneBase.h"
#include "CameraEnding.h"

#include "Stage.h"


//================================//
//========== エンディングクラス ==========//
//================================//
class clsSCENE_ENDING : public clsSCENE_BASE
{
public:
	clsSCENE_ENDING( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_ENDING();


private:
	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//「 UIの 」Render.

	struct TEXT_STATE
	{
		D3DXVECTOR3 vPos;
		float		fScale;
		float		fAlpha;
	};

	//範囲をoverするとfalseが返ってくる.
	bool AddAlphaState( TEXT_STATE* const pTextState, const float fAlpha );

	//そのばで透過する文字用のステータス.
	std::vector< std::unique_ptr< TEXT_STATE > > m_vupTextStateAlpha;
	//スクロールする文字用のステータス.
	std::vector< std::unique_ptr< TEXT_STATE > > m_vupTextStateScroll;

	unsigned int	m_uiSpriteCnt;
	bool			m_isSpriteAlphaAppear;
	int				m_iIntervalCnt;//始まるまでの時間カウント.
	bool			m_isScroll;

	//最後に表示する添え字.
	int m_iGoScrollIndex;

	//スクロールが終わったら決定ボタンでタイトルに戻る許可を出す.
	bool m_isCanGoTitle;

	//スタッフロール透過文字の同時表示数.
	unsigned int m_uiRenderTextNum;

	std::unique_ptr< clsStage >	m_upStage;//テスト用.

#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG

	//音の引数.
	enum enBGM : int
	{
		enBGM_MAFIA0 = 0,
		enBGM_MAFIA1,
		enBGM_MAOU2,
		enBGM_MUS0,
		enBGM_POKET0,
		enBGM_POKET1,
		enBGM_RENGOKU0
	};

	enum enSE : int
	{
		enSE_CURSOL_MOVE = 0,
		enSE_ENTER,
		enSE_EXIT,
	};

};
#endif//#ifndef SCENE_ENDING_H_