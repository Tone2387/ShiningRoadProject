#include "RoboStatusEnemy.h"


#include "File.h"
#include "OperationString.h"

using namespace std;

namespace{
	const char sROBO_STATUS_ENEMY_PATH[] = "Data\\FileData\\Hiyoshi\\RoboStatusEnemy.csv";
}


clsROBO_STATUS_ENEMY::clsROBO_STATUS_ENEMY( const int iRow )
	:m_iFILE_ROW( iRow )
{
	LoadFileData( sROBO_STATUS_ENEMY_PATH );
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


