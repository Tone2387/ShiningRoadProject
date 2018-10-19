#include "MenuWindowBase.h"
#include "PtrGroup.h"
#include "UiText.h"












//                箱の拡縮中は操作が出来ないようにする   .












using namespace std;



clsMENU_WINDOW_BASE::clsMENU_WINDOW_BASE(		
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* pParentWindow,
	const D3DXVECTOR2& vSize )
		:clsWINDOW_BOX( pPtrGroup->GetDevice(), pPtrGroup->GetContext() )
		,m_wpFont( pPtrGroup->GetFont() )
		,m_pParentWindow( pParentWindow )
		,m_pNextWindow( nullptr )
		,m_isOperation( true )
		,m_iSelectNum( 0 )
{
	Operation( true );

	const char sCURSOR_PATH[] = "Data\\Image\\StartUp\\Cursor.png";
	const WHSIZE_FLOAT CURSOR_DISP = { 1.0f, 1.0f };
	const float fCURSOR_ALPHA = 0.75f;
	SPRITE_STATE ss;
	ss.Disp = CURSOR_DISP;
	m_upCursor = make_unique< clsSprite2D >();
	m_upCursor->Create( pPtrGroup->GetDevice(), pPtrGroup->GetContext(), sCURSOR_PATH, ss );
	m_upCursor->SetAlpha( fCURSOR_ALPHA );

	//窓を開く.
	const float fINIT_SIZE = 0.5f;
	const float fADD_SIZE_RATE = 1.0f / 8.0f;
	SetSize( fINIT_SIZE );
	SetSizeTarget( { vSize.x, vSize.y, 0.0f } );
	AddChangeData( 
		vSize.x * fADD_SIZE_RATE, vSize.y * fADD_SIZE_RATE, 
		encBEFOR_CHANGE::WIDTH );
}

clsMENU_WINDOW_BASE::~clsMENU_WINDOW_BASE()
{
	SAFE_DELETE( m_pNextWindow );

	Operation( false );
	if( m_pParentWindow ){
		m_pParentWindow->Operation( true );
		m_pParentWindow = nullptr;
	}

	m_wpFont = nullptr;
}



void clsMENU_WINDOW_BASE::Update()
{
	clsWINDOW_BOX::Update();

	//子供を動かす.
	if( m_pNextWindow ){
		m_pNextWindow->Update();
		//子供が閉じたら.
		if( m_pNextWindow->GetSize().x <= 0.0f && m_pNextWindow->GetSize().y <= 0.0f ){
			delete m_pNextWindow;//消されたら操作権が戻ってくる.
			m_pNextWindow = nullptr;
		}
	}

	//操作権がないなら動かせない.
	if( !m_isOperation ){
		return;
	}
	//大きさ変化中は動かせない.
	else if( !isStopChange() ){
		return;
	}

	UpdateProduct();
}

void clsMENU_WINDOW_BASE::Render()
{
	clsWINDOW_BOX::Render();

	//操作する窓だけカーソル表示.
	if( m_isOperation ){
		m_upCursor->Render();
	}

	//文字.
	RenderProduct();

	//子供を描画.
	if( m_pNextWindow ){
		m_pNextWindow->Render();
	}

}


//次の窓を吐き出す( 成功したならtrue ).
bool clsMENU_WINDOW_BASE::CreateNextWindow( 
	clsMENU_WINDOW_BASE** ppOutNextWindow )
{
	//既に作られているなら.
	if( !ppOutNextWindow ){
		delete *ppOutNextWindow;
		*ppOutNextWindow = nullptr;
	}

	//作れたら.
	if( CreateNextWindowProduct( ppOutNextWindow, this ) ){
		Operation( false );
		m_pNextWindow = *ppOutNextWindow;
		return true;
	}

	return false;
}



//このウィンドウを操作するようになるならtrue,離れるならfalse.
void clsMENU_WINDOW_BASE::Operation( const bool isOperation )
{
	m_isOperation = isOperation;

	//操作しない窓は暗くする1.
	if( m_isOperation ){
		const D3DXVECTOR3 vTRUE_COLOR = { 1.0f, 1.0f, 1.0f };
		SetColor( vTRUE_COLOR );
	}
	else{
		const D3DXVECTOR3 vBLACK_COLOR = { 0.375f, 0.375f, 0.375f };
		SetColor( vBLACK_COLOR );
	}
}


//このウィンドウを閉じて親ウィンドウに操作を返す.
void clsMENU_WINDOW_BASE::Close()
{
	SetSizeTarget( { 0.0f, 0.0f, 0.0f } );

	const float CLOSE_SPD_RATE = 4.0f;
	const D3DXVECTOR2 vCLOSE_SPD = { m_vSize.x / CLOSE_SPD_RATE, m_vSize.y / CLOSE_SPD_RATE };
	AddChangeData( 
		vCLOSE_SPD.x, vCLOSE_SPD.y, 
		encBEFOR_CHANGE::WIDTH );
}



void clsMENU_WINDOW_BASE::SetColor( const D3DXVECTOR3& vColor )
{
	clsWINDOW_BOX::SetColor( vColor );
	if( m_wpFont ){
		m_wpFont->SetColor( { vColor.x, vColor.y, vColor.z, 1.0f } );
	}
	for( unsigned int i=0; i<m_vecupUiText.size(); i++ ){
		m_vecupUiText[i]->SetColor( { vColor.x, vColor.y, vColor.z, 1.0f } );
	}
}


