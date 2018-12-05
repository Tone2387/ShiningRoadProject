#include "MenuWindowBase.h"


#include "PtrGroup.h"
#include "SoundManagerBase.h"
#include "CFont.h"
#include "UiText.h"
#include "DxInput.h"
#include "CXInput.h"


using namespace std;

namespace{
	//�{�^���̃z�[���h�t���[��.
	const int iHOLD_FREAM = 30;
	const int iHOLD_FREAM_FIRST = 6;
	const int iHOLD_FREAM_FIRST_STEP = 1;
}

clsMENU_WINDOW_BASE::clsMENU_WINDOW_BASE(		
	clsPOINTER_GROUP* const pPtrGroup,
	clsMENU_WINDOW_BASE* pParentWindow,
	std::vector<unsigned int>* const pInformationVec,
	const int iCloseSeNum )
		:clsWINDOW_BOX( pPtrGroup->GetDevice(), pPtrGroup->GetContext() )
		,m_pPtrGroup( pPtrGroup )
		,m_pInformationVec( pInformationVec )
		,m_wpFont( pPtrGroup->GetFont() )
		,m_wpXInput( pPtrGroup->GetXInput() )
		,m_wpDInput( pPtrGroup->GetDxInput() )
		,m_wpSound( pPtrGroup->GetSound() )
		,m_pParentWindow( pParentWindow )
		,m_pNextWindow( nullptr )
		,m_isOperation( true )
		,m_iSelectNum( 0 )
		,m_uiInformation( 0 )
		,m_isClose( false )
		,m_iCLOSE_SE_NUM( iCloseSeNum )
{
	Operation( true );
	if( pParentWindow ){
		pParentWindow->Operation( false );
	}

	const char sCURSOR_PATH[] = "Data\\Image\\MenuWindow\\Cursor.png";
	const WHSIZE_FLOAT CURSOR_DISP = { 1.0f, 1.0f };
	const float fCURSOR_ALPHA = 0.75f;
	SPRITE_STATE ss;
	ss.Disp = CURSOR_DISP;
	m_upCursor = make_unique< clsSprite2D >();
	m_upCursor->Create( pPtrGroup->GetDevice(), pPtrGroup->GetContext(), sCURSOR_PATH, ss );
	m_upCursor->SetAlpha( fCURSOR_ALPHA );

//	//�p����ł��.
//	//�����J��.
//	Open( D3DXVECTOR2 vWINDOW_SIZE );
}


clsMENU_WINDOW_BASE::~clsMENU_WINDOW_BASE()
{
	SAFE_DELETE( m_pNextWindow );

	Operation( false );
	m_pParentWindow = nullptr;

	m_wpSound = nullptr;
	m_wpDInput = nullptr;
	m_wpXInput = nullptr;
	m_wpFont = nullptr;
	m_pInformationVec = nullptr;
	m_pPtrGroup = nullptr;
}


//���̃��j���[�E�B���h�E��delete�͂���if���̒��Ŏg���܂��傤.
bool clsMENU_WINDOW_BASE::isDeletePermission()
{
	const float fDELETE_SIZE = 0.1f;
	if( m_vSize.x <= fDELETE_SIZE && m_vSize.y <= fDELETE_SIZE ){
		return true;
	}

	return false;
}


//�p����̃R���X�g���N�^�Ŏg��.
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

	//�q���𓮂���.
	if( m_pNextWindow ){
		m_pNextWindow->Update();
		//�q����������.
		if( m_pNextWindow->GetSize().x <= 0.0f && m_pNextWindow->GetSize().y <= 0.0f ){
			delete m_pNextWindow;
			m_pNextWindow = nullptr;
			//����\������Ă����玩��������.
			if( m_isClose ){
				Close();
			}
		}
	}

	//���쌠���Ȃ��Ȃ瓮�����Ȃ�.
	if( !m_isOperation ){
		return;
	}
	//�傫���ω����͓������Ȃ�.
	else if( !isStopChange() ){
		return;
	}


	UpdateProduct();
}

void clsMENU_WINDOW_BASE::Render()
{
	clsWINDOW_BOX::Render();

	//���삷�鑋�����J�[�\���\��.
	if( m_isOperation && isStopChange() ){
		m_upCursor->Render();
	}

	//���삵�Ȃ����͈Â�����.
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

	//�����ƃJ�[�\��.
	if( isStopChange() ){
		RenderProduct();
	}

	//�q����`��.
	if( m_pNextWindow ){
		m_pNextWindow->Render();
	}

}


//���̑���f���o��( ���������Ȃ�true ).
bool clsMENU_WINDOW_BASE::CreateNextWindow( 
	clsMENU_WINDOW_BASE** ppOutNextWindow )
{
	//���ɍ���Ă���Ȃ�.
	if( !ppOutNextWindow ){
		delete *ppOutNextWindow;
		*ppOutNextWindow = nullptr;
	}

	//��ꂽ��.
	if( CreateNextWindowProduct( ppOutNextWindow, this ) ){
		Operation( false );
		m_pNextWindow = *ppOutNextWindow;
		return true;
	}

	return false;
}




//���̃E�B���h�E����n�߂Đe�E�B���h�E�ɑ����Ԃ�.
void clsMENU_WINDOW_BASE::Close( const float fCloseSpdRate )
{
	m_isClose = true;
	if( m_pNextWindow ){
		m_pNextWindow->Close( fCloseSpdRate );
		return;
	}

	m_wpSound->PlaySE( m_iCLOSE_SE_NUM );

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


//���̍����0�Ƃ��č��W��^����.
D3DXVECTOR3 clsMENU_WINDOW_BASE::SetPosFromWindow( 
	const D3DXVECTOR2& vPos )
{
	D3DXVECTOR3 vReturn;
	vReturn = { vPos.x, vPos.y, 0.0f };

	//���̒��S��.
	vReturn += m_vPos;
	//�����.
	const float fHARH = 0.5f;
	vReturn -= m_vSize * fHARH;

	return vReturn;
}


	//����.
bool clsMENU_WINDOW_BASE::SelectUp( bool isWithStick )	
{
	bool isPush = false;
	if( m_wpXInput->isPressStay( XINPUT_UP ) ){
		isPush = true;
	}
	else if( m_wpXInput->isSlopeStay( XINPUT_UP ) && isWithStick ){
		isPush = true;
	}
	else if( m_wpDInput->IsLSUpStay() && isWithStick ){
		isPush = true;
	}
	else if( GetAsyncKeyState( VK_UP ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldUp.iHoldFream ++;
		//�ŏ�.
		if(!m_HoldUp.iFirstPush ){
			m_HoldUp.iFirstPush ++;
			m_HoldUp.iHoldFream = 0;
			return true;
		}
		//��Ԗ�.
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
	if( m_wpXInput->isPressStay( XINPUT_DOWN ) ){
		isPush = true;
	}
	else if( m_wpXInput->isSlopeStay( XINPUT_DOWN ) && isWithStick ){
		isPush = true;
	}
	else if( m_wpDInput->IsLSDownStay() && isWithStick ){
		isPush = true;
	}
	else if( GetAsyncKeyState( VK_DOWN ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldDown.iHoldFream ++;
		//�ŏ�.
		if(!m_HoldDown.iFirstPush ){
			m_HoldDown.iFirstPush ++;
			m_HoldDown.iHoldFream = 0;
			return true;
		}
		//��Ԗ�.
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
	if( m_wpXInput->isPressStay( XINPUT_RIGHT ) ){
		isPush = true;
	}
	else if( m_wpXInput->isSlopeStay( XINPUT_RIGHT ) && isWithStick ){
		isPush = true;
	}
	else if( m_wpDInput->IsLSRightStay() && isWithStick ){
		isPush = true;
	}
	else if( GetAsyncKeyState( VK_RIGHT ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldRight.iHoldFream ++;
		//�ŏ�.
		if(!m_HoldRight.iFirstPush ){
			m_HoldRight.iFirstPush ++;
			m_HoldRight.iHoldFream = 0;
			return true;
		}
		//��Ԗ�.
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
	if( m_wpXInput->isPressStay( XINPUT_LEFT ) ){
		isPush = true;
	}
	else if( m_wpXInput->isSlopeStay( XINPUT_LEFT ) && isWithStick ){
		isPush = true;
	}
	else if( m_wpDInput->IsLSLeftStay() && isWithStick ){
		isPush = true;
	}
	else if( GetAsyncKeyState( VK_LEFT ) & 0x8000 ){
		isPush = true;
	}

	if( isPush ){
		m_HoldLeft.iHoldFream ++;
		//�ŏ�.
		if(!m_HoldLeft.iFirstPush ){
			m_HoldLeft.iFirstPush ++;
			m_HoldLeft.iHoldFream = 0;
			return true;
		}
		//��Ԗ�.
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
	if( GetAsyncKeyState( VK_RETURN ) & 0x1 ){
		return true;
	}
	else if( m_wpXInput->isPressEnter( XINPUT_B ) ){
		return true;
	}

	return false;
}

bool clsMENU_WINDOW_BASE::SelectExit()	
{
	if( GetAsyncKeyState( VK_BACK ) & 0x1 ){
		return true;
	}
	else if( m_wpXInput->isPressEnter( XINPUT_A ) ){
		return true;
	}

	return false;
}


