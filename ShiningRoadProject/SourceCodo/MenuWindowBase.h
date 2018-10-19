#ifndef MENU_WINDOW_BASE_H_
#define MENU_WINDOW_BASE_H_

class clsPOINTER_GROUP;
class clsUiText;
class clsFont;

#include "WindowBox.h"


//窓メニューの基底クラス.
class clsMENU_WINDOW_BASE : public clsWINDOW_BOX
{
public:

	clsMENU_WINDOW_BASE(		
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		const D3DXVECTOR2& vSize );

	~clsMENU_WINDOW_BASE();

	void Update() final;

	void Render() final;

	//操作.
	virtual bool SelectUp() = 0;
	virtual bool SelectDown() = 0;
	virtual bool SelectRight() = 0;
	virtual bool SelectLeft() = 0;
	virtual bool SelectEnter() = 0;
	virtual bool SelectExit() = 0;



protected:

	//次の窓を吐き出す( 成功したならtrue ).
	bool CreateNextWindow( clsMENU_WINDOW_BASE** ppOutNextWindow );

	//このウィンドウを操作するようになるならtrue,離れるならfalse.
	void Operation( const bool isOperation );

	//このウィンドウを閉じて親ウィンドウに操作を返す.
	void Close();

private:
	virtual void UpdateProduct() = 0;
	virtual void RenderProduct() = 0;
	virtual bool CreateNextWindowProduct( 
		clsMENU_WINDOW_BASE** ppOutNextWindow, 
		clsMENU_WINDOW_BASE* const pParentWindow ) = 0;



	void SetColor( const D3DXVECTOR3& vColor ) final;

	//選択カーソル.
	std::unique_ptr< clsSprite2D > m_upCursor;

	//この窓を開いた窓.
	clsMENU_WINDOW_BASE* m_pParentWindow;
	//次に開く窓.
	clsMENU_WINDOW_BASE* m_pNextWindow;


	//trueならこの窓を動かせる.
	bool m_isOperation;

	//選択肢.
	int m_iSelectNum;


	//表示.
	clsFont* m_wpFont;
	std::vector< std::unique_ptr< clsUiText > > m_vecupUiText;

};

#endif//#ifndef MENU_WINDOW_BASE_H_