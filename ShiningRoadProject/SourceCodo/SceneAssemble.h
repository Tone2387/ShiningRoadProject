#pragma once

#include "File.h"

#include "SceneClass.h"
#include "CameraAssemble.h"
#include "AssembleModel.h"

#include <vector>
#include <string>

//================================//
//========== 組み換えクラス ==========//
//================================//
class clsSCENE_ASSEMBLE : public clsSCENE_BASE
{
public:
	clsSCENE_ASSEMBLE( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_ASSEMBLE();

	//選択肢のあるパーツの種類( 配列の添え字になる ).
	enum enPARTS_TYPES : UCHAR
	{
		LEG = 0,
		CORE,
		HEAD,
		ARMS,
		WEAPON,

		ENUM_SIZE
	};

	//どのパーツを選んでるの?.
	struct PARTS_SELECT
	{
		int iType;	//パーツの種類( 脚、コア等 ).
		int iNum;	//パーツ番号.

		PARTS_SELECT(){
			iType = iNum = 0;
		}
	}m_PartsSelect;



private:

	void CreateProduct() final;
	void UpdateProduct( enSCENE &nextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;

#if _DEBUG
	//デバック゛テキストの表示.
	void RenderDebugText() final;
#endif//#if _DEBUG


	clsASSEMBLE_MODEL*	m_pAsmModel;

	clsFILE*	m_pFile[enPARTS_TYPES::ENUM_SIZE];

//	clsSPRITE2D_CENTER* m_pSprite;
//	clsCharaStatic* m_pTestChara;
//	clsCharaStatic* m_pParts;


};



