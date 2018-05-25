#ifndef RESOURCE_H_
#define RESOURCE_H_

#define _CRT_SECURE_NO_WARNINGS

#include "Global.h"
#include "DX9Mesh.h"
#include "CD3DXSKINMESH.h"



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

		enStaticModel_Max,
	};

	//スキンモデル種類.
	enum enSKIN_MODEL : UCHAR
	{
		enSkinModel_Player = 0,

		enSkinModel_Max,//数固定モデルのmax.
	};

	//脚.
	enum enLEG_MODEL : UCHAR
	{
		enLegModel0 = enSkinModel_Max,
		enLegModel1,
		enLegModel2,
		enLegModel3,

		enLegModelMax,
	};
	//コア.
	enum enCORE_MODEL : UCHAR
	{
		enCoreModel0 = enLegModelMax,
		enCoreModel1,
		enCoreModel2,
		enCoreModel3,

		enCoreModelMax,
	};
	//頭.
	enum enHEAD_MODEL : UCHAR
	{
		enHeadModel0 = enCoreModelMax,
		enHeadModel1,
		enHeadModel2,
		enHeadModel3,
		  
		enHeadModelMax,
	};
	//左腕.
	enum enARML_MODEL : UCHAR
	{
		enArmLModel0 = enHeadModelMax,
		enArmLModel1,
		enArmLModel2,
		enArmLModel3,
		  
		enArmLModelMax,
	};
	//右腕.
	enum enARMR_MODEL : UCHAR
	{
		enArmRModel0 = enArmLModelMax,
		enArmRModel1,
		enArmRModel2,
		enArmRModel3,
		  
		enArmRModelMax,
	};
	//武器.
	enum enWEAPON_MODEL : UCHAR
	{
		enWeaponModel0 = enArmRModelMax,
		enWeaponModel1,
		enWeaponModel2,
		enWeaponModel3,

		enWeaponModelMax,//全スキンモデルのmax.
	};
	//すべてのスキンモデルの数.
	enum enALL_SKIN_MODEL : UCHAR
	{
		enAllPartsMax = enWeaponModelMax
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


	//スタティックモデル.
	clsDX9Mesh* GetStaticModels( const enSTATIC_MODEL enModel ) const;


	//スキンモデル.
	clsD3DXSKINMESH*	GetSkinModels( const enSKIN_MODEL enModel ) const;

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
		UCHAR &ucStart,	//(out)そのパーツの始まり番号.
		UCHAR &ucMax,	//(out)そのパーツの最大番号.
		const enPARTS enParts );
		

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