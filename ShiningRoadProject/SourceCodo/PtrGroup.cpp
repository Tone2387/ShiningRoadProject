#include "PtrGroup.h"

clsPOINTER_GROUP::clsPOINTER_GROUP(		
	ID3D11Device* const pDevice,
	ID3D11DeviceContext* const pContext,
	D3D10_VIEWPORT* const pViewPort,
	ID3D11DepthStencilState* const pDepthState,
		clsDxInput* const pDxInput,
		clsXInput* const pXInput,
	clsResource* const pResource,
	clsEffects* const pEffects,
	clsSOUND_MANAGER_BASE* const pSound,
	clsROBO_STATUS* const pRoboStatus,
	clsBLACK_SCREEN* const pBlack )
		:m_wpDevice( pDevice )
		,m_wpContext( pContext )
		,m_wpViewPort( pViewPort )
		,m_wpDepthStencilState( pDepthState )
		,m_wpDxInput( pDxInput )
		,m_wpXInput( pXInput )
		,m_wpResource( pResource )
		,m_wpEffects( pEffects )
		,m_wpSound( pSound )
		,m_wpRoboStatus( pRoboStatus )
		,m_wpBlackScreen( pBlack )
		,m_wpCamera( nullptr )
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
	m_wpDepthStencilState = nullptr;
	m_wpViewPort = nullptr;
	m_wpContext = nullptr;
	m_wpDevice = nullptr;
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

D3D10_VIEWPORT* clsPOINTER_GROUP::GetViewPort() const
{
	return m_wpViewPort;
}

ID3D11DepthStencilState* clsPOINTER_GROUP::GetDepthState() const
{
	return m_wpDepthStencilState;
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
clsROBO_STATUS* clsPOINTER_GROUP::GetRoboStatus() const
{
	return m_wpRoboStatus;
}

//暗転用黒スプライト.
clsBLACK_SCREEN* clsPOINTER_GROUP::GetBlackScreen() const
{
	return m_wpBlackScreen;
}
