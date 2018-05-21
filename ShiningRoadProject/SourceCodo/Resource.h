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

		enSkinModel_Max,
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



	//スタティックモデル.
	HRESULT		InitStaticModel( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext );
	HRESULT		CreateStaticModel( LPSTR const fileName, const enSTATIC_MODEL enModel );
	clsDX9Mesh* GetStaticModels( const enSTATIC_MODEL enModel ) const;
	HRESULT		ReleaseStaticModel( const enSTATIC_MODEL enModel );


	//スキンモデル.
	HRESULT		InitSkinModel( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext );
	HRESULT		CreateSkinModel( LPSTR const fileName, const enSKIN_MODEL enModel );
	clsD3DXSKINMESH*	GetSkinModels( const enSKIN_MODEL enModel ) const;
	HRESULT		ReleaseSkinModel( const enSKIN_MODEL enModel );

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