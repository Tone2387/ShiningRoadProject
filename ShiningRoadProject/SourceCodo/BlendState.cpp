#include "BlendState.h"


clsBLEND_STATE::clsBLEND_STATE(
	ID3D11Device* const pDevice,
	ID3D11DeviceContext* const pContext )
	:m_pContext( pContext )
	,m_isAlpha( false )
{
	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		m_pBlendState[i] = nullptr;
	}

	if( FAILED( CreateBlendState( pDevice ) ) ){
		assert( !"Can't Create Blend State" );
	}

	SetBlend( !m_isAlpha );
}

clsBLEND_STATE::~clsBLEND_STATE()
{
	m_isAlpha = false;

	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		SAFE_RELEASE( m_pBlendState[i] );
	}

	m_pContext = nullptr;
}


//ブレンドステート作成.
HRESULT clsBLEND_STATE::CreateBlendState( ID3D11Device* const pDevice )
{
	assert( pDevice );

	//アルファブレンド用ブレンドステート作成.
	//pngファイル内にアルファ情報があるので、透過するようにブレンドステートで設定する.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( D3D11_BLEND_DESC ) );	//初期化.
	blendDesc.IndependentBlendEnable = false;			//false:RenderTarget[0]のメンバーのみ使用する。true:RenderTarget[0～7]が使用できる(レンダーターゲット毎に独立したブレンド処理).
	blendDesc.AlphaToCoverageEnable = false;			//true:アルファトゥカバレッジを使用する.

	//表示タイプ
//	blendDesc.RenderTarget[0].BlendEnable = true;					//true:アルファブレンドを使用する.
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		//アルファブレンドを指定.
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//アルファブレンドの反転を指定.
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;			//ADD：加算合成.
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		//そのまま使用.
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		//何もしない.
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	//ADD：加算合成.
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//全ての成分(RGBA)へのデータの格納を許可する.

	bool tmpBlendEnable[ enBLEND_STATE_size ];
	tmpBlendEnable[ enBLEND_STATE_ALPHA_ON ] = true;
	tmpBlendEnable[ enBLEND_STATE_ALPHA_OFF ] = false;

	for( unsigned char i=0; i<enBLEND_STATE_size; i++ )
	{
		assert( !m_pBlendState[i] );
		blendDesc.RenderTarget[0].BlendEnable = tmpBlendEnable[i];
		if( FAILED( pDevice->CreateBlendState( &blendDesc, &m_pBlendState[i] ) ) ){
			assert( !"ブレンドステートの作成に失敗" );
			return E_FAIL;
		}
	}

	return S_OK;
}


void clsBLEND_STATE::SetBlend( const bool isAlpha )
{
	//今と同じならわざわざやらんでよい.
	if( isAlpha == m_isAlpha ){
		return;
	}

	const UINT mask = 0xffffffff;	//マスク値白.

	assert( m_pContext );
	//ブレンドステートの設定.
	if( isAlpha ){		
		m_pContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_ON ], NULL, mask );
	}
	else{
		m_pContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_OFF ], NULL, mask );
	}
}

