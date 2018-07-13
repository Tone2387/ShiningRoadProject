#pragma once

#include "File.h"

#include "SceneBase.h"
#include "CameraAssemble.h"
#include "AssembleModel.h"

#include "AssembleUi.h"

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
		WEAPON_L,
		WEAPON_R,

		ENUM_SIZE
	};

	//どのパーツを選んでるの?.
	struct PARTS_SELECT
	{
		short Type;	//パーツの種類( 脚、コア等 ).
		short Num[ENUM_SIZE];	//パーツ番号.

		PARTS_SELECT()
		:Num()
		{
			Type = 0;
		}
	}m_PartsSelect;


	void CreateProduct() final;
	void UpdateProduct( enSCENE &enNextScene ) final;
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

	//お着換えするモデル.
	clsASSEMBLE_MODEL*	m_pAsmModel;

	//UI.
	clsASSEMBLE_UI*		m_pUI;

	std::vector< std::shared_ptr< clsFILE > >	m_vspFile;
	UCHAR										m_cuFileMax;

//	clsSPRITE2D_CENTER* m_pSprite;
//	clsCharaStatic* m_pTestChara;
//	clsCharaStatic* m_pParts;



	//エフェクト.
	::Effekseer::Handle m_ehHibana;


	//音の引数.
	enum enBGM : int
	{
		ASSEMBLE = 0,
	};

	enum enSE : int
	{
		CURSOL_MOVE = 0,
		ENTER,
		EXIT,
		MISSION_START,
	};

};



