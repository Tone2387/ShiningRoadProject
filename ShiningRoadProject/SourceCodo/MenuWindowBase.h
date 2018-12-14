#ifndef MENU_WINDOW_BASE_H_
#define MENU_WINDOW_BASE_H_


class clsPOINTER_GROUP;
class clsUiText;
class clsFont;
class clsXInput;
class clsDxInput;
class clsSOUND_MANAGER_MENUWINDOW;

#include "WindowBox.h"




//窓メニューの基底クラス.
class clsMENU_WINDOW_BASE : public clsWINDOW_BOX
{
public:

	clsMENU_WINDOW_BASE(	
		const HWND hWnd,
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		std::vector<unsigned int>* const pInformationVec,
		const char* sWindowName );
	virtual ~clsMENU_WINDOW_BASE();

	//シーンクラスと情報のやり取りをする変数の型及び作成関数.
	using INFORMATION_MENU_DATA_ARRAY = std::vector<unsigned int>;
	//実働関数はこの基底クラス内のprotected:内にあり、継承先の各シーンBaseクラスで、下のコメントの形でラップする.
	//その内容は、参照するInformationのファイルパスを指定するだけ.
//	static INFORMATION_MENU_DATA_ARRAY CreateInformation( 
//		INFORMATION_MENU_DATA_ARRAY* const InformationDataArray,
//		const int iInformationSize );



public:



	void Update() final;

	void Render() final;

	//このウィンドウを閉じ始めて親ウィンドウに操作を返す.
	void Close( const float fCloseSpdRate = 8.0f );

	//このメニューウィンドウのdeleteはこのif文の中で使いましょう.
	bool isDeletePermission();

	//窓の左上を0として座標を与える.
	D3DXVECTOR3 SetPosFromWindow( const D3DXVECTOR2& vPos );


	//選択先した結果を返す.
	unsigned int GetInformation(){
		auto ReturnInformation = m_uiInformation;
		if( m_pNextWindow ){
			ReturnInformation = m_pNextWindow->GetInformation();
		}
		m_uiInformation = 0;
		return ReturnInformation;
	}


protected:

	//===== 継承先のコンストラクタで使う =====//.
	void Open( const D3DXVECTOR2& vSize );
	//===== 継承先のコンストラクタで使う =====//.


	//次の窓を吐き出す( 成功したならtrue ).
	bool CreateNextWindow( clsMENU_WINDOW_BASE** ppOutNextWindow );

	//このウィンドウを操作するようになるならtrue,離れるならfalse.
	void Operation( const bool isOperation ){ m_isOperation = isOperation; };


	//操作.
	bool SelectUp	( bool isWithStick = true );
	bool SelectDown	( bool isWithStick = true );
	bool SelectRight( bool isWithStick = true );
	bool SelectLeft	( bool isWithStick = true );
	bool SelectEnter();
	bool SelectExit();

	//シーンクラスと情報のやり取りをする変数の型及び作成関数の実働関数.
	static INFORMATION_MENU_DATA_ARRAY CreateInformationProduct( 
		INFORMATION_MENU_DATA_ARRAY* const InformationDataArray,
		const int iInformationSize,
		const char* sInformationDataPath );

protected:

	//選択肢.
	int m_iSelectNum;
	//メニューを開いたものに返す値.
	unsigned int m_uiInformation;
	//シーンからもらってくる, 情報の何が何を表すかの数値の配列.
	std::vector<unsigned int>*		m_pInformationVec;

	//文字.
	clsFont* m_wpFont;
	std::vector< std::unique_ptr< clsUiText > > m_vecupUiText;
	//選択カーソル.
	std::unique_ptr< clsSprite2D > m_upCursor;

	//操作.
	clsXInput*	m_wpXInput;
	clsDxInput* m_wpDInput;


	//次に開く窓.
	clsMENU_WINDOW_BASE* m_pNextWindow;


	//子供のために必要.
	clsPOINTER_GROUP*	m_pPtrGroup;

	HWND m_hWnd;

	//効果音.
	std::unique_ptr< clsSOUND_MANAGER_MENUWINDOW > m_upSound;

private:
	virtual void UpdateProduct() = 0;
	virtual void RenderProduct() = 0;
	virtual bool CreateNextWindowProduct( 
		clsMENU_WINDOW_BASE** ppOutNextWindow, 
		clsMENU_WINDOW_BASE* const pParentWindow ) = 0;

	void SetColor( const D3DXVECTOR3& vColor ) final;
	void SetTextAlpha( const float& fAlpha );

	//最小か( 消す許可の出るサイズか ).
	bool isMinimum();

private:

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





	//この窓を開いた窓.
	clsMENU_WINDOW_BASE* m_pParentWindow;


	//trueならこの窓を動かせる.
	bool m_isOperation;
	
	//この窓を閉じる予約.
	bool m_isClose;

};

#endif//#ifndef MENU_WINDOW_BASE_H_