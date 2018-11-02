#include "PlayerRoboStatus.h"

#include "File.h"
#include "OperationString.h"

using namespace std;

namespace{

	const char sROBO_STATUS_HERO_PATH[] = "Data\\FileData\\RoboStatusCsvData\\RoboStatusHero.csv";
}

clsPLAYER_ROBO_STATUS::clsPLAYER_ROBO_STATUS()
{
	clsFILE File;
	bool isOpened = File.Open( sROBO_STATUS_HERO_PATH );

	if( !isOpened ){
		ERR_MSG( 
			sROBO_STATUS_HERO_PATH, 
			"データがありません。\
			ドライブからデータをDLして、以下のパスに入れてください" );
	}

	UCHAR tmpSize = sizeof( m_RoboStateData.ucPartsModelNum ) / sizeof( m_RoboStateData.ucPartsModelNum[0] );
	for( UCHAR i=0; i<tmpSize; i++ ){
		m_RoboStateDataHero.ucPartsModelNum[i] = File.GetDataInt( m_iFILE_VAR_ROW, static_cast<int>( i ) );
		m_RoboStateData.ucPartsModelNum[i] = m_RoboStateDataHero.ucPartsModelNum[i];
	}

	for( char i=0; i<enCOLOR_GAGE_size; i++ ){
		m_RoboStateDataHero.iColorRank[i]	= File.GetDataInt( m_iFILE_VAR_ROW, static_cast<int>( i ) + m_iFILE_INDEX_COL_COLOR_ARMOR_R );
		m_RoboStateData.iColorRank[i]		= m_RoboStateDataHero.iColorRank[i];
	}

}

clsPLAYER_ROBO_STATUS::~clsPLAYER_ROBO_STATUS()
{
}


//クリア画面で使う : タイトル用の初期化用.
void clsPLAYER_ROBO_STATUS::SaveHeroData()
{
	const string InitString;
	clsOPERATION_STRING OprtStr;
	clsFILE::FILE_DATA FileData;
	clsFILE File;
	const int iOUT_DATA_ROW_SIZE = 1 + m_iFILE_VAR_ROW;
	const int iOUT_DATA_COL_SIZE = 1 + m_iFILE_INDEX_COL_COLOR_BASE_B;
	File.CreateFileDataForOutPut( FileData, iOUT_DATA_ROW_SIZE, iOUT_DATA_COL_SIZE );

	int iFileDataIndex = 0;

	UCHAR tmpSize = sizeof( m_RoboStateData.ucPartsModelNum ) / sizeof( m_RoboStateData.ucPartsModelNum[0] );
	for( UCHAR i=0; i<tmpSize; i++ ){
		m_RoboStateDataHero.ucPartsModelNum[i] = m_RoboStateData.ucPartsModelNum[i];
		FileData[0][ iFileDataIndex++ ] = OprtStr.ConsolidatedNumber( InitString, m_RoboStateDataHero.ucPartsModelNum[i] );
	}
	for( char i=0; i<enCOLOR_GAGE_size; i++ ){
		m_RoboStateDataHero.iColorRank[i] = m_RoboStateData.iColorRank[i];
		FileData[0][ iFileDataIndex++ ] = OprtStr.ConsolidatedNumber( InitString, m_RoboStateDataHero.iColorRank[i] );
	}


	File.Open( sROBO_STATUS_HERO_PATH );
	File.OutPutCsv( FileData );
	File.Close();
}

//AssembleModelでのタイトル画面での初期化でAssembleModelのInitの前に使う.
void clsPLAYER_ROBO_STATUS::LodeHeroData()
{
	int tmpSize = sizeof( m_RoboStateData.ucPartsModelNum ) / sizeof( m_RoboStateData.ucPartsModelNum[0] );
	for( int i=0; i<tmpSize; i++ ){
		m_RoboStateData.ucPartsModelNum[i] = m_RoboStateDataHero.ucPartsModelNum[i];
	}
	for( char i=0; i<enCOLOR_GAGE_size; i++ ){
		m_RoboStateData.iColorRank[i] = m_RoboStateDataHero.iColorRank[i];
	}

}

