#ifndef MENU_WINDOW_BASE_H_
#define MENU_WINDOW_BASE_H_

class clsPOINTER_GROUP;
class clsUiText;
class clsFont;
class clsXInput;
class clsDxInput;
class clsSOUND_MANAGER_BASE;

#include "WindowBox.h"


//窓メニューの基底クラス.
class clsMENU_WINDOW_BASE : public clsWINDOW_BOX
{
public:

	clsMENU_WINDOW_BASE(		
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		unsigned int* const pInformationArray );
	virtual ~clsMENU_WINDOW_BASE();

	//このメニューウィンドウのdeleteはこのif文の中で使いましょう.
	bool isDeletePermission();

protected:
	//継承先のコンストラクタで使う.
	void Open( const D3DXVECTOR2& vSize );
public:


	void Update() final;

	void Render() final;

	//このウィンドウを閉じて親ウィンドウに操作を返す.
	void Close();

	//選択先した結果を返す.
	unsigned int GetInformation(){
		if( m_pNextWindow ){
			m_pNextWindow->GetInformation();
		}
		return m_uiInformation;
	}


protected:

	//次の窓を吐き出す( 成功したならtrue ).
	bool CreateNextWindow( clsMENU_WINDOW_BASE** ppOutNextWindow );

	//このウィンドウを操作するようになるならtrue,離れるならfalse.
	void Operation( const bool isOperation );


	//操作.
	bool SelectUp();
	bool SelectDown();
	bool SelectRight();
	bool SelectLeft();
	bool SelectEnter();
	bool SelectExit();

	//選択肢.
	int m_iSelectNum;
	//メニューを開いたものに返す値.
	unsigned int m_uiInformation;
	//シーンからもらってくる, 情報の何が何を表すかの数値の配列.
	unsigned int* m_puiInformationDataArray;

private:
	virtual void UpdateProduct() = 0;
	virtual void RenderProduct() = 0;
	virtual bool CreateNextWindowProduct( 
		clsMENU_WINDOW_BASE** ppOutNextWindow, 
		clsMENU_WINDOW_BASE* const pParentWindow ) = 0;

	void SetColor( const D3DXVECTOR3& vColor ) final;

	//カーソル移動に必要.
	struct HOLD_STATE
	{
		int iHoldFream;
		int iFirstPush;//最初の一歩.
		HOLD_STATE()
		:iHoldFream( 0 )
		,iFirstPush( 0 )
		{}
	};
	HOLD_STATE m_HoldRight;
	HOLD_STATE m_HoldLeft;
	HOLD_STATE m_HoldUp;
	HOLD_STATE m_HoldDown;




	//選択カーソル.
	std::unique_ptr< clsSprite2D > m_upCursor;

	//この窓を開いた窓.
	clsMENU_WINDOW_BASE* m_pParentWindow;
	//次に開く窓.
	clsMENU_WINDOW_BASE* m_pNextWindow;


	//trueならこの窓を動かせる.
	bool m_isOperation;




	//表示.
	clsFont* m_wpFont;
	std::vector< std::unique_ptr< clsUiText > > m_vecupUiText;

	//操作.
	clsXInput*	m_wpXInput;
	clsDxInput* m_wpDInput;

	clsSOUND_MANAGER_BASE* m_wpSound;
};

#endif//#ifndef MENU_WINDOW_BASE_H_