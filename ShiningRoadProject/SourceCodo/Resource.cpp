#include "Resource.h"

using namespace std;
const string sPARTS_PASS = "Data\\RoboParts\\";
const string sLEG_PASS = sPARTS_PASS + "Leg\\Leg";
const string sCORE_PASS = sPARTS_PASS + "Core\\Core";
const string sHEAD_PASS = sPARTS_PASS + "Head\\Head";
const string sARML_PASS = sPARTS_PASS + "ArmL\\ArmL";
const string sARMR_PASS = sPARTS_PASS + "ArmR\\ArmR";
const string sWEAPON_PASS = sPARTS_PASS + "Weapon\\Weapon";
//extension = 拡張子.
const string sEXTENSION_X = ".X";//上記のパスとこれの間に数字を挟んで使う.


const int iTEST_ROBO_PARTS_MODEL_MAX = 4;//テスト中のパーツ最大数.


clsResource::clsResource()
{
	m_hWnd = nullptr;
	m_pDevice11 = nullptr;
	m_pCotext11 = nullptr;
	m_ppStaticModels = nullptr;
	m_ppSkinModels = nullptr;

	m_ucSkinModelMax = m_ucLegNum = m_ucCoreNum = m_ucHeadNum = m_ucArmsNum = m_ucWeaponNum = 0;

	ZeroMemory( m_FilePath, sizeof( m_FilePath ) );
}

clsResource::~clsResource()
{
	for( UCHAR i=0; i<enStaticModel_Max; i++ ){
		ReleaseStaticModel( static_cast<enSTATIC_MODEL>( i ) );
	}
	for( SKIN_ENUM_TYPE i = 0; i<m_ucSkinModelMax; i++ ){
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

	m_ucSkinModelMax = m_ucLegNum = m_ucCoreNum = m_ucHeadNum = m_ucArmsNum = m_ucWeaponNum = 0;

	m_pCotext11 = nullptr;
	m_pDevice11 = nullptr;
	m_hWnd = nullptr;
}


//new直後に使う.
void clsResource::Create( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext )
{
	m_ucLegNum = m_ucCoreNum = m_ucHeadNum = m_ucArmsNum = m_ucWeaponNum = iTEST_ROBO_PARTS_MODEL_MAX;
	m_ucSkinModelMax = m_ucLegNum + m_ucCoreNum + m_ucHeadNum + m_ucArmsNum + m_ucWeaponNum + enSkinModel_Max;

	InitStaticModel( hWnd, pDevice, pContext );
	InitSkinModel( hWnd, pDevice, pContext );

	CreateStaticModel( 
		"Data\\Stage\\kami_map.x",
//		"Data\\RoboParts\\Head\\Head1.X",
//		"Data\\RoboParts\\Weapon\\Weapon1.X",
//		"Data\\RoboParts\\Leg\\Leg0.X",
//		"Data\\RoboParts\\ArmR\\ArmR1.X",
//		"Data\\RoboParts\\ArmL\\ArmL1.X",
//		"Data\\RoboParts\\Core\\Core1.X",
		enSTATIC_MODEL::enStaticModel_Ground );
	CreateStaticModel(
		"Data\\Trap\\Spia.x",
		enSTATIC_MODEL::enStaticModel_Enemy );

	CreateSkinModel(
		"Data\\hime\\hime_run.x",
		enSKIN_MODEL::enSkinModel_Player );
	CreateSkinModel(
		"Data\\RoboParts\\Leg\\Leg0.X",
//		"Data\\hime\\hime_kougeki.x", 
		enSKIN_MODEL::enSkinModel_Leg );

	//パーツ作成.
	CreatePartsGroup();
}

//パーツ作成.
void clsResource::CreatePartsGroup()
{
	CreateParts( enPARTS::LEG );
	CreateParts( enPARTS::CORE );
	CreateParts( enPARTS::HEAD );
	CreateParts( enPARTS::ARM_L	);
	CreateParts( enPARTS::ARM_R	);
	CreateParts( enPARTS::WEAPON_L );//武器はLR同じなのでRは無視.
}
void clsResource::CreateParts( const enPARTS enParts )
{
	UCHAR ucStart, ucMax;
	string sPass = SetVarToCreateParts( ucStart, ucMax, enParts );
	
	//作成.
	for( UCHAR i=0; i<ucMax - ucStart; i++ ){
		//パーツファイル名連結.
		ostringstream ss;
		ss << static_cast<int>( i );
		string tmpString = sPass + ss.str();
		tmpString += sEXTENSION_X;
		//メモリ確保.
		char *tmpPass = new char[tmpString.size() + 1];
		//stringからchar[]へコピー.
		char_traits<char>::copy( 
			tmpPass, tmpString.c_str(), tmpString.size() + 1 );
		//作る.
		CreateSkinModel(
			tmpPass, 
			static_cast<enSKIN_MODEL>( ucStart + i ) );

		delete[] tmpPass;
	}
}

//CreatePartsで必要な変数を準備する.
string clsResource::SetVarToCreateParts(
	SKIN_ENUM_TYPE &ucStart,	//(out)そのパーツの始まり番号.
	SKIN_ENUM_TYPE &ucMax,	//(out)そのパーツの最大番号.
	const enPARTS enParts )
{
	string sPass;

	//そのパーツのリソース番号での開始番号.
	ucStart = GetPartsResourceStart( enParts );

	switch( enParts )
	{
	case enPARTS::LEG:
		ucMax = ucStart + m_ucLegNum;
		sPass = sLEG_PASS;
		break;
	case enPARTS::CORE:
		ucMax = ucStart + m_ucCoreNum;
		sPass = sCORE_PASS;
		break;
	case enPARTS::HEAD:
		ucMax = ucStart + m_ucHeadNum;
		sPass = sHEAD_PASS;
		break;
	case enPARTS::ARM_L:
		ucMax = ucStart + m_ucArmsNum;
		sPass = sARML_PASS;
		break;
	case enPARTS::ARM_R:
		ucMax = ucStart + m_ucArmsNum;
		sPass = sARMR_PASS;
		break;
	case enPARTS::WEAPON_L:
	case enPARTS::WEAPON_R:
		ucMax = ucStart + m_ucWeaponNum;
		sPass = sWEAPON_PASS;
		break;
	default:
		assert( !"不正なパーツが指定されました" );
		break;
	}
	return sPass;
}
//GetSkinModels()の引数をどのパーツかとそのパーツの番号から引き出す関数.
clsResource::enSKIN_MODEL clsResource::GetPartsResourceNum( 
	const enPARTS enParts, const SKIN_ENUM_TYPE PartsNum ) const
{
	enSKIN_MODEL SkinResourceNo;

	SkinResourceNo = 
		static_cast<enSKIN_MODEL>( GetPartsResourceStart( enParts ) + PartsNum );

	return SkinResourceNo;
}
//SetVarToCreateParts()やGetPartsResourceNum()の補助.
//そのパーツの最初のナンバーをリソース番号にして教えてくれる.
SKIN_ENUM_TYPE clsResource::GetPartsResourceStart( const enPARTS enParts ) const
{
	switch( enParts )
	{
	case enPARTS::LEG:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max );
		break;
	case enPARTS::CORE:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_ucLegNum );
		break;
	case enPARTS::HEAD:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_ucLegNum + m_ucCoreNum );
		break;
	case enPARTS::ARM_L:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_ucLegNum + m_ucCoreNum + m_ucHeadNum );
		break;
	case enPARTS::ARM_R:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_ucLegNum + m_ucCoreNum + m_ucHeadNum + m_ucArmsNum );
		break;
	case enPARTS::WEAPON_L:
	case enPARTS::WEAPON_R:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_ucLegNum + m_ucCoreNum + m_ucHeadNum + ( m_ucArmsNum * 2 ) );
		break;
	default:
		assert( !"不正なパーツが指定されました" );
		break;
	}

	return 0;
}
//ロボのパーツをAttachする関数.
//第一引数 : 何のパーツ?.
//第二引数 : そのパーツの何番目?.
clsD3DXSKINMESH* clsResource::GetPartsModels(
	const enPARTS enParts, const SKIN_ENUM_TYPE PartsNum )
{
	return GetSkinModels( GetPartsResourceNum( enParts, PartsNum ) );
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
	for( UCHAR i=0; i<enStaticModel_Max; i++ ){
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
	m_ppSkinModels = new clsD3DXSKINMESH*[m_ucSkinModelMax];
	for ( SKIN_ENUM_TYPE i = 0; i<m_ucSkinModelMax; i++ ){
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
	if ( 0 <= enModel && enModel < m_ucSkinModelMax ){
		return true;
	}
	return false;
}
