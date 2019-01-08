#include "RoboStatusPlayer.h"

#include "File.h"
#include "OperationString.h"

using namespace std;

namespace{

	const char sROBO_STATUS_HERO_PATH[] = "Data\\FileData\\RoboStatusCsvData\\RoboStatusHero.csv";

}

clsROBO_STATUS_PLAYER::clsROBO_STATUS_PLAYER()
{
	LoadFileData( sROBO_STATUS_HERO_PATH );
	vector< unique_ptr< clsFILE > > vecupFile;
	LoadPartsData( vecupFile );
	AttachData( vecupFile );
}

clsROBO_STATUS_PLAYER::~clsROBO_STATUS_PLAYER()
{
}


//�O������f�[�^��ǂݍ���.
void clsROBO_STATUS_PLAYER::LoadFileData( const char* sFilePath )
{
	clsFILE File;
	bool isOpened = File.Open( sFilePath );

	if( !isOpened ){
		ERR_MSG( 
			sFilePath, 
			"�f�[�^������܂���B\
			�h���C�u����f�[�^��DL���āA�ȉ��̃p�X�ɓ���Ă�������" );
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






//�N���A��ʂŎg�� : �^�C�g���p�̏������p�̃f�[�^��p�ӂ��� : �N���A�������{���o���Ă���.
void clsROBO_STATUS_PLAYER::SaveHeroData()
{
	const string InitString;
	const int iOUT_DATA_ROW_SIZE = 1 + m_iFILE_VAR_ROW;
	const int iOUT_DATA_COL_SIZE = 1 + m_iFILE_INDEX_COL_COLOR_BASE_B;

	clsOPERATION_STRING OprtStr;
	//�o�͗p.
	clsFILE File;
	clsFILE::FILE_DATA FileData;

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


	//�t�@�C���o��.
	File.Open( sROBO_STATUS_HERO_PATH );
	File.OutPutCsv( FileData );
	File.Close();
}

//AssembleModel�ł̃^�C�g����ʂł̏�������AssembleModel��Init�̑O�Ɏg��.
void clsROBO_STATUS_PLAYER::LodeHeroData()
{
	int tmpSize = sizeof( m_RoboStateData.ucPartsModelNum ) / sizeof( m_RoboStateData.ucPartsModelNum[0] );
	for( int i=0; i<tmpSize; i++ ){
		m_RoboStateData.ucPartsModelNum[i] = m_RoboStateDataHero.ucPartsModelNum[i];
	}
	for( char i=0; i<enCOLOR_GAGE_size; i++ ){
		m_RoboStateData.iColorRank[i] = m_RoboStateDataHero.iColorRank[i];
	}

}


