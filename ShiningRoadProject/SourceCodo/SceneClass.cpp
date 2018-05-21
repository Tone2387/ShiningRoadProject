#include "SceneClass.h"
#include <Windows.h>

//================================//
//========== 基底クラス ==========//
//================================//
clsSCENE_BASE::clsSCENE_BASE( clsPOINTER_GROUP* const ptrGroup ) :
	m_wpDevice( ptrGroup->GetDevice() ),
	m_wpContext( ptrGroup->GetContext() ),
	m_wpPtrGroup( ptrGroup ),
	m_wpDxInput( ptrGroup->GetDxInput() ),
	m_wpResource( ptrGroup->GetResource() ),
	m_wpEffects( ptrGroup->GetEffects() ),
	m_wpSound( ptrGroup->GetSound() ),
	m_wpCamera( ptrGroup->GetCamera() )
{
}

clsSCENE_BASE::~clsSCENE_BASE()
{
	m_wpCamera = nullptr;
	m_wpSound = nullptr;
	m_wpEffects = nullptr;
	m_wpResource = nullptr;
	m_wpDxInput = nullptr;
	m_wpPtrGroup = nullptr;
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
}

D3DXVECTOR3 clsSCENE_BASE::GetCameraPos() const
{
	ASSERT_IF_NULL( m_wpCamera );
	return m_wpCamera->GetPos();
}
D3DXVECTOR3 clsSCENE_BASE::GetCameraLookPos() const
{
	ASSERT_IF_NULL( m_wpCamera );
	return m_wpCamera->GetLookPos();
}

//デバッグ用シーン切り替え.
void clsSCENE_BASE::DebugChangeScene( enSCENE &nextScene ) const
{
	if( GetAsyncKeyState( VK_F5 ) & 0x1 ){
		nextScene = enSCENE::TITLE;
	}
	else if( GetAsyncKeyState( VK_F6 ) & 0x1 ){
		nextScene = enSCENE::ASSEMBLE;
	}
	else if( GetAsyncKeyState( VK_F7  ) & 0x1 ){
		nextScene = enSCENE::MISSION;
	}
	else if( GetAsyncKeyState( VK_F8  ) & 0x1 ){
		nextScene = enSCENE::ENDING;
	}
}
