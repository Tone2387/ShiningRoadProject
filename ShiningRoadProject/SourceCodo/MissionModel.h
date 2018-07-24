#pragma once 


#include "AssembleModel.h"


#define sBONE_NAME_COL_JOINT	"Joint"//00〜xx.

#define sBONE_NAME_COL_CORE "Jenerator"//コアの中心.
#define sBONE_NAME_COL_HEAD "Center"//頭の中心.




class clsMISSION_MODEL : public clsASSEMBLE_MODEL
{
public:
	clsMISSION_MODEL();
	~clsMISSION_MODEL();

	//ボーンに必要な情報.
	struct BONE_SET
	{
		std::string sName;
		D3DXVECTOR3 vPos;
		int			iParts;//何のパーツか(enPARTSをキャストしてこの変数に入れる).
	};

	enum enCOL_PARTS : UCHAR
	{
		enCOL_PARTS_LEG = 0,
		enCOL_PARTS_CORE,
		enCOL_PARTS_HEAD,
		enCOL_PARTS_ARMS,

		enCOL_PARTS_size
	};

	//そのパーツの当たり判定の数を返す.
	int GetColNum( const enCOL_PARTS enColParts );

	//ボーンの数を確定する( GetColNum関数の後に使う ).
	void FixBoneStates();

	//当たり判定の座標の配列をすべて返す.
	std::vector< D3DXVECTOR3 > GetColPosArr();

private:

	std::vector< BONE_SET > m_vColStates;

	int m_iColNum[enCOL_PARTS_size];//各パーツの当たり判定の数.

	int m_iColMax;//当たり判定の合計数.

};

