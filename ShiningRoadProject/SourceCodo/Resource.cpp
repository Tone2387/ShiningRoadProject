#include "Resource.h"

clsResource::clsResource()
{
	m_hWnd = nullptr;
	m_pDevice11 = nullptr;
	m_pCotext11 = nullptr;
	m_ppStaticModels = nullptr;
	m_ppSkinModels = nullptr;
	ZeroMemory( m_FilePath, sizeof( m_FilePath ) );
}

clsResource::~clsResource()
{
	for( UCHAR i=0; i<enStaticModel_Max; i++ ){
		ReleaseStaticModel( static_cast<enSTATIC_MODEL>( i ) );
	}
	for (UCHAR i = 0; i<enSkinModel_Max; i++){
		ReleaseSkinModel( static_cast<enSKIN_MODEL>( i ) );
	}
	//スキンメッシュのポインタ領域を解放.
	if( m_ppSkinModels ){
		delete[] m_ppSkinModels;
		m_ppSkinModels = nullptr;
	}
	//スタティックメッシュのポインタ領域を解放.
	if( m_ppStaticModels ){
		delete[] m_ppStaticModels;
		m_ppStaticModels = nullptr;
	}


	m_pCotext11 = nullptr;
	m_pDevice11 = nullptr;
	m_hWnd = nullptr;
}


//new直後に使う.
void clsResource::Create( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext )
{
	InitStaticModel( hWnd, pDevice, pContext );
	InitSkinModel( hWnd, pDevice, pContext );

	CreateStaticModel( 
		"Data\\Stage\\yuka.x", 
		clsResource::enSTATIC_MODEL::enStaticModel_Ground );
	CreateStaticModel(
		"Data\\Trap\\Spia.x", 
		clsResource::enSTATIC_MODEL::enStaticModel_Enemy );

	CreateSkinModel(
		"Data\\hime\\hime_run.x", 
		clsResource::enSKIN_MODEL::enSkinModel_Player );
}


//==================================================.
//	初期化.
//==================================================.
HRESULT clsResource::InitStaticModel( 
			const HWND hWnd,
			ID3D11Device* const pDevice,
			ID3D11DeviceContext* const pContext)
{
	m_hWnd = hWnd;
	m_pDevice11 = pDevice;
	m_pCotext11 = pContext;
	//スタティックメッシュのポインタ領域を確保.
	m_ppStaticModels = new clsDX9Mesh*[ enStaticModel_Max ];
	for( int i=0; i<enStaticModel_Max; i++ ){
		m_ppStaticModels[i] = nullptr;
	}

	return S_OK;
}
HRESULT clsResource::InitSkinModel( 
			const HWND hWnd,
			ID3D11Device* const pDevice,
			ID3D11DeviceContext* const pContext)
{
	m_Si.hWnd = hWnd;
	m_Si.pDevice = pDevice;
	m_Si.pDeviceContext = pContext;
	//スキンメッシュのポインタ領域を確保.
	m_ppSkinModels = new clsD3DXSKINMESH*[enSkinModel_Max];
	for (int i = 0; i<enSkinModel_Max; i++){
		m_ppSkinModels[i] = nullptr;
	}

	return S_OK;
}



//==================================================.
//	モデル読込.
//==================================================.
// Static Model.
HRESULT clsResource::CreateStaticModel( LPSTR const fileName, const enSTATIC_MODEL enModel )
{
	//列挙体の範囲内?.
	if( !IsRangeStaticModel( enModel ) ){
		return E_FAIL;
	}
	//既にデータがあるか.
	if( m_ppStaticModels[ enModel ] != nullptr ){
		return E_FAIL;
	}

	//モデル読込.
	m_ppStaticModels[ enModel ] = new clsDX9Mesh;
	m_ppStaticModels[ enModel ]->Init(
		m_hWnd, m_pDevice11, m_pCotext11, fileName );

	return S_OK;
}
//	Skin Model.
HRESULT clsResource::CreateSkinModel( LPSTR const fileName, const enSKIN_MODEL enModel )
{
	//列挙体の範囲内か.
	if ( !IsRangeSkinModel( enModel ) ){
		return E_FAIL;
	}

	//既にデータがあるか.
	if ( m_ppSkinModels[ enModel ] != nullptr ){
		return E_FAIL;
	}

	//モデル読込.
	m_ppSkinModels[ enModel ] = new clsD3DXSKINMESH;
	m_ppSkinModels[ enModel ]->Init( &m_Si );
	m_ppSkinModels[ enModel ]->CreateFromX( fileName );

	return S_OK;
}

//==================================================.
//	モデルのポインタ取得.
//==================================================.
//	Static Model.
clsDX9Mesh* clsResource::GetStaticModels( const enSTATIC_MODEL enModel ) const
{
	//列挙体の範囲内?.
	if( !IsRangeStaticModel( enModel ) ){
		return nullptr;
	}
	//既にデータがあるか.
	if( m_ppStaticModels[ enModel ] == nullptr ){
		return nullptr;
	}
	
	//モデルのポインタ返す.
	return m_ppStaticModels[ enModel ];
}
//	Skin Model.
clsD3DXSKINMESH* clsResource::GetSkinModels( const enSKIN_MODEL enModel) const
{
	//列挙体の範囲内?.
	if ( !IsRangeSkinModel( enModel ) ){
		return nullptr;
	}

	//既にデータがあるか.
	if ( m_ppSkinModels[ enModel ] == nullptr ){
		return nullptr;
	}

	//モデルのポインタ返す.
	return m_ppSkinModels[ enModel ];
}


//==================================================.
//	解放.
//==================================================.
//	Static Model.
HRESULT clsResource::ReleaseStaticModel( const enSTATIC_MODEL enModel )
{
	//列挙体の範囲内?.
	if( !IsRangeStaticModel( enModel ) ){
		return E_FAIL;
	}
	//既にデータがあるか.
	if( m_ppStaticModels[ enModel ] == nullptr ){
		return E_FAIL;
	}

	delete m_ppStaticModels[ enModel ];
	m_ppStaticModels[ enModel ] = nullptr;

	return S_OK;
}
//	Skin Model.
HRESULT clsResource::ReleaseSkinModel( const enSKIN_MODEL enModel )
{
	//列挙体の範囲内?.
	if( !IsRangeSkinModel( enModel ) ){
		return E_FAIL;
	}
	//既にデータがあるか.
	if( m_ppSkinModels[ enModel ] == nullptr ){
		return E_FAIL;
	}

	delete m_ppSkinModels[ enModel ];
	m_ppSkinModels[ enModel ] = nullptr;

	return S_OK;
}


//==================================================.
//	範囲内かチェックする関数.
//==================================================.
bool clsResource::IsRangeStaticModel( const enSTATIC_MODEL enModel ) const
{
	if( 0 <= enModel && enModel < enStaticModel_Max ){
		return true;//範囲内.
	}
	return false;	//範囲外.
}
bool clsResource::IsRangeSkinModel( const enSKIN_MODEL enModel ) const
{
	if (0 <= enModel && enModel < enSkinModel_Max){
		return true;
	}
	return false;
}
