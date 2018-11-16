#ifndef ASSENBLE_UI_H_
#define ASSENBLE_UI_H_

//#include "Global.h"
#include "DxInput.h"
#include "CXInput.h"

#include "Sprite2DCenter.h"
#include "UiText.h"
#include "File.h"
#include "CFont.h"
#include "AssembleModel.h"
#include <vector>
class clsWINDOW_BOX;


class clsASSEMBLE_UI
{
public:
	clsASSEMBLE_UI( clsFont* const pFont );
	~clsASSEMBLE_UI();

	//各パーツUI数受け取り用.
	using PARTS_NUM_DATA = std::vector< int >; 

	//パーツ選択中かそれ以外か.
	enum class enSELECT_MODE : UCHAR
	{
		PARTS = 0,
		STATUS,
		MISSION_START,
		TITLE_BACK,
		COLOR_CHANGE,
	}	m_enSelectMode;

	void Create( 
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext,
		PARTS_NUM_DATA data );//各パーツUIの数確定用.

	void Input(
		const clsXInput* const pXInput,
		const clsDxInput* const pDxInput );

	//第二引数はデータから、ステータス以外のデータの数。第三引数はパーツ名の番号.
	void Update( 
		enSELECT_MODE enSelect,
		std::shared_ptr< clsFILE > const spFile = nullptr,
		clsASSEMBLE_MODEL* const pModel = nullptr,
		const int iPartsType = 0,	//パーツ種類.
		const int iPartsNum = 0,	//パーツ番号.
		const int iStatusCutNum = 0 );//ステータスじゃないデータの数.	

	void Render( 
		enSELECT_MODE enSelect, 
		const int iPartsType, 
		const int iPartsNum ) const;//選択中パーツ番号.

	void RenderPartsState( 
		enSELECT_MODE enSelect, 
		const int iPartsType, 
		const int iPartsNum ) const;//選択中パーツ番号.

	
	//説明文の行指定.
	int SetReadLinePartsComment(
		const int iPartsType );	//パーツ種類.


	//ステータスウィンドウを隠す.
	void SwitchDispStatusComment();
	//ステータスcommentの切り替え許可.
	bool isCanSwitchStatusComment();

	//ステータス詳細とパーツ選択の切り替え.
	void SwitchStatusComment();
	//指定.
	void SetStatusComment( const enSELECT_MODE enMode );
	//.
	void AddStatusCommentNo( const bool isPlus );
	//ゴリ押し気味.
	//選択肢を横に持って行った時の調整.
	void AddCommentNoForChangePartsType( const int iPartsType );

	//パーツ説明文の表示文字数を0に戻す.
	void InitReadNumPartsComment(){ m_iReadNumPartsComment = 0; }

#if _DEBUG
	//デバッグテキスト用.
	D3DXVECTOR3 GetUiPos();
#endif//#if _DEBUG

private:


	D3DXVECTOR4 GetStatusColor( 
		const int iBefore, const int iAfter,
		const int iPartsType, const int iStatusNum ) const;


	//超えていたなら収める.
	void StatusNumKeepRange();
	//超えていたならループする.
	void StatusNumLoopRange();

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

	std::vector< std::unique_ptr< clsSprite2D > >	m_vecupPartsType;		//パーツカテゴリ.
	std::unique_ptr< clsSprite2D >					m_upPartsTypeSelect;//選択中( 半透明 ).

	std::vector< std::unique_ptr< clsSprite2D > >	m_vecupPartsIconArray[ enPARTS_TYPE_SIZE ];	//各パーツのUI.
	std::unique_ptr< clsSprite2D >					m_upPartsNumSelect;	//選択中( 半透明 ).

	std::unique_ptr< clsSprite2D >					m_upStatusWindow;	//ステータスが表示される.

	std::unique_ptr< clsSprite2D >					m_upPartsWindow;	//パーツの単体モデル表示される.
	std::unique_ptr< clsSprite2D >					m_upRoboWindow;		//ロボ全体が表示される.

	std::unique_ptr< clsSprite2D > m_upHeader;//画面上部の帯.
	std::unique_ptr< clsSprite2D > m_upFooter;//画面下部の帯.

	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vecupArrow;//矢印.

	std::unique_ptr< clsUiText > m_upHeaderText;//ヘッダー文字.

	std::vector< std::unique_ptr< clsUiText > > m_vecupStatusText;	//ステータス文字( 項目名 ).
	std::vector< std::unique_ptr< clsUiText > > m_vecupStatusNumText;	//今見ているパーツのステータス値.
	std::vector< std::unique_ptr< clsUiText > > m_vecupStatusNumTextNow;	//現在の装備のステータス値.


	std::unique_ptr< clsUiText > m_upPartsNameText;//パーツ名.

	std::unique_ptr< clsSprite2D > m_upButton;


	//「その武器は右腕?それとも左腕?」.
	std::unique_ptr< clsWINDOW_BOX > m_upWndBox;//左右どっちか聞いてくる箱.


	//ステータスの数( 行数 ).
	int m_iStatusNum;
	//ステータスの名前を格納している.
	std::vector< std::string > m_vecsStatusNameBoxArray[ enPARTS_TYPE_SIZE ];

	//ステータスの表示フラグ.
	bool	m_isDispStatus;

	//選択しているステータスのNo.
	int		m_iStatusCommentNo;
	//それを示すUI.
	std::unique_ptr< clsSprite2D > m_upSelectStatus;
	//調整用フラグ.
	bool	m_bStatusCommentOffset;

	//日本語フォント.
	clsFont*	m_wpFont;
	//パーツ、ステータスの日本語説明文の読み込み行指定.
	int m_iReadLinePartsComment;
	//説明文を何文字目まで表示するか.
	int m_iReadNumPartsComment;


#if _DEBUG
	std::unique_ptr< clsSprite2D > m_upDegine;
#endif//#if _DEBUG

};

#endif//#ifndef ASSENBLE_UI_H_