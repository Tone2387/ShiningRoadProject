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



private:

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


	void CreateProduct() final;
	void UpdateProduct( enSCENE &nextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;

	//コントローラ操作.
	//カーソル移動.
	void MoveCursorUp();
	void MoveCursorDown();
	void MoveCursorRight();
	void MoveCursorLeft();
	//決定.
	void Enter();
	//戻る.
	void Undo();
	PARTS_SELECT m_OldSelect;//Undo()のために必要( のちに配列化する ).



	//範囲内に収める( パーツの選択肢がオーバーしないようにする ).
	//minはその数値より小さくならない、maxはそれ以上にはならない.
	// min <= t < max.
	template< class T, class MIN, class MAX >
	T KeepRange( T t, const MIN min, const MAX max ) const;	


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



