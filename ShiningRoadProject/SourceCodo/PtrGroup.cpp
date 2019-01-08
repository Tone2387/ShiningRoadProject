#include "PtrGroup.h"

#include "DxInput.h"
#include "CXInput.h"
#include "Resource.h"
#include "Effects.h"
#include "SoundManagerBase.h"
#include "Camera.h"
#include "RoboStatusPlayer.h"
#include "BlackScreen.h"
#include "CFont.h"

clsPOINTER_GROUP::clsPOINTER_GROUP(		
	ID3D11Device* const pDevice,
	ID3D11DeviceContext* const pContext,
	D3D10_VIEWPORT* const pViewPort10,
	D3D11_VIEWPORT* const pViewPort11,
	clsDxInput* const pDxInput,
	clsXInput* const pXInput,
	clsResource* const pResource,
	clsEffects* const pEffects,
	clsSOUND_MANAGER_BASE* const pSound,
	clsROBO_STATUS_PLAYER* const pRoboStatus,
	clsBLACK_SCREEN* const pBlack,
	clsFont* const pFont )
		:m_wpDevice( pDevice )
		,m_wpContext( pContext )
		,m_wpViewPort10( pViewPort10 )
		,m_wpViewPort11( pViewPort11 )
		,m_wpDxInput( pDxInput )
		,m_wpXInput( pXInput )
		,m_wpResource( pResource )
		,m_wpEffects( pEffects )
		,m_wpSound( pSound )
		,m_wpRoboStatus( pRoboStatus )
		,m_wpBlackScreen( pBlack )
		,m_wpCamera( nullptr )
		,m_wpFont( pFont )
{
}

clsPOINTER_GROUP::~clsPOINTER_GROUP()
{
	m_wpCamera = nullptr;
	m_wpBlackScreen = nullptr;
	m_wpRoboStatus = nullptr;
	m_wpSound = nullptr;
	m_wpEffects = nullptr;
	m_wpResource = nullptr;
	m_wpXInput = nullptr;
	m_wpDxInput = nullptr;
	m_wpViewPort10 = nullptr;
	m_wpViewPort11 = nullptr;
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
}

//サウンドのアドレス取得.
void clsPOINTER_GROUP::UpdateSoundPtr( clsSOUND_MANAGER_BASE* const pSound )
{
	m_wpSound = pSound;
}

//カメラのアドレス取得.
void clsPOINTER_GROUP::UpdateCameraPtr( clsCAMERA_BASE* const pCamera )
{
	m_wpCamera = pCamera;
}

//----- ポインターを渡す -----//.
ID3D11Device* clsPOINTER_GROUP::GetDevice() const
{
	return m_wpDevice;
}

ID3D11DeviceContext* clsPOINTER_GROUP::GetContext() const
{
	return m_wpContext;
}

D3D10_VIEWPORT* clsPOINTER_GROUP::GetViewPort10() const
{
	return m_wpViewPort10;
}

D3D11_VIEWPORT* clsPOINTER_GROUP::GetViewPort11() const
{
	return m_wpViewPort11;
}


//ジョイスティック.
clsDxInput* clsPOINTER_GROUP::GetDxInput() const
{
	return m_wpDxInput;
}
clsXInput* clsPOINTER_GROUP::GetXInput() const
{
	return m_wpXInput;
}

//リソース.
clsResource* clsPOINTER_GROUP::GetResource() const
{
	return m_wpResource;
}
//エフェクトクラス.
clsEffects*	clsPOINTER_GROUP::GetEffects() const
{
	return m_wpEffects;
}
//BGM,SE.
clsSOUND_MANAGER_BASE* clsPOINTER_GROUP::GetSound() const
{
	return m_wpSound;
}

//カメラ.
clsCAMERA_BASE*	clsPOINTER_GROUP::GetCamera() const
{
	return m_wpCamera;
}

//( シーンをまたぐ )ロボットのステータス.
clsROBO_STATUS_PLAYER* clsPOINTER_GROUP::GetRoboStatus() const
{
	return m_wpRoboStatus;
}

//暗転用黒スプライト.
clsBLACK_SCREEN* clsPOINTER_GROUP::GetBlackScreen() const
{
	return m_wpBlackScreen;
}

clsFont* clsPOINTER_GROUP::GetFont() const
{
	return m_wpFont;
}
