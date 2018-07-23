#include "MissionModel.h"

using namespace std;

clsMISSION_MODEL::clsMISSION_MODEL()
	:m_iColNum()
{
}

clsMISSION_MODEL::~clsMISSION_MODEL()
{
}




//そのパーツの当たり判定の数を返す.
int clsMISSION_MODEL::GetColNum( const enCOL_PARTS enColParts )
{
	int iReturn = 0;

	switch( enColParts )
	{
	case enCOL_PARTS_LEG:
		break;
	case enCOL_PARTS_CORE:
		break;
	case enCOL_PARTS_HEAD:
		break;
	case enCOL_PARTS_ARMS:
		break;
	default:
		break;
	}

	return iReturn;
}

//当たり判定の座標の配列をすべて返す.
vector< D3DXVECTOR3 > clsMISSION_MODEL::GetColPosArr()
{
	vector< D3DXVECTOR3 > vvReturn;

	return vvReturn;
}

