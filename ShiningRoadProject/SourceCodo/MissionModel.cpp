#include "MissionModel.h"
#include "OperationString.h"


using namespace std;

//ジョイントボーンの数字の桁数.
const char cBONE_NAME_NUM_DIGIT_JOINT = 2;

//当たり判定.
const int iRESURVE_SIZE_COL = 64;

clsMISSION_MODEL::clsMISSION_MODEL()
	:m_iColNum()
	,m_iColMax( 0 )
	,m_iColStateIndex( 0 )
{
	m_vColStates.reserve( iRESURVE_SIZE_COL );
}

clsMISSION_MODEL::~clsMISSION_MODEL()
{
}

void clsMISSION_MODEL::CreateProduct()
{
	CreateColStateBones();
}


int clsMISSION_MODEL::CreateColStateBones()
{
	int iReturn = 0;
	iReturn += CreateColStateBone( clsMISSION_MODEL::enCOL_PARTS_LEG );
	iReturn += CreateColStateBone( clsMISSION_MODEL::enCOL_PARTS_CORE );
	iReturn += CreateColStateBone( clsMISSION_MODEL::enCOL_PARTS_HEAD );
	iReturn += CreateColStateBone( clsMISSION_MODEL::enCOL_PARTS_ARMS );

	return iReturn;
}



//そのパーツの当たり判定の数を返す.
int clsMISSION_MODEL::CreateColStateBone( const enCOL_PARTS enColParts )
{
	int iReturn = 0;			//戻り値.
	clsOPERATION_STRING OprtStr;//ボーン名と番号を繋げる役割.
	int iColNum = 0;			//m_iColNumに格納する当たり判定の数.
	int tmpIndex;				//m_vpPartsの添え字.
	const BONE_SET INIT_BONE_SET;//初期化用.
	 
	switch( enColParts )
	{
	case enCOL_PARTS_LEG:
		tmpIndex = static_cast< int >( enPARTS::LEG );
		//ボーンがあるだけ繰り返し.
		while( m_vpParts[ tmpIndex ]->ExistsBone( OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT ).c_str() ) )
		{
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[ m_iColStateIndex ].sName = OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT );

			m_iColStateIndex ++;	
			iColNum ++;
		}
		m_iColNum[ enColParts ] = iReturn = iColNum;
		break;
	case enCOL_PARTS_CORE:
		tmpIndex = static_cast< int >( enPARTS::CORE );
		if( m_vpParts[ tmpIndex ]->ExistsBone( sBONE_NAME_COL_CORE ) ){
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[ m_iColStateIndex ].sName = sBONE_NAME_COL_CORE;

			m_iColStateIndex ++;	
			m_iColNum[ enColParts ] = iReturn = 1;
		}
		break;
	case enCOL_PARTS_HEAD:
		tmpIndex = static_cast< int >( enPARTS::HEAD );
		if( m_vpParts[ tmpIndex ]->ExistsBone( sBONE_NAME_COL_HEAD ) ){
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[ m_iColStateIndex ].sName = sBONE_NAME_COL_HEAD;

			m_iColStateIndex ++;	
			m_iColNum[ enColParts ] = iReturn = 1;
		}
		break;
	case enCOL_PARTS_ARMS:
		//左腕.
		tmpIndex = static_cast< int >( enPARTS::ARM_L );
		while( m_vpParts[ tmpIndex ]->ExistsBone( OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT ).c_str() ) )
		{
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[ m_iColStateIndex ].sName = OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT );

			m_iColStateIndex ++;	
			iColNum ++;
		}
		m_iColNum[ enColParts ] = iReturn = iColNum;

		//右腕.
		tmpIndex = static_cast< int >( enPARTS::ARM_R );
		iColNum = 0;
		while( m_vpParts[ tmpIndex ]->ExistsBone( OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT ).c_str() ) )
		{
			m_vColStates.push_back( INIT_BONE_SET );
			m_vColStates[ m_iColStateIndex ].iParts = tmpIndex;
			m_vColStates[ m_iColStateIndex ].sName = OprtStr.ConsolidatedNumber( sBONE_NAME_COL_JOINT, iColNum, cBONE_NAME_NUM_DIGIT_JOINT );

			m_iColStateIndex ++;	
			iColNum ++;
		}
		m_iColNum[ enColParts ] = iReturn += iColNum;
		break;
	default:
		return -1;
	}

	return iReturn;
}


//ボーンの数を確定する.
void clsMISSION_MODEL::FixBoneStates()
{
	m_vColStates.shrink_to_fit();
	m_iColMax = 0;
	for( UCHAR i=0; i<enCOL_PARTS_size; i++ ){
		m_iColMax += m_iColNum[i];
	}

	m_vvColPos.resize( m_iColMax );
}

//各パーツの当たり判定の数を返す.
int clsMISSION_MODEL::GetColNum( const enCOL_PARTS enColParts )
{
	switch( enColParts )
	{
	case enCOL_PARTS_LEG:
	case enCOL_PARTS_CORE:
	case enCOL_PARTS_HEAD:
	case enCOL_PARTS_ARMS:
		return m_iColNum[ enColParts ];
	}

	return -1;
}


//当たり判定の座標の配列をすべて返す.
shared_ptr< vector< D3DXVECTOR3 > > clsMISSION_MODEL::GetColPosPtr()
{
//	vector< D3DXVECTOR3 > vvReturn;
//	vvReturn.resize( m_iColMax );

	for( int i=0; i<m_iColMax; i++ ){
		//ボーンの座標を取得.
		m_vColStates[i].vPos = GetBonePos( 
			static_cast<enPARTS>( m_vColStates[i].iParts ), 
			m_vColStates[i].sName.c_str() );
		
		m_vvColPos[i] = m_vColStates[i].vPos;
	}

	shared_ptr< vector< D3DXVECTOR3 > > spvvReturn( &m_vvColPos );

	return spvvReturn;
}

