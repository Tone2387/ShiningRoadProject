#include "Resource.h"
#include "File.h"

#include "OperationString.h"



using namespace std;

namespace{

	//パーツディレクトリのパス.
	const string sPARTS_PASS = "Data\\RoboParts\\";
	//各パーツのディレクトリのパス.
	const string sLEG_PASS		= sPARTS_PASS + "Leg\\Leg";
	const string sCORE_PASS		= sPARTS_PASS + "Core\\Core";
	const string sHEAD_PASS		= sPARTS_PASS + "Head\\Head";
	const string sARML_PASS		= sPARTS_PASS + "Arms\\Arms";//腕が左右同じなのに分けているのは仕様変更の可能性があるため.
	const string sARMR_PASS		= sPARTS_PASS + "Arms\\Arms";
	const string sWEAPON_PASS	= sPARTS_PASS + "Weapon\\Weapon";
	//上記に数字を挟んだうえでくっつけるモデル名.
	const string sLEG_NAME		= "\\Leg";
	const string sCORE_NAME		= "\\Core";
	const string sHEAD_NAME		= "\\Head";
	const string sARML_NAME		= "\\ArmL";
	const string sARMR_NAME		= "\\ArmR";
	const string sWEAPON_NAME	= "\\Weapon";

	//extension = 拡張子.
	const string sEXTENSION_X = ".X";//上記のパスとこれの間に数字を挟んで使う.

	const UCHAR ucARM_NUM = 2;//腕の本数.


	//データファイルのパス.
	const string sDATA_PASS_ROOT = "Data\\RoboParts\\";
	const string sDATA_PASS_MID[] =
	{
		"Leg",
		"Core",
		"Head",
		"Arms",
		"Weapon"
	};
	const string sDATA_PASS_END	 = "\\RoboPartsData.csv";

}



clsResource::clsResource()
	:m_hWnd( nullptr )
	,m_wpDevice11( nullptr )
	,m_wpCotext11( nullptr )
	,m_ppStaticModels( nullptr )
	,m_ppSkinModels( nullptr )
	,m_ucSkinModelMax( 0 )
	,m_PartsNum( )
{
	for( UCHAR i=0; i<enPARTS_READ_SIZE; i++ ){
		m_PartsNum[i] = 0;
	}
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

	for( UCHAR i=0; i<enPARTS_READ_SIZE; i++ ){
		m_PartsNum[i] = 0;
	}

	m_ucSkinModelMax = 0;

	m_wpCotext11 = nullptr;
	m_wpDevice11 = nullptr;
	m_hWnd = nullptr;
}

//パーツの数を吐き出す.
SKIN_ENUM_TYPE clsResource::GetPartsNum( const enPARTS_READ enPartsRead )
{
	SKIN_ENUM_TYPE tmpNum = 0;

	string tmpPass =  sDATA_PASS_ROOT + sDATA_PASS_MID[enPartsRead] + sDATA_PASS_END;
	
	unique_ptr< clsFILE > upFile = make_unique< clsFILE >();
	upFile->Open( tmpPass );

	tmpNum = static_cast<SKIN_ENUM_TYPE>( upFile->GetSizeRow() );

	upFile->Close();
	upFile.reset( nullptr );
	return tmpNum;
}

//new直後に使う.
void clsResource::Create( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext )
{
#ifdef RESOURCE_READ_PARTS_MODEL_LOCK
	for( UCHAR i=0; i<enPARTS_READ_SIZE; i++ ){
		m_PartsNum[i] = iTEST_ROBO_PARTS_MODEL_MAX;
	}
#else//#ifdef RESOURCE_READ_PARTS_MODEL_LOCK
	m_PartsNum[enPARTS_READ::LEG] = GetPartsNum( enPARTS_READ::LEG );
	m_PartsNum[enPARTS_READ::CORE] = GetPartsNum( enPARTS_READ::CORE );
	m_PartsNum[enPARTS_READ::HEAD] = GetPartsNum( enPARTS_READ::HEAD );
	m_PartsNum[enPARTS_READ::ARMS] = GetPartsNum( enPARTS_READ::ARMS );
	m_PartsNum[enPARTS_READ::WEAPON] = GetPartsNum( enPARTS_READ::WEAPON );
#endif//#ifdef RESOURCE_READ_PARTS_MODEL_LOCK

	m_ucSkinModelMax = 
		m_PartsNum[enPARTS_READ::LEG] + 
		m_PartsNum[enPARTS_READ::CORE] + 
		m_PartsNum[enPARTS_READ::HEAD] + 
		( ucARM_NUM * m_PartsNum[enPARTS_READ::ARMS] ) + 
		m_PartsNum[enPARTS_READ::WEAPON] + 
		enSkinModel_Max;

	InitStaticModel( hWnd, pDevice, pContext );
	InitSkinModel( hWnd, pDevice, pContext );

	CreateStaticModel( 
		"Data\\Stage\\kami_map.x",
		enSTATIC_MODEL::enStaticModel_StageBase );
	CreateStaticModel(
		"Data\\Stage\\Building.X",
		enSTATIC_MODEL::enStaticModel_Building );
	CreateStaticModel(
		"Data\\Collision\\Sphere.x",
		enSTATIC_MODEL::enStaticModel_Shpere );

	CreateSkinModel(
		"Data\\RoboParts\\Leg\\Leg0\\Leg0.x",
		enSKIN_MODEL::enSkinModel_Player );
	CreateSkinModel(
		"Data\\RoboParts\\Leg\\Leg0\\Leg0.X",
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
	//そのパーツの始まりと終わりの番号が、それぞれスキンメッシュ全体の何番目にあたるか、の変数.
	UCHAR ucStart, ucMax;
	//Xファイルの名前.
	string sModelName;
	//DataディレクトリからXファイルまでのパス.
	string sPass = SetVarToCreateParts( ucStart, ucMax, sModelName, enParts );
	//パスとモデル名を切り離しているのは、ディレクトリ名とモデル名に番号を入れるため.
	
	//作成.//そのパーツのモデル種類の数だけ繰り返す.
	for( UCHAR i=0; i<ucMax - ucStart; i++ ){
#if 0
		//パーツファイル名連結.
		ostringstream ss;
		ss << static_cast<int>( i );		//数字を文字列に( intじゃないと事故が起こるさ ).
		//ディレクトリ名、モデル名に数字を連結.
		string tmpString = sPass + ss.str();//パーツのディレクトリ名.
		tmpString += sModelName + ss.str();	//パーツのモデル名.
		tmpString += sEXTENSION_X;			//拡張子連結.
		//パーツファイル名連結完了.
#else
		//文字列操作クラス作成.
		unique_ptr<clsOPERATION_STRING> upOprtStr = make_unique<clsOPERATION_STRING>();
	
		string tmpString = upOprtStr->ConsolidatedNumber( sPass, i );//パーツのディレクトリ名.
		tmpString += upOprtStr->ConsolidatedNumber( sModelName, i );//パーツのモデル名.
		tmpString += sEXTENSION_X;									//拡張子連結.

		upOprtStr.reset( nullptr );
#endif

		//作る.
		CreateSkinModel(
			const_cast<LPSTR>( tmpString.c_str() ), //tmpPass.
			static_cast<enSKIN_MODEL>( ucStart + i ) );

	}
}

//CreatePartsで必要な変数を準備する.
string clsResource::SetVarToCreateParts(
	SKIN_ENUM_TYPE &ucStart,	//(out)そのパーツの始まり番号.
	SKIN_ENUM_TYPE &ucMax,	//(out)そのパーツの最大番号.
	string &sModelName,//(out)パスにくっつけるモデル名.
	const enPARTS enParts )
{
	string sPass;

	//そのパーツのリソース番号での開始番号.
	ucStart = GetPartsResourceStart( enParts );

	switch( enParts )
	{
	case enPARTS::LEG:
		ucMax = ucStart + m_PartsNum[enPARTS_READ::LEG];
		sPass = sLEG_PASS;
		sModelName = sLEG_NAME;
		break;
	case enPARTS::CORE:
		ucMax = ucStart + m_PartsNum[enPARTS_READ::CORE];
		sPass = sCORE_PASS;
		sModelName = sCORE_NAME;
		break;
	case enPARTS::HEAD:
		ucMax = ucStart + m_PartsNum[enPARTS_READ::HEAD];
		sPass = sHEAD_PASS;
		sModelName = sHEAD_NAME;
		break;
	case enPARTS::ARM_L:
		ucMax = ucStart + m_PartsNum[enPARTS_READ::ARMS];
		sPass = sARML_PASS;
		sModelName = sARML_NAME;
		break;
	case enPARTS::ARM_R:
		ucMax = ucStart + m_PartsNum[enPARTS_READ::ARMS];
		sPass = sARMR_PASS;
		sModelName = sARMR_NAME;
		break;
	case enPARTS::WEAPON_L:
//	case enPARTS::WEAPON_R:	//二回されないように片方だけにしておく.
		ucMax = ucStart + m_PartsNum[enPARTS_READ::WEAPON];
		sPass = sWEAPON_PASS;
		sModelName = sWEAPON_NAME;
		break;
	default:
		assert( !"不正なパーツが指定されました" );
		break;
	}
	return sPass;
}

//GetSkinModels()の引数をどのパーツかとそのパーツの番号から引き出す関数.
clsResource::enSKIN_MODEL clsResource::GetPartsResourceNum( 
	const enPARTS enParts, SKIN_ENUM_TYPE PartsNum ) const
{
	enSKIN_MODEL SkinResourceNo;//return.

	//オーバーしていたら0にする.
	switch( enParts )
	{
	case enPARTS::LEG:
		if( m_PartsNum[enPARTS_READ::LEG] <= PartsNum ) PartsNum = 0;
		break;
	case enPARTS::CORE:
		if( m_PartsNum[enPARTS_READ::CORE] <= PartsNum ) PartsNum = 0;
		break;
	case enPARTS::HEAD:
		if( m_PartsNum[enPARTS_READ::HEAD] <= PartsNum ) PartsNum = 0;
		break;
	case enPARTS::ARM_L:
	case enPARTS::ARM_R:
		if( m_PartsNum[enPARTS_READ::ARMS] <= PartsNum ) PartsNum = 0;
		break;
	case enPARTS::WEAPON_L:
	case enPARTS::WEAPON_R:
		if( m_PartsNum[enPARTS_READ::WEAPON] <= PartsNum ) PartsNum = 0;
		break;
	default:
		assert( !"不正なパーツが指定されました" );
		break;
	}


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
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_PartsNum[enPARTS_READ::LEG] );
		break;
	case enPARTS::HEAD:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_PartsNum[enPARTS_READ::LEG] + m_PartsNum[enPARTS_READ::CORE] );
		break;
	case enPARTS::ARM_L:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_PartsNum[enPARTS_READ::LEG] + m_PartsNum[enPARTS_READ::CORE] + m_PartsNum[enPARTS_READ::HEAD] );
		break;
	case enPARTS::ARM_R:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_PartsNum[enPARTS_READ::LEG] + m_PartsNum[enPARTS_READ::CORE] + m_PartsNum[enPARTS_READ::HEAD] + m_PartsNum[enPARTS_READ::ARMS] );
		break;
	case enPARTS::WEAPON_L:
	case enPARTS::WEAPON_R:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_PartsNum[enPARTS_READ::LEG] + m_PartsNum[enPARTS_READ::CORE] + m_PartsNum[enPARTS_READ::HEAD] + ( ucARM_NUM * m_PartsNum[enPARTS_READ::ARMS] ) );
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
	m_wpDevice11 = pDevice;
	m_wpCotext11 = pContext;
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
		m_hWnd, m_wpDevice11, m_wpCotext11, fileName );

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
	m_ppSkinModels[ enModel ] = new clsD3DXSKINMESH(
		m_hWnd,
		m_wpDevice11,
		m_wpCotext11,
		fileName );
//	m_ppSkinModels[ enModel ]->Init( &m_Si );
//	m_ppSkinModels[ enModel ]->CreateFromX( fileName );

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
