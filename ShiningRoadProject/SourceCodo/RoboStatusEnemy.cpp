#include "RoboStatusEnemy.h"

#include "AssembleModel.h"

#include "File.h"
#include "OperationString.h"

using namespace std;

namespace{

	const char sROBO_STATUS_ENEMY_PATH[] = "Data\\FileData\\Hiyoshi\\RoboStatusEnemy.csv";

	//要素数は<clsSCENE_ASSEMBLE::ENUM_SIZE>.
	const string sPARTS_STATUS_PASS[] =
	{
		"Data\\RoboParts\\Leg\\RoboPartsData.csv",
		"Data\\RoboParts\\Core\\RoboPartsData.csv",
		"Data\\RoboParts\\Head\\RoboPartsData.csv",
		"Data\\RoboParts\\Arms\\RoboPartsData.csv",
		"Data\\RoboParts\\Weapon\\RoboPartsData.csv",
		"Data\\RoboParts\\Weapon\\RoboPartsData.csv",
	};
}


clsROBO_STATUS_ENEMY::clsROBO_STATUS_ENEMY( const int iRow )
	:m_iFILE_ROW( iRow )
{
	vector< unique_ptr< clsFILE > > vecupFile;
	LoadFileData( sROBO_STATUS_ENEMY_PATH );
	LoadPartsData( vecupFile );
	AttachData( vecupFile );
}

clsROBO_STATUS_ENEMY::~clsROBO_STATUS_ENEMY()
{
}



//外部からデータを読み込む.
void clsROBO_STATUS_ENEMY::LoadFileData( const char* sFilePath )
{
	clsFILE File;
	bool isOpened = File.Open( sFilePath );

	if( !isOpened ){
		ERR_MSG( 
			sFilePath, 
			"データがありません。\
			ドライブからデータをDLして、以下のパスに入れてください" );
	}

	UCHAR tmpSize = sizeof( m_RoboStateData.ucPartsModelNum ) / sizeof( m_RoboStateData.ucPartsModelNum[0] );
	for( UCHAR i=0; i<tmpSize; i++ ){
		m_RoboStateData.ucPartsModelNum[i] = File.GetDataInt( m_iFILE_ROW, static_cast<int>( i ) );
	}

	for( char i=0; i<enCOLOR_GAGE_size; i++ ){
		m_RoboStateData.iColorRank[i]	= File.GetDataInt( m_iFILE_ROW, static_cast<int>( i ) + m_iFILE_INDEX_COL_COLOR_ARMOR_R );
	}
}


//パーツのステータスデータを読み込む.
void clsROBO_STATUS_ENEMY::LoadPartsData( vector< unique_ptr< clsFILE > >& PartsFile )
{

	//パーツのステータス読み込み.
	UCHAR ucFileMax = clsASSEMBLE_MODEL::ENUM_SIZE;
	PartsFile.resize( ucFileMax ); 
	for( UCHAR i=0; i<PartsFile.size(); i++ ){
		if( PartsFile[i] != nullptr ){
			assert( !"m_spFile[i]は作成済みです" );
			continue;
		}
		PartsFile[i] = make_unique< clsFILE >();
		PartsFile[i]->Open( sPARTS_STATUS_PASS[i] );
	}


}

//読み込んだデータを取得する.
void clsROBO_STATUS_ENEMY::AttachData( const vector< unique_ptr< clsFILE > >& PartsFile )
{
	//ステータスの、CSVから削る行数.
	const int iSTATUS_CUT_NUM = 2;//番号と名前.

	int PartsType = clsASSEMBLE_MODEL::LEG;

	//引数用変数.
	vector<int> tmpStatus;

//	tmpStatus.resize( PartsFile[ PartsType ]->GetSizeCol() - iSTATUS_CUT_NUM );
//	for( unsigned int i=0; i<tmpStatus.size(); i++ ){
//		//PartsFile[]の添え字はどのパーツか、である.
//		tmpStatus[i] = PartsFile[ PartsType ]->
//			GetDataInt( m_RoboStateData.ucPartsModelNum[ PartsType ], i + iSTATUS_CUT_NUM );
//		//GetDataInt()の第一引数は、そのパーツ部位の何番目の行を参照すればよいのか.
//		//第二引数でiSTATUS_CUT_NUMを足しているのは、元の表にあるパーツ番号と名前はいらないからカットするためである.
//	}

	tmpStatus = CreateDataForReceive( PartsFile, PartsType );
	this->ReceiveLeg( tmpStatus,		m_RoboStateData.ucPartsModelNum[ PartsType ++ ] );

	tmpStatus = CreateDataForReceive( PartsFile, PartsType );
	this->ReceiveCore( tmpStatus,		m_RoboStateData.ucPartsModelNum[ PartsType ++ ] );

	tmpStatus = CreateDataForReceive( PartsFile, PartsType );
	this->ReceiveHead( tmpStatus,		m_RoboStateData.ucPartsModelNum[ PartsType ++ ] );

	tmpStatus = CreateDataForReceive( PartsFile, PartsType );
	this->ReceiveArms( tmpStatus,		m_RoboStateData.ucPartsModelNum[ PartsType ++ ] );

	tmpStatus = CreateDataForReceive( PartsFile, PartsType );
	this->ReceiveWeaponL( tmpStatus,	m_RoboStateData.ucPartsModelNum[ PartsType ++ ] );

	tmpStatus = CreateDataForReceive( PartsFile, PartsType );
	this->ReceiveWeaponR( tmpStatus,	m_RoboStateData.ucPartsModelNum[ PartsType ++ ] );


//	m_spAsmModel->AttachModel( enPARTS::LEG,	tmpPartsNum );
//	m_spAsmModel->AttachModel( enPARTS::CORE,	tmpPartsNum );
//	m_spAsmModel->AttachModel( enPARTS::HEAD,	tmpPartsNum );
//	m_spAsmModel->AttachModel( enPARTS::ARM_L,	tmpPartsNum );
//	m_spAsmModel->AttachModel( enPARTS::ARM_R,	tmpPartsNum );
//	m_spAsmModel->AttachModel( enPARTS::WEAPON_L,tmpPartsNum );
//	m_spAsmModel->AttachModel( enPARTS::WEAPON_R,tmpPartsNum );



}

vector<int> clsROBO_STATUS_ENEMY::CreateDataForReceive( 
	const vector< unique_ptr< clsFILE > >& PartsFile,
	const int PartsType )
{
	//ステータスの、CSVから削る行数.
	const int iSTATUS_CUT_NUM = 2;//番号と名前.

	//引数用変数.
	vector<int> ReturnState;
	ReturnState.resize( PartsFile[ PartsType ]->GetSizeCol() - iSTATUS_CUT_NUM );
	for( unsigned int i=0; i<ReturnState.size(); i++ ){
		//PartsFile[]の添え字はどのパーツか、である.
		ReturnState[i] = PartsFile[ PartsType ]->
			GetDataInt( m_RoboStateData.ucPartsModelNum[ PartsType ], i + iSTATUS_CUT_NUM );
		//GetDataInt()の第一引数は、そのパーツ部位の何番目の行を参照すればよいのか.
		//第二引数でiSTATUS_CUT_NUMを足しているのは、元の表にあるパーツ番号と名前はいらないからカットするためである.
	}

	return ReturnState;
}
