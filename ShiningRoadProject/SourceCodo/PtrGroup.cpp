#include "PtrGroup.h"

clsPOINTER_GROUP::clsPOINTER_GROUP(		
	ID3D11Device* const pDevice,
	ID3D11DeviceContext* const pContext,
	clsDxInput* const pInput,
	clsResource* const pResource,
	clsEffects* const pEffects,
	clsSOUND_MANAGER* const pSound ) :
		m_wpDevice( pDevice ),
		m_wpContext( pContext ),
		m_wpDxInput( pInput ),
		m_wpResource( pResource ),
		m_wpEffects( pEffects ),
		m_wpSound( pSound ),
		m_wpCamera( nullptr )
{
}

clsPOINTER_GROUP::~clsPOINTER_GROUP()
{
	m_wpCamera = nullptr;
	m_wpSound = nullptr;
	m_wpEffects = nullptr;
	m_wpResource = nullptr;
	m_wpDxInput = nullptr;
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

//ジョイスティック.
clsDxInput* clsPOINTER_GROUP::GetDxInput() const
{
	return m_wpDxInput;
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
clsSOUND_MANAGER* clsPOINTER_GROUP::GetSound() const
{
	return m_wpSound;
}

//カメラ.
clsCAMERA_BASE*	clsPOINTER_GROUP::GetCamera() const
{
	return m_wpCamera;
}
