#include "MenuWindowBase.h"


#include "PtrGroup.h"
#include "SoundManagerMenuWindow.h"
#include "CFont.h"
#include "UiText.h"
#include "DxInput.h"
#include "CXInput.h"

#include "File.h"


using namespace std;
#define DINPUT_ENTER enPKey_00
#define DINPUT_EXIT	 enPKey_03

namespace{
	//ボタンのホールドフレーム.
	const int iHOLD_FREAM = 30;
	const int iHOLD_FREAM_FIRST = 6;
	const int iHOLD_FREAM_FIRST_STEP = 1;
}

clsMENU_WINDOW_BASE::clsMENU_WINDOW_BASE(		
	const HWND						hWnd,
	clsPOINTER_GROUP* const			pPtrGroup,
	clsMENU_WINDOW_BASE* const		pParentWindow,
	std::vector<unsigned int>* const pInformationVec,
	const D3DXVECTOR2&				vSize,
	const char*						sWindowName )
		:clsWINDOW_BOX( pPtrGroup->GetDevice(), pPtrGroup->GetContext() )
		,m_hWnd( hWnd )
		,m_pPtrGroup( pPtrGroup )
		,m_pInformationVec( pInformationVec )
		,m_wpFont( pPtrGroup->GetFont() )
		,m_wpXInput( pPtrGroup->GetXInput() )
		,m_wpDInput( pPtrGroup->GetDxInput() )
		,m_pParentWindow( pParentWindow )
		,m_pNextWindow( nullptr )
		,m_isOperation( true )
		,m_iSelectNum( 0 )
		,m_uiInformation( 0 )
		,m_isClose( false )
{
	Operation( true );
	if( pParentWindow ){
		pParentWindow->Operation( false );
	}

	//カーソル画像作成.
	const char sCURSOR_PATH[] = "Data\\Image\\MenuWindow\\Cursor.png";
	const WHSIZE_FLOAT CURSOR_DISP = { 1.0f, 1.0f };
	const float fCURSOR_ALPHA = 0.75f;
	SPRITE_STATE ss;
	ss.Disp = CURSOR_DISP;
	m_upCursor = make_unique< clsSprite2D >();
	m_upCursor->Create( pPtrGroup->GetDevice(), pPtrGroup->GetContext(), sCURSOR_PATH, ss );
	m_upCursor->SetAlpha( fCURSOR_ALPHA );


	m_upSound = make_unique< clsSOUND_MANAGER_MENUWINDOW >( m_hWnd );
	m_upSound->Create( sWindowName );
	m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_WIN_APP );

	//窓を開く.
	Open( vSize );
}


clsMENU_WINDOW_BASE::~clsMENU_WINDOW_BASE()
{
	SAFE_DELETE( m_pNextWindow );

	Operation( false );
	m_pParentWindow = nullptr;

	m_wpDInput = nullptr;
	m_wpXInput = nullptr;
	m_wpFont = nullptr;
	m_pInformationVec = nullptr;
	m_pPtrGroup = nullptr;
}

clsMENU_WINDOW_BASE::INFORMATION_MENU_DATA_ARRAY clsMENU_WINDOW_BASE::CreateInformationProduct( 
	INFORMATION_MENU_DATA_ARRAY* const InformationDataArray,
	const int iInformationSize,
	const char* sInformationDataPath )
{
	//照合用情報の作成の為のファイルデータ取得.
	unique_ptr< clsFILE > upFile = make_unique< clsFILE >();
	upFile->Open( sInformationDataPath );
	//照合用情報の作成.
	InformationDataArray->resize( iInformationSize );
	for( char i=0; i<iInformationSize; i++ ){
		const int iCOL = 0;
		assert( static_cast<unsigned int>( i ) < upFile->GetSizeRow() );
		( *InformationDataArray )[i] = static_cast<unsigned int>( upFile->GetDataInt( i, iCOL ) );
	}

	return *InformationDataArray;
}




//このメニューウィンドウのdeleteはこのif文の中で使いましょう.
bool clsMENU_WINDOW_BASE::isDeletePermission()
{
	//ウィンドウが縮み切っている.
	if( isMinimum() ){
		//音も鳴り終わっている.
		if( m_upSound->IsStoppedSE( clsSOUND_MANAGER_MENUWINDOW::enSE_WIN_DISAPP ) ){
			return true;
		}
	}

	return false;
}

//最小か( 消す許可の出るサイズか ).
bool clsMENU_WINDOW_BASE::isMinimum()
{
	const float fDELETE_SIZE = 0.1f;
	if( m_vSize.x <= fDELETE_SIZE && m_vSize.y <= fDELETE_SIZE ){
		return true;
	}

	return false;
}

//継承先のコンストラクタで使う.
void clsMENU_WINDOW_BASE::Open( const D3DXVECTOR2& vSize )
{
	const float fINIT_SIZE = 0.5f;
	const float fADD_SIZE_RATE = 1.0f / 8.0f;
	SetSize( fINIT_SIZE );
	SetSizeTarget( { vSize.x, vSize.y, 0.0f } );
	AddChangeData( 
		vSize.x * fADD_SIZE_RATE, vSize.y * fADD_SIZE_RATE, 
		encBEFOR_CHANGE::WIDTH );
}



void clsMENU_WINDOW_BASE::Update()
{
	clsWINDOW_BOX::Update();

	//子供を動かす.
	if( m_pNextWindow ){
		m_pNextWindow->Update();
		//子供が閉じたら.
//		if( m_pNextWindow->GetSize().x <= 0.0f && m_pNextWindow->GetSize().y <= 0.0f ){
		if( m_pNextWindow->isDeletePermission() ){
			delete m_pNextWindow;
			m_pNextWindow = nullptr;
			//閉じる予約をしていたら自分も閉じる.
			if( m_isClose ){
				Close();
			}
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
	if( m_isOperation && isStopChange() ){
		m_upCursor->Render();
	}

	//操作しない窓は暗くする.
	if( m_isOperation ){
		const D3DXVECTOR3 vTRUE_COLOR = { 1.0f, 1.0f, 1.0f };
		SetColor( vTRUE_COLOR );
	}
	else{
		const D3DXVECTOR3 vBLACK_COLOR = { 0.375f, 0.375f, 0.375f };
		SetColor( vBLACK_COLOR );
	}
	const float fTEXT_ALPHA = 1.0f;
	SetTextAlpha( fTEXT_ALPHA );

	//文字とカーソル.
	//変化中ではないこと.
	if( isStopChange() ){
		//大きい時( 小さくないとき ).
		if( !isMinimum() ){
			//継承先で定義.
			RenderProduct();
		}
	}

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




//このウィンドウを閉じ始めて親ウィンドウに操作を返す.
void clsMENU_WINDOW_BASE::Close( const float fCloseSpdRate )
{
	m_isClose = true;
	if( m_pNextWindow ){
		m_pNextWindow->Close( fCloseSpdRate );
		return;
	}

	m_upSound->PlaySE( clsSOUND_MANAGER_MENUWINDOW::enSE_WIN_DISAPP );

	SetSizeTarget( { 0.0f, 0.0f, 0.0f } );

	const D3DXVECTOR2 vCLOSE_SPD = { m_vSize.x / fCloseSpdRate, m_vSize.y / fCloseSpdRate };
	AddChangeData( 
		vCLOSE_SPD.x, vCLOSE_SPD.y, 
		encBEFOR_CHANGE::BOTH );

	Operation( false );
	if( m_pParentWindow ){
		m_pParentWindow->Operation( true );
		m_pParentWindow = nullptr;
	}

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

void clsMENU_WINDOW_BASE::SetTextAlpha( const float& fAlpha )
{
	m_wpFont->SetAlpha( fAlpha );
	for( unsigned int i=0; i<m_vecupUiText.size(); i++ ){
		m_vecupUiText[i]->SetAlpha( fAlpha );
	}

}


//窓の左上を0として座標を与える.
D3DXVECTOR3 clsMENU_WINDOW_BASE::SetPosFromWindow( 
	const D3DXVECTOR2& vPos )
{
	D3DXVECTOR3 vReturn;
	vReturn = { vPos.x, vPos.y, 0.0f };

	//窓の中心へ.
	vReturn += m_vPos;
	//左上へ.
	const float fHARH = 0.5f;
	vReturn -= m_vSize * fHARH;

	return vReturn;
}


	//操作.
bool clsMENU_WINDOW_BASE::SelectUp( bool isWithStick )	
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_UP ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_UP ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDInput->IsLSUpStay()/* && isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_UP ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldUp.iHoldFream ++;
		//最初.
		if(!m_HoldUp.iFirstPush ){
			m_HoldUp.iFirstPush ++;
			m_HoldUp.iHoldFream = 0;
			return true;
		}
		//二番目.
		else if(m_HoldUp.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldUp.iHoldFream >= iHOLD_FREAM ){
				m_HoldUp.iFirstPush ++;
				m_HoldUp.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldUp.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldUp.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldUp.iHoldFream = iHOLD_FREAM;
		m_HoldUp.iFirstPush = 0;
	}

	return false;
}

bool clsMENU_WINDOW_BASE::SelectDown( bool isWithStick )	
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_DOWN ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_DOWN ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDInput->IsLSDownStay() /*&& isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_DOWN ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldDown.iHoldFream ++;
		//最初.
		if(!m_HoldDown.iFirstPush ){
			m_HoldDown.iFirstPush ++;
			m_HoldDown.iHoldFream = 0;
			return true;
		}
		//二番目.
		else if(m_HoldDown.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldDown.iHoldFream >= iHOLD_FREAM ){
				m_HoldDown.iFirstPush ++;
				m_HoldDown.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldDown.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldDown.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldDown.iHoldFream = iHOLD_FREAM;
		m_HoldDown.iFirstPush = 0;
	}

	return false;
}

bool clsMENU_WINDOW_BASE::SelectRight( bool isWithStick )	
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_RIGHT ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_RIGHT ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDInput->IsLSRightStay() /*&& isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldRight.iHoldFream ++;
		//最初.
		if(!m_HoldRight.iFirstPush ){
			m_HoldRight.iFirstPush ++;
			m_HoldRight.iHoldFream = 0;
			return true;
		}
		//二番目.
		else if(m_HoldRight.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldRight.iHoldFream >= iHOLD_FREAM ){
				m_HoldRight.iFirstPush ++;
				m_HoldRight.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldRight.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldRight.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldRight.iHoldFream = iHOLD_FREAM;
		m_HoldRight.iFirstPush = 0;
	}

	return false;
}

bool clsMENU_WINDOW_BASE::SelectLeft( bool isWithStick )	
{
	bool isPush = false;
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressStay( XINPUT_LEFT ) ){
			isPush = true;
		}
		else if( m_wpXInput->isSlopeStay( XINPUT_LEFT ) && isWithStick ){
			isPush = true;
		}
	}
	else{
		if( m_wpDInput->IsLSLeftStay() /*&& isWithStick*/ ){
			isPush = true;
		}
	}

	if( GetAsyncKeyState( VK_LEFT ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldLeft.iHoldFream ++;
		//最初.
		if(!m_HoldLeft.iFirstPush ){
			m_HoldLeft.iFirstPush ++;
			m_HoldLeft.iHoldFream = 0;
			return true;
		}
		//二番目.
		else if(m_HoldLeft.iFirstPush == iHOLD_FREAM_FIRST_STEP ){
			if( m_HoldLeft.iHoldFream >= iHOLD_FREAM ){
				m_HoldLeft.iFirstPush ++;
				m_HoldLeft.iHoldFream = 0;
				return true;
			}
		}
		else{
			if( m_HoldLeft.iHoldFream >= iHOLD_FREAM_FIRST ){
				m_HoldLeft.iHoldFream = 0;
				return true;
			}
		}
	}
	else{
		m_HoldLeft.iHoldFream = iHOLD_FREAM;
		m_HoldLeft.iFirstPush = 0;
	}

	return false;
}

bool clsMENU_WINDOW_BASE::SelectEnter()	
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_B ) ){
			return true;
		}
	}
	else{
		if( m_wpDInput->IsPressKeyEnter( DINPUT_ENTER ) ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_RETURN ) & 0x1 ){
		return true;
	}

	return false;
}

bool clsMENU_WINDOW_BASE::SelectExit()	
{
	if( m_wpXInput->isConnected() )
	{
		if( m_wpXInput->isPressEnter( XINPUT_A ) ){
			return true;
		}
	}
	else{
		if( m_wpDInput->IsPressKeyEnter( DINPUT_EXIT ) ){
			return true;
		}
	}

	if( GetAsyncKeyState( VK_BACK ) & 0x1 ){
		return true;
	}

	return false;
}


