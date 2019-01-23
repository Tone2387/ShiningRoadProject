#include "SceneMovieBase.h"
#include "File.h"


clsSCENE_MOVIE_BASE::clsSCENE_MOVIE_BASE( 
	clsPOINTER_GROUP* const ptrGroup,
	const enSCENE enScene,
	const char* sCutFrameFilePath  )
	:clsSCENE_BASE( ptrGroup )
	,m_enNEXT_SCENE( enScene )
	,m_iCut( -1 )//初期化時に++されるのでその時に0になるように.
	,m_fMovieFrame( 0.0f )
	,m_isTrigger( false )
{
	clsFILE File;
	assert( File.Open( sCutFrameFilePath ) );

	//次のシーン( 配列の最後の添え字になる数 ).
	m_iCutEnd = File.GetSizeRow() - 1;

	//フレーム数のセット.
	m_vecfMovieFrameNext.resize( File.GetSizeRow() );
	for( unsigned int i=0; i<m_vecfMovieFrameNext.size(); i++ ){
		m_vecfMovieFrameNext[i] = File.GetDataInt( i, 0 );
	}
}

clsSCENE_MOVIE_BASE::~clsSCENE_MOVIE_BASE()
{
}

void clsSCENE_MOVIE_BASE::CreateProduct()
{
	InitMovie();
}


void clsSCENE_MOVIE_BASE::UpdateProduct( enSCENE &enNextScene )
{
#ifndef CAMERA_FREE_MOVE_
//	if( m_enCut != enCUT_END ){ m_fMovieFrame ++; }
	m_fMovieFrame ++;
#endif//CAMERA_FREE_MOVE_

	if( isPressEnter() ){
#ifdef _DEBUG
		//フレーム数の変数を次のカットへ飛ぶ数まで増やす.
		NextCut();
#else//#ifdef _DEBUG
		//ムービーをスキップし次のシーンへ.
		enNextScene = m_enNEXT_SCENE;
#endif//#ifdef _DEBUG
	}

	//今のカットのフレーム数を満たしたら.
	assert( m_iCut < static_cast<int>( m_vecfMovieFrameNext.size() ) );
	if( m_fMovieFrame > m_vecfMovieFrameNext[ m_iCut ] ){
		//次のシーンへ.
		if( m_iCut == m_iCutEnd ){
			enNextScene = m_enNEXT_SCENE;
		}
		//次のカットへ.
		else{
			InitMovie();
		}
	}

	UpdateMovie();
}


void clsSCENE_MOVIE_BASE::InitMovie()
{
	++ m_iCut;
	m_fMovieFrame = 0.0f;
	m_isTrigger = false;

	InitMovieProduct();
}


void clsSCENE_MOVIE_BASE::UpdateMovie()
{
	int iOtherDataIndex = 0;

	UpdateMovieProduct( iOtherDataIndex );

}

//フレームが満たしていなくても次のカットへ飛ぶ.
void clsSCENE_MOVIE_BASE::NextCut()
{
	//配列のサイズチェック.
	if( m_iCut <= m_iCutEnd ){
		//カウント変数を次に進むまで増やす.
		m_fMovieFrame = m_vecfMovieFrameNext[ m_iCut ] + 1.0f;
	}
}
