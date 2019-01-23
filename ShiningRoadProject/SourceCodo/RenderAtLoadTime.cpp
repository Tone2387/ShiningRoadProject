#include "RenderAtLoadTime.h"

using namespace std;

namespace{
	const D3DXVECTOR3 vGAGE_BOX_SIZE = { 360.0f, 32.0f, 1.0f };
	const D3DXVECTOR3 vINIT_GAGE_BOX_POS = { 
		WND_W - vGAGE_BOX_SIZE.x * 0.5f - vGAGE_BOX_SIZE.y * 2, 
		WND_H - vGAGE_BOX_SIZE.y * 2, 
		0.0f };

	//ゲージ本体の数.
	const int iGAGE_MAX = 16;
	const char* sGAGE_PATH = "Data\\Image\\StartUp\\LodeGage.png";
	const float fGAGE_ALPHA_OFFSET = 1.0f / static_cast<float>( iGAGE_MAX );

	const float fGAGE_BOX_ADD_SIZE_X = 64.0f;
	const float fGAGE_BOX_ADD_SIZE_Y = 32.0f;

	//線の太さ( ゲージを少し細くする ).
	const float fLINE_WIDTH = 1.0f;
	//ゲージは枠の何分割?.
	const float fGAGE_SIZE_RATE = 64.0f;


	const int iRENDER_FRAME = 160280;
}

clsRENDER_AT_LOAD_TIME::clsRENDER_AT_LOAD_TIME(	
	ID3D11Device*			const pDevice,
	ID3D11DeviceContext*	const pContext,
	IDXGISwapChain*			const pSwapChain,
	ID3D11RenderTargetView*	const pBackBuffer_TexRTV,
	ID3D11DepthStencilView*	const pBackBuffer_DSTexDSV )
	:clsRENDER_SUB_THREAD_BASE( pDevice, pContext, pSwapChain, pBackBuffer_TexRTV, pBackBuffer_DSTexDSV )
	,m_bTrigger( false )
	,m_iFrameCnt( iRENDER_FRAME )
{
	if( !m_upGageBox ){
		m_upGageBox = make_unique< clsLINE_BOX >( pDevice, pContext );
		m_upGageBox->SetPos( vINIT_GAGE_BOX_POS );
		m_upGageBox->SetSize( { 0.0f, 0.0f, 0.0f } );
		m_upGageBox->SetSizeTarget( vGAGE_BOX_SIZE );
		m_upGageBox->AddChangeData( fGAGE_BOX_ADD_SIZE_X, fGAGE_BOX_ADD_SIZE_Y, clsLINE_BOX::encBEFOR_CHANGE::WIDTH );
	}

	SPRITE_STATE ss;
	const WHSIZE_FLOAT DISP= { 1.0f, 1.0f };
	ss.Disp = DISP;

	m_vecupGage.resize( iGAGE_MAX );
	for( unsigned int i=0; i<m_vecupGage.size(); i++ ){
		m_vecupGage[i] = make_unique< clsSPRITE2D_CENTER >();
		m_vecupGage[i]->Create( pDevice, pContext, sGAGE_PATH, ss );

		m_vecupGage[i]->SetScale( 0.0f );

		float tmp = 1.0f - ( fGAGE_ALPHA_OFFSET * static_cast<float>( i ) );
		m_vecupGage[i]->SetAlpha( tmp );
	}
}

clsRENDER_AT_LOAD_TIME::~clsRENDER_AT_LOAD_TIME()
{
}

//ループ処理.
void clsRENDER_AT_LOAD_TIME::Loop()
{
	while( !isEnd() )
	{
		if( m_iFrameCnt >= iRENDER_FRAME ){
			m_iFrameCnt = 0;
			Update();
		}
		else{
			m_iFrameCnt++;
		}
	}
}


void clsRENDER_AT_LOAD_TIME::FinishLoad()
{
	End();
}

	
void clsRENDER_AT_LOAD_TIME::UpdateProduct()
{
	if( !m_upGageBox ){ 
		return;
	}
	m_upGageBox->Update(); 

	//変化が終わればここを通す.
	if( m_upGageBox->isStopChange() ){
		if( !m_bTrigger ){
			//ゲージの初期化.
			for( unsigned int i=0; i<m_vecupGage.size(); i++ ){
				if( !m_vecupGage[i] ){ 
					continue; 
				}
				m_vecupGage[i]->SetScale( {
					m_upGageBox->GetSize().x / fGAGE_SIZE_RATE - fLINE_WIDTH, m_upGageBox->GetSize().y, 0.0f } );
				m_vecupGage[i]->SetPos( {
					WND_W - ( m_vecupGage[i]->GetScale().x * static_cast<float>( i ) ), 
					m_upGageBox->GetPos().y, 
					0.0f } );
			}
		}
		m_bTrigger = true;
	}
	else{
		return;
	}

	for( unsigned int i=0; i<m_vecupGage.size(); i++ ){
		if( !m_vecupGage[i] ){ 
			continue; 
		}
		m_vecupGage[i]->AddPos( { m_vecupGage[i]->GetScale().x*0.25f, 0.0f, 0.0f } );
		if( m_vecupGage[i]->GetPos().x > WND_W ){
			m_vecupGage[i]->SetPos( { 
				m_upGageBox->GetPos().x - ( m_upGageBox->GetSize().x * 0.5f ) + fLINE_WIDTH, 
				m_upGageBox->GetPos().y,
				0.0f } );
		}
	}

}

void clsRENDER_AT_LOAD_TIME::RenderProduct() const
{
	if( !m_upGageBox ){ 
		return; 
	}

	for( unsigned int i=0; i<m_vecupGage.size(); i++ ){
		if( !m_vecupGage[i] ){ 
			continue; 
		}
		//枠の中だけ描画する.
		if( m_vecupGage[i]->GetPos().x < m_upGageBox->GetPos().x - ( m_upGageBox->GetSize().x * 0.5f ) ||
			m_vecupGage[i]->GetPos().x > m_upGageBox->GetPos().x + ( m_upGageBox->GetSize().x * 0.5f ) )
		{
			continue;
		}
		m_vecupGage[i]->Render();
	}

	m_upGageBox->Render();
}

