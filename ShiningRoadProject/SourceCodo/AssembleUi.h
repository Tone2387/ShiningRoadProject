#pragma once

//#include "Global.h"

#include "Sprite2DCenter.h"
#include "UiText.h"
#include "File.h"
#include <vector>

class clsASSEMBLE_UI
{
public:
	clsASSEMBLE_UI();
	~clsASSEMBLE_UI();

	//各パーツUI数受け取り用.
	using PARTS_NUM_DATA = std::vector< int >; 

	void Create( 
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext,
		PARTS_NUM_DATA data );//各パーツUIの数確定用.

	void Input();

	//第二引数はデータから、ステータス以外のデータの数。第三引数はパーツ名の番号.
	void Update( 
		std::shared_ptr< clsFILE > const spFile,
		const int iPartsType,	//パーツ種類.
		const int iPartsNum,	//パーツ番号.
		const int iStatusCutNum );//ステータスじゃないデータの数.	

	void Render( const int iPartsType, const int iPartsNum );//選択中パーツ番号.

#if _DEBUG
	//デバッグテキスト用.
	D3DXVECTOR3 GetUiPos();
#endif//#if _DEBUG

private:

	enum enPARTS_TYPE : int
	{
		LEG = 0,
		CORE,
		HEAD,
		ARMS,
		WEAPON,
	
		enPARTS_TYPE_SIZE
	};

	std::vector< std::unique_ptr< clsSprite2D > >	m_vupPartsType;		//パーツカテゴリ.
	std::unique_ptr< clsSprite2D >					m_upPartsTypeSelect;//選択中( 半透明 ).
	std::vector< std::unique_ptr< clsSprite2D > >	m_vupPartsIcon[ enPARTS_TYPE_SIZE ];	//各パーツのUI.
	std::unique_ptr< clsSprite2D >					m_upPartsNumSelect;	//選択中( 半透明 ).
	std::unique_ptr< clsSprite2D >					m_upStatusWindow;	//ステータスが表示される.
	std::unique_ptr< clsSprite2D >					m_upPartsWindow;	//パーツの単体モデル表示される.

	std::unique_ptr< clsSprite2D > m_upHeader;//画面上部の帯.
	std::unique_ptr< clsSprite2D > m_upFooter;//画面下部の帯.

	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_pArrow;//矢印.

	std::unique_ptr< clsUiText > m_upHeaderText;//ヘッダー文字.
	std::unique_ptr< clsUiText > m_upFooterText;//フッター文字.

	std::unique_ptr< clsUiText >				m_upStatusTitleText;//ステータスのタイトル.
	std::vector< std::unique_ptr< clsUiText > > m_vupStatusText;	//ステータス文字( 項目名 ).
	std::vector< std::unique_ptr< clsUiText > > m_vupStatusNumText;	//ステータス値.


	std::unique_ptr< clsUiText > m_upPartsNameText;//パーツ名.


	//ステータスの数( 行数 ).
	int m_iStatusNum;
	//ステータスの名前を格納している.
	std::vector< std::string > m_vsStatusNameBox[enPARTS_TYPE_SIZE];

#if _DEBUG
	std::unique_ptr< clsSprite2D > m_upDegine;
#endif//#if _DEBUG
};
