#ifndef RESOURCE_H_
#define RESOURCE_H_

#define _CRT_SECURE_NO_WARNINGS


////シングルトンにするならばつける.
////#define RESOURCE_CLASS_SINGLETON


//テスト中はつける( パーツの読み込み数を固定化 ).
#define RESOURCE_READ_PARTS_MODEL_LOCK


#ifdef RESOURCE_READ_PARTS_MODEL_LOCK
	//テスト中のパーツ最大数.
	const int iTEST_ROBO_PARTS_MODEL_MAX = 1;
#endif//#ifndef RESOURCE_READ_PARTS_MODEL_LOCK

//
////テストモデルに足の3番のモデルを割り当てる例.
//	m_pMesh->AttachModel(
//		m_wpResource->GetPartsModels( enPARTS::LEG, 3 ) );
//
//
////今まで.
//	m_pMesh->AttachModel(
//		m_wpResource->GetSkinModels( clsResource::enSkinModel_Leg ) );
//

#include "Global.h"
#include "DX9Mesh.h"
#include "CD3DXSKINMESH.h"


//スキンメッシュ列挙体の型.
using SKIN_ENUM_TYPE = UCHAR;


//3Dモデルのもとデータを格納するクラス.
//シングルトンにしてどこからでも参照できる.
class clsResource
{
public:
	//スタティックモデル種類.
	enum enSTATIC_MODEL : UCHAR
	{
		enStaticModel_StageCollision = 0,//ステージの当たり判定.
		enStaticModel_StageFloor,	//ステージの床と壁.
		enStaticModel_StageCelling,	//ステージの天井.
		enStaticModel_Building,		//ステージの障害物.
		enStaticModel_Shpere,		//当たり判定チェック用.

		enStaticModel_Max
	};

	//スキンモデル種類.
	enum enSKIN_MODEL : SKIN_ENUM_TYPE
	{
		enSkinModel_Door = 0,//ステージのドア.
		enSkinModel_Lia,	//ステージの天井のドア.

		enSkinModel_Max//数固定モデルのmax.
	};




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
		const enPARTS enParts, 
		const SKIN_ENUM_TYPE PartsNum );



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
	void CreateParts( const enPARTS enParts );//引数としてenPARTS::WEAPON_Rは使わない(同じものを2つ作ってしまう).
	//CreatePartsで必要な変数を準備する.
	std::string SetVarToCreateParts(
		SKIN_ENUM_TYPE &ucStart,	//(out)そのパーツの始まり番号.
		SKIN_ENUM_TYPE &ucMax,	//(out)そのパーツの最大番号.
		std::string &sModelName,//(out)パスにくっつけるモデル名.
		const enPARTS enParts );
		

	//GetSkinModels()の引数をどのパーツかとそのパーツの番号から引き出す関数.
	//第一引数 : 何のパーツ?.
	//第二引数 : そのパーツの何番目?.
	enSKIN_MODEL GetPartsResourceNum( 
		const enPARTS enParts, SKIN_ENUM_TYPE PartsNum ) const;

	//SetVarToCreateParts()やGetPartsResourceNum()の補助.
	//そのパーツの最初のナンバーをリソース番号にして教えてくれる.
	SKIN_ENUM_TYPE GetPartsResourceStart( const enPARTS enParts ) const;


	//範囲内かチェックする関数.
	bool IsRangeStaticModel( const enSTATIC_MODEL enModel ) const;
	bool IsRangeSkinModel( const enSKIN_MODEL enModel ) const;


	//パーツの数を吐き出す.
	//読み込むパーツ種類.
	enum enPARTS_READ : UCHAR
	{
		LEG = 0,
		CORE,
		HEAD,
		ARMS,
		WEAPON,

		enPARTS_READ_SIZE
	};
	SKIN_ENUM_TYPE GetPartsNum( const enPARTS_READ enPartsRead );

private:

	SKIN_ENUM_TYPE m_PartsNum[ enPARTS_READ_SIZE ];
	SKIN_ENUM_TYPE m_ucSkinModelMax;


	HWND					m_hWnd;
	ID3D11Device*			m_wpDevice11;
	ID3D11DeviceContext*	m_wpCotext11;
	CD3DXSKINMESH_INIT		m_Si;

	clsDX9Mesh**			m_ppStaticModels;
	clsD3DXSKINMESH**		m_ppSkinModels;

};

#endif//#ifndef RESOURCE_H_