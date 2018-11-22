#include "BlendState.h"






clsBLEND_STATE::clsBLEND_STATE(
	ID3D11Device* const pDevice,
	ID3D11DeviceContext* const pContext )
	:m_wpDevice( pDevice )
	,m_wpContext( pContext )
	,m_encBrendFlg( encBLEND_FLAG::NOT_ALPHA )
{
	//初期化と別のものを置かないと作らない.
	SetBlend( encBLEND_FLAG::ALPHA );
}

clsBLEND_STATE::~clsBLEND_STATE()
{
	m_wpDevice = nullptr;
	m_wpContext = nullptr;
}



void clsBLEND_STATE::SetBlend( const encBLEND_FLAG encBlendFlag )
{
	//今と同じならわざわざやらんでよい.
	if( m_encBrendFlg == encBlendFlag ){
		return;
	}
	m_encBrendFlg = encBlendFlag;

	ID3D11BlendState* pBlendState = nullptr;
	if( FAILED( CreateBlendState( &pBlendState ) ) ){
		assert( !"Can't Create Blend State" );
	}

	//ブレンドステートの設定.
	const UINT mask = 0xffffffff;	//マスク値白.
	m_wpContext->OMSetBlendState( pBlendState, NULL, mask );

	SAFE_DELETE( pBlendState );
}

//ブレンドステート作成.
HRESULT clsBLEND_STATE::CreateBlendState( ID3D11BlendState** ppBlendState )
{
	D3D11_BLEND_DESC blendDesc;
	CreateBlendDesc( &blendDesc );

	//アルファブレンド用ブレンドステート作成.
	//pngファイル内にアルファ情報があるので、透過するようにブレンドステートで設定する.
	if( FAILED( m_wpDevice->CreateBlendState( &blendDesc, ppBlendState ) ) ){
		assert( !"ブレンドステートの作成に失敗" );
		return E_FAIL;
	}

	return S_OK;
}

//ブレンドデスク作成.
D3D11_BLEND_DESC clsBLEND_STATE::CreateBlendDesc( D3D11_BLEND_DESC* const pBlendDesc )
{
	assert( pBlendDesc );

	ZeroMemory( pBlendDesc, sizeof( D3D11_BLEND_DESC ) );	//初期化.
	pBlendDesc->IndependentBlendEnable	= false;			//false:RenderTarget[0]のメンバーのみ使用する。true:RenderTarget[0～7]が使用できる(レンダーターゲット毎に独立したブレンド処理).
	pBlendDesc->AlphaToCoverageEnable	= false;			//true:アルファトゥカバレッジを使用する.

	//表示タイプ
	pBlendDesc->RenderTarget[0].BlendEnable				= true;							//true:アルファブレンドを使用する.
	pBlendDesc->RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;		//アルファブレンドを指定.
	pBlendDesc->RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;	//アルファブレンドの反転を指定.
	pBlendDesc->RenderTarget[0].BlendOp					= D3D11_BLEND_OP_ADD;			//ADD：加算合成.
	pBlendDesc->RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;				//そのまま使用.
	pBlendDesc->RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;				//何もしない.
	pBlendDesc->RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;			//ADD：加算合成.
	pBlendDesc->RenderTarget[0].RenderTargetWriteMask	= D3D11_COLOR_WRITE_ENABLE_ALL;	//全ての成分(RGBA)へのデータの格納を許可する.


	switch( m_encBrendFlg )
	{
	case encBLEND_FLAG::ALPHA:
		pBlendDesc->RenderTarget[0].BlendEnable = static_cast<BOOL>( true );
		break;
	case encBLEND_FLAG::NOT_ALPHA:
		pBlendDesc->RenderTarget[0].BlendEnable = static_cast<BOOL>( false );
		break;
	default:
		break;
	}

	return *pBlendDesc;
}