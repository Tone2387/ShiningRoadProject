#include "RoboStatusEnemy.h"

#include "AssembleModel.h"

#include "File.h"
#include "OperationString.h"

using namespace std;

namespace{

	const char sROBO_STATUS_ENEMY_PATH[] = "Data\\FileData\\Hiyoshi\\RoboStatusEnemy.csv";

	//�v�f����<clsSCENE_ASSEMBLE::ENUM_SIZE>.
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



//�O������f�[�^��ǂݍ���.
void clsROBO_STATUS_ENEMY::LoadFileData( const char* sFilePath )
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
		m_RoboStateData.ucPartsModelNum[i] = File.GetDataInt( m_iFILE_ROW, static_cast<int>( i ) );
	}

	for( char i=0; i<enCOLOR_GAGE_size; i++ ){
		m_RoboStateData.iColorRank[i]	= File.GetDataInt( m_iFILE_ROW, static_cast<int>( i ) + m_iFILE_INDEX_COL_COLOR_ARMOR_R );
	}
}


//�p�[�c�̃X�e�[�^�X�f�[�^��ǂݍ���.
void clsROBO_STATUS_ENEMY::LoadPartsData( vector< unique_ptr< clsFILE > >& PartsFile )
{

	//�p�[�c�̃X�e�[�^�X�ǂݍ���.
	UCHAR ucFileMax = clsASSEMBLE_MODEL::ENUM_SIZE;
	PartsFile.resize( ucFileMax ); 
	for( UCHAR i=0; i<PartsFile.size(); i++ ){
		if( PartsFile[i] != nullptr ){
			assert( !"m_spFile[i]�͍쐬�ς݂ł�" );
			continue;
		}
		PartsFile[i] = make_unique< clsFILE >();
		PartsFile[i]->Open( sPARTS_STATUS_PASS[i] );
	}


}

//�ǂݍ��񂾃f�[�^���擾����.
void clsROBO_STATUS_ENEMY::AttachData( const vector< unique_ptr< clsFILE > >& PartsFile )
{
	//�X�e�[�^�X�́ACSV������s��.
	const int iSTATUS_CUT_NUM = 2;//�ԍ��Ɩ��O.

	int PartsType = clsASSEMBLE_MODEL::LEG;

	//�����p�ϐ�.
	vector<int> tmpStatus;

//	tmpStatus.resize( PartsFile[ PartsType ]->GetSizeCol() - iSTATUS_CUT_NUM );
//	for( unsigned int i=0; i<tmpStatus.size(); i++ ){
//		//PartsFile[]�̓Y�����͂ǂ̃p�[�c���A�ł���.
//		tmpStatus[i] = PartsFile[ PartsType ]->
//			GetDataInt( m_RoboStateData.ucPartsModelNum[ PartsType ], i + iSTATUS_CUT_NUM );
//		//GetDataInt()�̑������́A���̃p�[�c���ʂ̉��Ԗڂ̍s���Q�Ƃ���΂悢�̂�.
//		//��������iSTATUS_CUT_NUM�𑫂��Ă���̂́A���̕\�ɂ���p�[�c�ԍ��Ɩ��O�͂���Ȃ�����J�b�g���邽�߂ł���.
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
	//�X�e�[�^�X�́ACSV������s��.
	const int iSTATUS_CUT_NUM = 2;//�ԍ��Ɩ��O.

	//�����p�ϐ�.
	vector<int> ReturnState;
	ReturnState.resize( PartsFile[ PartsType ]->GetSizeCol() - iSTATUS_CUT_NUM );
	for( unsigned int i=0; i<ReturnState.size(); i++ ){
		//PartsFile[]�̓Y�����͂ǂ̃p�[�c���A�ł���.
		ReturnState[i] = PartsFile[ PartsType ]->
			GetDataInt( m_RoboStateData.ucPartsModelNum[ PartsType ], i + iSTATUS_CUT_NUM );
		//GetDataInt()�̑������́A���̃p�[�c���ʂ̉��Ԗڂ̍s���Q�Ƃ���΂悢�̂�.
		//��������iSTATUS_CUT_NUM�𑫂��Ă���̂́A���̕\�ɂ���p�[�c�ԍ��Ɩ��O�͂���Ȃ�����J�b�g���邽�߂ł���.
	}

	return ReturnState;
}
