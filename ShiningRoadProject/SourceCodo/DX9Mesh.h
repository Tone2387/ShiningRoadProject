#ifndef DX9MESH_H_
#define DX9MESH_H_
//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )	

//class LPD3DXMESH;
#include <d3dx9.h>

#include "Global.h"


#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )



//メッシュデータをファイルから取り出すためだけにDirectX9を使用する.
//※レンダリング(表示)はDirectX11で行う.
class clsDX9Mesh
{
public:
	clsDX9Mesh();	//コンストラクタ.
	~clsDX9Mesh();	//デストラクタ.

	TRANSFORM m_Trans;

	//初期化.
	HRESULT Init( const HWND hWnd, ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11, const LPSTR fileName );


	//レンダリング用(※DX9MESH内とMain内で2つ存在するので注意).
	void Render( 
		const D3DXMATRIX& mView,
		const D3DXMATRIX& mProj,
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR4& vColor = D3DXVECTOR4( 1.0f,1.0f,1.0f,1.0f ),
		const bool isAlpha = false );

	//透過(アルファブレンド)設定の切り替え.
	void SetBlend( const bool isAlpha );


public:

	LPD3DXMESH			m_pMesh;		//メッシュオブジェクト.

	D3DXVECTOR3	m_vRay;			//レイの位置.
	D3DXVECTOR3	m_vAxis;		//回転軸.
	LPD3DXMESH	m_pMeshForRay;	//レイとメッシュ用.

private:

	//ブレンドステート作成.
	HRESULT CreateBlendState();

	//Dx9初期化用.
	HRESULT InitDx9( const HWND hWnd, LPDIRECT3DDEVICE9* ppOutDevice9, const LPSTR sErrFilePath );

	//メッシュ読込.
	HRESULT LoadXMesh( const LPSTR fileName, LPDIRECT3DDEVICE9 pDevice9  );

	//シェーダ作成.
	HRESULT InitShader( const LPSTR sErrFilePath );

private:


	D3DXVECTOR3 vecAxisX;
	//Z軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisZ;

//	HWND				m_hWnd;	//ウィンドウハンドル.

	//Dx9.
//	LPDIRECT3D9			m_pD3d;	//DX9オブジェクト.
//	LPDIRECT3DDEVICE9	m_pDevice9;	//Dx9デバイスオブジェクト.

	//D3DMATERIAL9*		m_pMaterials;	//マテリアル情報.
	//LPDIRECT3DTEXTURE9*	m_pTextures;	//テクスチャ情報.
	//char				m_TexFileName[8][255];	//テクスチャファイル名(8枚まで).
	DWORD				m_dwNumMaterials;//マテリアル数.

	//Dx11.
	ID3D11Device*			m_pDevice;		//デバイスオブジェクト.
	ID3D11DeviceContext*	m_pContext;	//デバイスコンテキスト.
	ID3D11VertexShader*		m_pVertexShader;	//頂点シェーダ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;		//ピクセルシェーダ.
	ID3D11Buffer*			m_pConstantBuffer0;	//コンスタントバッファ0.
	ID3D11Buffer*			m_pConstantBuffer1;	//コンスタントバッファ1.
//
//
	ID3D11Buffer*			m_pVertexBuffer;//頂点(バーテックス)バッファ.
	ID3D11Buffer**			m_ppIndexBuffer;//インデックスバッファ.
	ID3D11SamplerState*		m_pSampleLinear;//テクスチャのサンプラー.//テクスチャに各種フィルタをかける.
//
//
	//マテリアル構造体.
	struct MY_MATERIAL
	{
		D3DXVECTOR4		Ambient;	//アンビエント.
		D3DXVECTOR4		Diffuse;	//ディフューズ.
		D3DXVECTOR4		Specular;	//スペキュラ.
		CHAR	szTextureName[128];	//テクスチャファイル名.
		ID3D11ShaderResourceView*	pTexture;//テクスチャ.
		DWORD	dwNumFace;			//そのマテリアルのポリゴン数.
		//コンストラクタ.
		MY_MATERIAL(){
	//		ZeroMemory( this, sizeof( MY_MATERIAL ) );
			pTexture = nullptr;
		}
		//デストラクタ.
		~MY_MATERIAL(){
			SAFE_RELEASE( this->pTexture );
		}
	}	*m_pMaterials;	//マテリアル構造体.

	DWORD			m_NumAttr;		//属性数.
	DWORD			m_AttrID[300];	//属性ID ※300属性まで.

	bool			m_bTexture;		//テクスチャの有無.



	ID3D11BlendState*	m_pBlendState[ enBLEND_STATE_size ];		//ブレンドステート.

};

#endif//#ifndef DX9MESH_H_
