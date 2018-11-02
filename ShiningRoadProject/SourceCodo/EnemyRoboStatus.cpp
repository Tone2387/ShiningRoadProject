#include "EnemyRoboStatus.h"


#include "File.h"
#include "OperationString.h"

using namespace std;

namespace{

	const char sROBO_STATUS_ENEMY_PATH[] = "Data\\FileData\\Hiyoshi\\RoboStatusEnemy.csv";
}


clsENEMY_ROBO_STATUS::clsENEMY_ROBO_STATUS()
{
	LoadFileData( sROBO_STATUS_ENEMY_PATH );
}

clsENEMY_ROBO_STATUS::~clsENEMY_ROBO_STATUS()
{
}



//外部からデータを読み込む.
void clsENEMY_ROBO_STATUS::LoadFileData( const char* sFilePath )
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
		m_RoboStateData.ucPartsModelNum[i] = File.GetDataInt( m_iFILE_VAR_ROW, static_cast<int>( i ) );
	}

	for( char i=0; i<enCOLOR_GAGE_size; i++ ){
		m_RoboStateData.iColorRank[i]	= File.GetDataInt( m_iFILE_VAR_ROW, static_cast<int>( i ) + m_iFILE_INDEX_COL_COLOR_ARMOR_R );
	}
}


