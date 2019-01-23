#ifndef SCENE_MOVIE_BASE_H_
#define SCENE_MOVIE_BASE_H_

#include "SceneBase.h"



//ムービーシーンの基底クラス.
class clsSCENE_MOVIE_BASE : public clsSCENE_BASE
{
public:
	clsSCENE_MOVIE_BASE( 
		clsPOINTER_GROUP* const ptrGroup,
		const enSCENE enScene,
		const char* sCutFrameFilePath );
	~clsSCENE_MOVIE_BASE();

protected:
	//残りの二つは継承先で作る.
	void CreateProduct() override;//継承先のこの関数の最後でこのクラスのこの関数を使うこと.
//	void RenderProduct( const D3DXVECTOR3 &vCamPos ) override;
//	void RenderUi() override;//「 UIの 」Render.

	virtual void InitMovieProduct() = 0;
	virtual void UpdateMovieProduct( int iOtherDataIndex ) = 0;
	//フレームが満たしていなくても次のカットへ飛ぶ.
	void NextCut();

protected:
	//カメラの移動速度など.
	std::vector< float > m_vecfOtherData;
	//今のカットのフレームカウント.
	float m_fMovieFrame;
	//各カット一回きりのフラグトリガー.
	bool m_isTrigger;
	//カット割り.
	int m_iCut;

private:
	void UpdateProduct( enSCENE &enNextScene ) final;
	//ムービーっぽく動かすための為の関数.
	void InitMovie();
	void UpdateMovie();

private:
	//最後のカット番号.
	int m_iCutEnd;
	//次のシーン.
	const enSCENE m_enNEXT_SCENE;

	//全カットのフレーム数.
	std::vector< float > m_vecfMovieFrameNext;

};


#endif//#ifndef SCENE_MOVIE_BASE_H_