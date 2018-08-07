#pragma once

//#include "Global.h"

#include "Sprite2DCenter.h"
#include "UiText.h"
#include "WindowBox.h"
#include "File.h"
#include <vector>


class clsASSEMBLE_UI
{
public:
	clsASSEMBLE_UI();
	~clsASSEMBLE_UI();

	//各パーツUI数受け取り用.
	using PARTS_NUM_DATA = std::vector< int >; 

	//パーツ選択中かそれ以外か.
	enum class enSELECT_MODE : UCHAR
	{
		PARTS = 0,
		MISSION_START
	}m_enSelectMode;

	void Create( 
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext,
		PARTS_NUM_DATA data );//各パーツUIの数確定用.

	void Input();

	//第二引数はデータから、ステータス以外のデータの数。第三引数はパーツ名の番号.
	void Update( 
		enSELECT_MODE enSelect,
		std::shared_ptr< clsFILE > const spFile = nullptr,
		const int iPartsType = 0,	//パーツ種類.
		const int iPartsNum = 0,	//パーツ番号.
		const int iStatusCutNum = 0 );//ステータスじゃないデータの数.	

	void Render( 
		enSELECT_MODE enSelect, 
		const int iPartsType, 
		const int iPartsNum );//選択中パーツ番号.

	void RenderPartsState( 
		enSELECT_MODE enSelect, 
		const int iPartsType, 
		const int iPartsNum );//選択中パーツ番号.


#if _DEBUG
	//デバッグテキスト用.
	D3DXVECTOR3 GetUiPos();
#endif//#if _DEBUG

private:

	//パーツ項目数.
	enum enPARTS_TYPE : int
	{
		LEG = 0,
		CORE,
		HEAD,
		ARMS,
		WEAPON_L,
		WEAPON_R,
	
		enPARTS_TYPE_SIZE
	};

	//パーツ表示用のウィンドウ.
	D3D11_VIEWPORT m_ViewPortPartsWindow;

	std::vector< std::unique_ptr< clsSprite2D > >	m_vupPartsType;		//パーツカテゴリ.
	std::unique_ptr< clsSprite2D >					m_upPartsTypeSelect;//選択中( 半透明 ).

	std::vector< std::unique_ptr< clsSprite2D > >	m_vupPartsIcon[ enPARTS_TYPE_SIZE ];	//各パーツのUI.
	std::unique_ptr< clsSprite2D >					m_upPartsNumSelect;	//選択中( 半透明 ).

	std::unique_ptr< clsSprite2D >					m_upStatusWindow;	//ステータスが表示される.

	std::unique_ptr< clsSprite2D >					m_upPartsWindow;	//パーツの単体モデル表示される.

	std::unique_ptr< clsSprite2D >					m_upMissionStart;	//出撃ボタン.

	std::unique_ptr< clsSprite2D > m_upHeader;//画面上部の帯.

	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_pArrow;//矢印.

	std::unique_ptr< clsUiText > m_upHeaderText;//ヘッダー文字.
	std::unique_ptr< clsUiText > m_upFooterText;//フッター文字.

	std::vector< std::unique_ptr< clsUiText > > m_vupStatusText;	//ステータス文字( 項目名 ).
	std::vector< std::unique_ptr< clsUiText > > m_vupStatusNumText;	//ステータス値.


	std::unique_ptr< clsUiText > m_upPartsNameText;//パーツ名.

	//「その武器は右腕?それとも左腕?」.
	std::unique_ptr< clsWINDOW_BOX > m_upWndBox;//左右どっちか聞いてくる箱.


	//ステータスの数( 行数 ).
	int m_iStatusNum;
	//ステータスの名前を格納している.
	std::vector< std::string > m_vsStatusNameBox[enPARTS_TYPE_SIZE];

#if _DEBUG
	std::unique_ptr< clsSprite2D > m_upDegine;
#endif//#if _DEBUG
};
