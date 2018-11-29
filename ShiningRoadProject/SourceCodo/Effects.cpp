#include "Effects.h"
#include "Global.h"
#include "OperationString.h"
#include "File.h"

using namespace std;

namespace{

	//エフェクト管理用インスタンス最大数.
	//最大描画スプライト数.(やばいならint型にする)
	const /*int32_t*/ int g_RenderSpriteMax = 1024 * 1;
	const int g_EffectInstanceMax = g_RenderSpriteMax * 1;

	const string sDATA_PATH = "Data\\Effekseer\\Effects.csv";

	const string sFILE_PATH = "Data\\Effekseer\\Effects\\";

	const int uiEFFECT_FILE_NAME_INDEX = 0;//ファイル名が格納されているインデックス.
	//const int uiMAX_PLAY_INDEX = 1;//最大再生数.

	////エフェクトファイルのリスト.
	//const wchar_t g_strFileNameList[ clsEffects::enEFFECTS_MAX ][128] =
	//{
	//	L"Data\\Effekseer\\tex\\asiato.efk",
	//	L"Data\\Effekseer\\tex\\arbia_kougeki.efk",
	//	L"Data\\Effekseer\\tex\\syougekiha.efk",
	//	L"Data\\Effekseer\\tex\\kougeki.efk",
	//
	//	L"Data\\Effekseer\\tex\\Teki_kougeki.efk",
	//	L"Data\\Effekseer\\tex\\yarare.efk",
	//	L"Data\\Effekseer\\tex\\bikkuri.efk",
	//
	//	L"Data\\Effekseer\\tex\\hibana.efk",
	//
	//	L"Data\\Effekseer\\tex\\takara_hasira.efk",
	//	L"Data\\Effekseer\\tex\\takara_nakami.efk",
	//
	//	L"Data\\Effekseer\\tex\\tobira_sunabokori.efk",
	//};

}



clsEffects::clsEffects()
{
//	ZeroMemory( this, sizeof( clsEffects ) );
	m_pManager = nullptr;
	m_pRender = nullptr;

#ifdef EFFECTS_USE_XAUDIO_
	m_pSound = nullptr;
	m_pXA2 = nullptr;
	m_pXA2Master = nullptr;
#endif//#ifdef EFFECTS_USE_XAUDIO_

	for( unsigned int i=0; i<m_vecpEffect.size(); i++ ){
		m_vecpEffect[i] = nullptr;
	}
}

clsEffects::~clsEffects()
{
	StopAll();
	Destroy();
}

//==================================================
//	破棄関数.
//==================================================
HRESULT clsEffects::Destroy()
{
	//エフェクトデータ解放.
	ReleaseData();

	//先にエフェクト管理用インスタンスを破棄.
	assert( m_pManager );
	m_pManager->Destroy();

#ifdef EFFECTS_USE_XAUDIO_
	//次に音再生用インスタンスを破棄.
	assert( m_pSound );
	m_pSound->Destory();
#endif//#ifdef EFFECTS_USE_XAUDIO_

	//描画用インスタンスを破棄.
	assert( m_pRender );
	m_pRender->Destory();

#ifdef EFFECTS_USE_XAUDIO_
	//XAudio2の解放.
	if( m_pXA2Master != nullptr ){
		m_pXA2Master->DestroyVoice();
		m_pXA2Master = nullptr;
	}
	ES_SAFE_RELEASE( m_pXA2 );
#endif//#ifdef EFFECTS_USE_XAUDIO_

	return S_OK;
}

//==================================================
//	データ解放関数.
//==================================================
HRESULT clsEffects::ReleaseData()
{
	//エフェクトの破棄.
	for( unsigned int i=0; i<m_vecpEffect.size(); i++ ){
		ES_SAFE_RELEASE( m_vecpEffect[i] );
	}
	m_vecpEffect.clear();
	m_vecpEffect.shrink_to_fit();

	return S_OK;
}


//==================================================
//	構築関数.
//==================================================
HRESULT clsEffects::Create( ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext )
{
	//初期化.
	if( FAILED( Init( pDevice, pContext ) ) ){
		return E_FAIL;
	}

	//データ読込.
	if( FAILED( LoadData() ) ){
		return E_FAIL;
	}

	return S_OK;
}

//初期化.
HRESULT clsEffects::Init( ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext )
{
#ifdef EFFECTS_USE_XAUDIO_
	//XAudio2の初期化を行う.
	if( FAILED( XAudio2Create( &m_pXA2 ) ) ){
		ERR_MSG( "XAudio2作成失敗", "clsEffects::Create()" );
		return E_FAIL;
	}
	if( FAILED( m_pXA2->CreateMasteringVoice( &m_pXA2Master ) ) ){
		ERR_MSG( "MasteringVoice作成失敗", "clsEffects::Create()" );
		return E_FAIL;
	}
#endif//#ifdef EFFECTS_USE_XAUDIO_

	//描画用インスタンスの生成.
	m_pRender = ::EffekseerRendererDX11::Renderer::Create(
		pDevice, pContext, g_RenderSpriteMax );

	//エフェクト管理用インスタンス.
	m_pManager = ::Effekseer::Manager::Create( g_EffectInstanceMax );

	//描画用インスタンスから描画機能を設定.
	m_pManager->SetSpriteRenderer( m_pRender->CreateSpriteRenderer() );
	m_pManager->SetRibbonRenderer( m_pRender->CreateRibbonRenderer() );
	m_pManager->SetRingRenderer( m_pRender->CreateRingRenderer() );
	m_pManager->SetModelRenderer( m_pRender->CreateModelRenderer() );

	//描画インスタンスからテクスチャの読込機能を設定.
	//独自拡張可能、現在はファイルから読み込んでいる.
	m_pManager->SetTextureLoader( m_pRender->CreateTextureLoader() );
	m_pManager->SetModelLoader( m_pRender->CreateModelLoader() );

#ifdef EFFECTS_USE_XAUDIO_
	//音再生用インスタンスの生成.
	m_pSound = ::EffekseerSound::Sound::Create( m_pXA2, 16, 16 );

	//音再生用インスタンスから再生機能を設定.
	m_pManager->SetSoundPlayer( m_pSound->CreateSoundPlayer() );

	//音再生用インスタンスからサウンドデータの読込機能を設定.
	//独自拡張可能、現在はファイルから読み込んでいる.
	m_pManager->SetSoundLoader( m_pSound->CreateSoundLoader() );
#endif//#ifdef EFFECTS_USE_XAUDIO_

	return S_OK;
}

//==================================================
//	データ読込関数.
//==================================================
HRESULT clsEffects::LoadData()
{
	unique_ptr< clsFILE > upFile = make_unique<clsFILE>();
	upFile->Open( sDATA_PATH );

	string tmpString;

	//文字列操作用.
	clsOPERATION_STRING OprtStr;

	//エフェクトの読込.
	m_vecpEffect.resize( upFile->GetSizeRow() );
	for( unsigned int i=0; i<m_vecpEffect.size(); i++ ){
		//パスを作る.
		tmpString = sFILE_PATH + upFile->GetDataString( i, uiEFFECT_FILE_NAME_INDEX );

		//マルチバイト文字列( char* )からワイドバイト文字列( wchat_t* )を作成.
		const wchar_t *tmpPath = OprtStr.CreateWcharPtrFromCharPtr( tmpString.c_str() );

		//作成.
		m_vecpEffect[i] =
			::Effekseer::Effect::Create( 
				m_pManager, 
				(const EFK_CHAR*)( tmpPath ) );

		//作ったものは消す.
		delete[] tmpPath;

		assert( m_vecpEffect[i] );
//		//エラー.
//		if( m_vecpEffect[i] == nullptr ){
//			char strMsg[128];
//			wsprintf( strMsg, "clsEffects::LoadData()\n%ls",
//				tmpPath );
//
//			ERR_MSG( strMsg, "エフェクトファイル読込失敗" );
//			return E_FAIL;
//		}
	}

	upFile->Close();
	upFile.reset( nullptr );

	return S_OK;
}

//==================================================
//	描画.
//==================================================
void clsEffects::Render( 
	const D3DXMATRIX& mView, const D3DXMATRIX& mProj, const D3DXVECTOR3& vEye ) const
{
	//ビュー行列設定.
	SetViewMatrix( mView );

	//プロジェクション行列設定.
	SetProjectionMatrix( mProj );

	//エフェクトの更新処理.
	m_pManager->Update();

	//-----------------------------
	//	Effekseerレンダリング.
	//-----------------------------
	//エフェクトの描画開始処理を行う.
	m_pRender->BeginRendering();

	//エフェクトの描画を行う.
	m_pManager->Draw();

	//エフェクトの描画終了処理を行う.
	m_pRender->EndRendering();

}

//==================================================
//	ビュー行列設定.
//==================================================
void clsEffects::SetViewMatrix( const D3DXMATRIX& mView ) const
{
	//ビュー(カメラ)行列.
	::Effekseer::Matrix44 tmpEsCamMat;

	//DirectX Matrix → Effekseer Matrix.
	tmpEsCamMat = MatrixDxToEfk( &mView );

	//ビュー行列を設定.
	m_pRender->SetCameraMatrix( tmpEsCamMat );

}

//==================================================
//	プロジェクション行列設定.
//==================================================
void clsEffects::SetProjectionMatrix( const D3DXMATRIX& mProj ) const
{
	//プロジェクション行列.
	::Effekseer::Matrix44 tmpEsPrjMat;

	//DirectX Matrix → Effekseer Matrix.
	tmpEsPrjMat = MatrixDxToEfk( &mProj );

	//プロジェクション行列を設定.
	m_pRender->SetProjectionMatrix( tmpEsPrjMat );

}

//==================================================
//	ベクター変換関数(1)	DirectX Vector3 → Effekseer Vector3.
//==================================================
::Effekseer::Vector3D clsEffects::Vector3DDxToEfk( const D3DXVECTOR3* pSrcVec3Dx ) const
{
	return ::Effekseer::Vector3D(
		pSrcVec3Dx->x,
		pSrcVec3Dx->y,
		pSrcVec3Dx->z );
}

//==================================================
//	ベクター変換関数(2)	Effekseer Matrix → DirectX Matrix.
//==================================================
D3DXVECTOR3 clsEffects::Vector3EfkToDx( const ::Effekseer::Vector3D* pSrcVec3Efk ) const
{
	return D3DXVECTOR3(
		pSrcVec3Efk->X,
		pSrcVec3Efk->Y,
		pSrcVec3Efk->Z );
}

//==================================================
//	行列変換関数(1)	DirectX Matrix → Effekseer Matrix.
//==================================================
::Effekseer::Matrix44 clsEffects::MatrixDxToEfk(
				const D3DXMATRIX* pSrcMatDx ) const
{
	::Effekseer::Matrix44 OutMatEfk;

#if 0

	for( int i=0; i<4; i++ ){
		for( int j=0; j<4; j++ ){
			OutMatEfk.Values[i][j] = pSrcMatDx->m[i][j];
		}
	}

#else

	OutMatEfk.Values[0][0] = pSrcMatDx->_11;
	OutMatEfk.Values[0][1] = pSrcMatDx->_12;
	OutMatEfk.Values[0][2] = pSrcMatDx->_13;
	OutMatEfk.Values[0][3] = pSrcMatDx->_14;

	OutMatEfk.Values[1][0] = pSrcMatDx->_21;
	OutMatEfk.Values[1][1] = pSrcMatDx->_22;
	OutMatEfk.Values[1][2] = pSrcMatDx->_23;
	OutMatEfk.Values[1][3] = pSrcMatDx->_24;

	OutMatEfk.Values[2][0] = pSrcMatDx->_31;
	OutMatEfk.Values[2][1] = pSrcMatDx->_32;
	OutMatEfk.Values[2][2] = pSrcMatDx->_33;
	OutMatEfk.Values[2][3] = pSrcMatDx->_34;

	OutMatEfk.Values[3][0] = pSrcMatDx->_41;
	OutMatEfk.Values[3][1] = pSrcMatDx->_42;
	OutMatEfk.Values[3][2] = pSrcMatDx->_43;
	OutMatEfk.Values[3][3] = pSrcMatDx->_44;

#endif

	return OutMatEfk;
}

//==================================================
//	行列変換関数(2)	Effekseer Matrix → DirectX Matrix.
//==================================================
D3DXMATRIX clsEffects::MatrixEfkToDx(
				const ::Effekseer::Matrix44* pSrcMatEfk ) const
{
	D3DXMATRIX OutMatDx;

#if 0

	for( int i=0; i<4; i++ ){
		for( int j=0; j<4; j++ ){
			OutMatDx.m[i][j] = pSrcMatEfk->Values[i][j];
		}
	}

#else

	OutMatDx._11 = pSrcMatEfk->Values[0][0];
	OutMatDx._12 = pSrcMatEfk->Values[0][1];
	OutMatDx._13 = pSrcMatEfk->Values[0][2];
	OutMatDx._14 = pSrcMatEfk->Values[0][3];
			
	OutMatDx._21 = pSrcMatEfk->Values[1][0];
	OutMatDx._22 = pSrcMatEfk->Values[1][1];
	OutMatDx._23 = pSrcMatEfk->Values[1][2];
	OutMatDx._24 = pSrcMatEfk->Values[1][3];
			
	OutMatDx._31 = pSrcMatEfk->Values[2][0];
	OutMatDx._32 = pSrcMatEfk->Values[2][1];
	OutMatDx._33 = pSrcMatEfk->Values[2][2];
	OutMatDx._34 = pSrcMatEfk->Values[2][3];
			
	OutMatDx._41 = pSrcMatEfk->Values[3][0];
	OutMatDx._42 = pSrcMatEfk->Values[3][1];
	OutMatDx._43 = pSrcMatEfk->Values[3][2];
	OutMatDx._44 = pSrcMatEfk->Values[3][3];

#endif

	return OutMatDx;
}

