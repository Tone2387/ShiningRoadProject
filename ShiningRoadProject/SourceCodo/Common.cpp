#include "Common.h"

clsCommon::clsCommon()
{
	m_pDevice = nullptr;
	m_pContext = nullptr;
	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		m_pBlendState[i] = nullptr;
	}

//	if( FAILED( CreateBlendState() ) ){
//		assert( !"Can't Create Blend State" );
//	}

};
clsCommon::~clsCommon()
{
	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		SAFE_RELEASE( m_pBlendState[i] );
	}

	//ここでは開放しない.
	m_pContext = nullptr;
	m_pDevice = nullptr;
};

//ブレンドステート作成.
HRESULT clsCommon::CreateBlendState()
{
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
		blendDesc.RenderTarget[0].BlendEnable = tmpBlendEnable[i];
		if( FAILED( m_pDevice->CreateBlendState( &blendDesc, &m_pBlendState[i] ) ) ){
			assert( !"ブレンドステートの作成に失敗" );
			return E_FAIL;
		}
	}

	return S_OK;
}

void clsCommon::SetBlend( const bool isAlpha )
{
	UINT mask = 0xffffffff;	//マスク値白.

	if( isAlpha ){		
		//ブレンドステートの設定.
		m_pContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_ON ], NULL, mask );
	}
	else{
		m_pContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_OFF ], NULL, mask );
	}
}

