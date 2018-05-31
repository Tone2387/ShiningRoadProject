#ifndef RESOURCE_H_
#define RESOURCE_H_

#define _CRT_SECURE_NO_WARNINGS

const int iTEST_ROBO_PARTS_MODEL_MAX = 1;//テスト中のパーツ最大数.

/*
//テストモデルに足の3番のモデルを割り当てる例.
	m_pMesh->AttachModel(
		m_wpResource->GetPartsModels( enPARTS::LEG, 3 ) );


//今まで.
	m_pMesh->AttachModel(
		m_wpResource->GetSkinModels( clsResource::enSkinModel_Leg ) );
*/

#include "Global.h"
#include "DX9Mesh.h"
#include "CD3DXSKINMESH.h"


//スキンメッシュ列挙体の型.
#define SKIN_ENUM_TYPE UCHAR

//シングルトンの時はつける.
//#define RESOURCE_CLASS_SINGLETON


//3Dモデルのもとデータを格納するクラス.
//シングルトンにしてどこからでも参照できる.
class clsResource
{
public:
	//スタティックモデル種類.
	enum enSTATIC_MODEL : UCHAR
	{
		enStaticModel_Ground = 0,
		enStaticModel_Shpere,
		enStaticModel_Enemy,

		enStaticModel_Max
	};

	//スキンモデル種類.
	enum enSKIN_MODEL : SKIN_ENUM_TYPE
	{
		enSkinModel_Player = 0,
		enSkinModel_Leg,

		enSkinModel_Max//数固定モデルのmax.
	};

	//使うときはこの順番.
	SKIN_ENUM_TYPE m_ucLegNum;	//脚の数.
	SKIN_ENUM_TYPE m_ucCoreNum;	//コアの数.
	SKIN_ENUM_TYPE m_ucHeadNum;	//頭の数.
	SKIN_ENUM_TYPE m_ucArmsNum;	//腕の数( 左右共通なので一つでよい ).
	SKIN_ENUM_TYPE m_ucWeaponNum;//武器の数.

	SKIN_ENUM_TYPE m_ucSkinModelMax;


#ifdef RESOURCE_CLASS_SINGLETON
	//インスタンス取得(唯一のアクセス経路).
	static clsResource* GetInstance()
	{
		//唯一のインスタンスを作成する.
		//(staticで作成されたので2回目は無視される).
		static clsResource s_Instance;
		return &s_Instance;
	}
#else//#ifdef RESOURCE_CLASS_SINGLETON
	clsResource();
#endif//#ifdef RESOURCE_CLASS_SINGLETON
	~clsResource();

	//new直後に使う.
	void Create( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext );


	//スタティックモデル( Attachの引数 ).
	clsDX9Mesh* GetStaticModels( const enSTATIC_MODEL enModel ) const;


	//スキンモデル( Attachの引数 )( パーツはこれではAttach出来ない ).
	clsD3DXSKINMESH*	GetSkinModels( const enSKIN_MODEL enModel ) const;

	//ロボのパーツをAttachする関数.
	//第一引数 : 何のパーツ?.
	//第二引数 : そのパーツの何番目?.
	clsD3DXSKINMESH* GetPartsModels(
		const enPARTS enParts, const SKIN_ENUM_TYPE PartsNum );


#ifdef Inoue
	enSTATIC_MODEL ItoE( const int iNum ) const {
		return static_cast<enSTATIC_MODEL>( iNum );
	}

	int EtoI( const enSTATIC_MODEL iNum ) const {
		return static_cast<int>( iNum );
	}
#endif//#ifdef Inoue.

private:

#ifdef RESOURCE_CLASS_SINGLETON
	//生成やコピーを禁止する.
	clsResource();
	clsResource( const clsResource& rhs );
	clsResource& operator = ( const clsResource& rhs );
#endif//#ifdef RESOURCE_CLASS_SINGLETON

	//スタティックモデル.
	HRESULT		InitStaticModel( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext );
	HRESULT		CreateStaticModel( LPSTR const fileName, const enSTATIC_MODEL enModel );
	HRESULT		ReleaseStaticModel( const enSTATIC_MODEL enModel );


	//スキンモデル.
	HRESULT		InitSkinModel( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext );
	HRESULT		CreateSkinModel( LPSTR const fileName, const enSKIN_MODEL enModel );
	HRESULT		ReleaseSkinModel( const enSKIN_MODEL enModel );


	//パーツ作成.
	void CreatePartsGroup();//CreatePartsの集合体.
	void CreateParts( const enPARTS enParts );
	//CreatePartsで必要な変数を準備する.
	std::string SetVarToCreateParts(
		SKIN_ENUM_TYPE &ucStart,	//(out)そのパーツの始まり番号.
		SKIN_ENUM_TYPE &ucMax,	//(out)そのパーツの最大番号.
		const enPARTS enParts );
		

	//GetSkinModels()の引数をどのパーツかとそのパーツの番号から引き出す関数.
	//第一引数 : 何のパーツ?.
	//第二引数 : そのパーツの何番目?.
	enSKIN_MODEL GetPartsResourceNum( 
		const enPARTS enParts, const SKIN_ENUM_TYPE PartsNum ) const;

	//SetVarToCreateParts()やGetPartsResourceNum()の補助.
	//そのパーツの最初のナンバーをリソース番号にして教えてくれる.
	SKIN_ENUM_TYPE GetPartsResourceStart( const enPARTS enParts ) const;


	HWND					m_hWnd;
	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pCotext11;
	CD3DXSKINMESH_INIT		m_Si;

	clsDX9Mesh**			m_ppStaticModels;
	clsD3DXSKINMESH**		m_ppSkinModels;

	//範囲内かチェックする関数.
	bool IsRangeStaticModel( const enSTATIC_MODEL enModel ) const;
	bool IsRangeSkinModel( const enSKIN_MODEL enModel ) const;


	char m_FilePath[255];


};

#endif//#ifndef RESOURCE_H_