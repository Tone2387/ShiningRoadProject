/***************************************************************************************************
*	SkinMeshCode Version 1.50
*	LastUpdate	: 2017/06/30
**/
#ifndef C_D3DXSKINMESH_H_
#define C_D3DXSKINMESH_H_
// 警告についてのコード分析を無効にする。4005：再定義.
#pragma warning( disable : 4005 )


#include <stdio.h>
#include <windows.h>
#include <d3dx9.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>

#include "Global.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

// 最大ボーン数.
#define MAX_BONES		255
// 最大ｱﾆﾒｰｼｮﾝｾｯﾄ数.
#define MAX_ANIM_SET	100

//// マクロ.
//#define SAFE_RELEASE(p)		{ if(p) { (p)->Release(); (p)=NULL; } }
//#define SAFE_DELETE(p)		{ if(p) { delete (p);     (p)=NULL; } }
//#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p);   (p)=NULL; } }

//方向列挙体.
enum enDirection
{
	enDirection_Stop = 0,//停止.
	enDirection_Foward,//前進.
	enDirection_Backward,//後退.
	enDirection_LeftTurn,//左回転.
	enDirection_RightTurn,//右回転.
	Left,
	Right
};

// シェーダーに渡す値.
struct SHADER_SKIN_GLOBAL0
{	
	D3DXVECTOR4 vLightDir;	// ライト方向.
	D3DXVECTOR4 vEye;		// カメラ位置.
};

struct SHADER_SKIN_GLOBAL1
{
	D3DXMATRIX	mW;			// ワールド行列.
	D3DXMATRIX	mWVP;		// ワールドから射影までの変換行列.
	D3DXVECTOR4 vAmbient;	// アンビエント光.
	D3DXVECTOR4 vDiffuse;	// ディフューズ色.
	D3DXVECTOR4 vSpecular;	// 鏡面反射.
	D3DXVECTOR4 vColorArmor;//カラー.
	D3DXVECTOR4 vColorBase;	//カラー.
};

struct SHADER_GLOBAL_BONES
{
	D3DXMATRIX mBone[MAX_BONES];
	SHADER_GLOBAL_BONES()
	{
		for( int i=0; i<MAX_BONES; i++ )
		{
			D3DXMatrixIdentity( &mBone[i] );
		}
	}
};

// オリジナル　マテリアル構造体.
struct MY_SKINMATERIAL
{
	CHAR szName[110];
	D3DXVECTOR4	Ka;	// アンビエント.
	D3DXVECTOR4	Kd;	// ディフューズ.
	D3DXVECTOR4	Ks;	// スペキュラー.
	CHAR szTextureName[512];	// テクスチャーファイル名.
	ID3D11ShaderResourceView* pTexture;
	DWORD dwNumFace;	// そのマテリアルであるポリゴン数.
	MY_SKINMATERIAL()
	{
		ZeroMemory( szName, 110 );
		Ka = Kd = Ks = { 0.0f, 0.0f, 0.0f, 0.0f };
		ZeroMemory( szTextureName, 512 );
		pTexture = NULL;
		dwNumFace = 0;
	}
	~MY_SKINMATERIAL()
	{
		SAFE_RELEASE( pTexture );
	}
};

// 頂点構造体.
struct MY_SKINVERTEX
{
	D3DXVECTOR3	vPos;		// 頂点位置.
	D3DXVECTOR3	vNorm;		// 頂点法線.
	D3DXVECTOR2	vTex;		// UV座標.
	UINT bBoneIndex[4];		// ボーン 番号.
	float bBoneWeight[4];	// ボーン 重み.
	MY_SKINVERTEX()
	{
//		ZeroMemory( this, sizeof( MY_SKINVERTEX ) );
		vPos = vNorm = { 0.0f, 0.0f, 0.0f };
		vTex = { 0.0f, 0.0f };
		for( char i=0; i<4; i++ ){
			bBoneIndex[i] = 0;
			bBoneWeight[i] = 0.0f;
		}
	}
};
// ボーン構造体.
struct BONE
{
	D3DXMATRIX mBindPose;	// 初期ポーズ（ずっと変わらない）.
	D3DXMATRIX mNewPose;	// 現在のポーズ（その都度変わる）.
	DWORD dwNumChild;		// 子の数.
	int iChildIndex[50];	// 自分の子の“インデックス”50個まで.
	CHAR Name[256];

	BONE()
	{
		ZeroMemory( this, sizeof( BONE ) );
		D3DXMatrixIdentity( &mBindPose );
		D3DXMatrixIdentity( &mNewPose );
	}
};

// パーツメッシュ構造体.
struct SKIN_PARTS_MESH 
{
	DWORD	dwNumVert;
	DWORD	dwNumFace;
	DWORD	dwNumUV;
	DWORD			dwNumMaterial;
	MY_SKINMATERIAL*	pMaterial;
	char				TextureFileName[8][256];	// テクスチャーファイル名(8枚まで).
	bool				bTex;

	ID3D11Buffer*	pVertexBuffer;
	ID3D11Buffer**	ppIndexBuffer;

	// ボーン.
	int		iNumBone;
	BONE*	pBoneArray;

	bool	bEnableBones;	// ボーンの有無フラグ.

	SKIN_PARTS_MESH()
	{
//		ZeroMemory( this, sizeof( SKIN_PARTS_MESH ) );
		dwNumVert = dwNumFace = dwNumUV = dwNumMaterial = 0;
		pMaterial = NULL;
		ZeroMemory( TextureFileName, 8 * 256 );
		bTex = bEnableBones = false;
		pVertexBuffer = NULL;
		ppIndexBuffer = NULL;
		iNumBone = 0;
		pBoneArray = NULL;
	}
};



// デバイス関係受取用構造体.
struct CD3DXSKINMESH_INIT
{
	HWND hWnd;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

	CD3DXSKINMESH_INIT(){
		hWnd = NULL;
		pDevice = NULL;
		pDeviceContext = NULL;
	}
};

// 派生フレーム構造体.
//	それぞれのメッシュ用の最終ワールド行列を追加する.
struct MYFRAME: public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
	SKIN_PARTS_MESH* pPartsMesh;
	MYFRAME(){
//		ZeroMemory( this, sizeof( MYFRAME ));
		ZeroMemory( &CombinedTransformationMatrix, sizeof( D3DXMATRIX ));
		pPartsMesh = NULL;
	}
};
// 派生メッシュコンテナー構造体.
//	コンテナーがテクスチャを複数持てるようにポインターのポインターを追加する
struct MYMESHCONTAINER: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;
	DWORD dwWeight;				// 重みの個数（重みとは頂点への影響。）.
	DWORD dwBoneNum;			// ボーンの数.
	LPD3DXBUFFER pBoneBuffer;	// ボーン・テーブル.
	D3DXMATRIX** ppBoneMatrix;	// 全てのボーンのワールド行列の先頭ポインタ.
	D3DXMATRIX* pBoneOffsetMatrices;// フレームとしてのボーンのワールド行列のポインタ.
};
// Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる.
//	ID3DXAllocateHierarchyは派生すること想定して設計されている.
class MY_HIERARCHY: public ID3DXAllocateHierarchy
{
public:
	MY_HIERARCHY(){}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR , LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR ,CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , 
		CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME );
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER );	
};

//==================================================================================================
//
//	パーサークラス.
//
//==================================================================================================
class D3DXPARSER
{
public:

	D3DXPARSER();
	~D3DXPARSER();

	MY_HIERARCHY cHierarchy;
	MY_HIERARCHY* m_pHierarchy;
	LPD3DXFRAME m_pFrameRoot;

	LPD3DXANIMATIONCONTROLLER m_pAnimController;//ﾃﾞﾌｫﾙﾄで一つ.
	LPD3DXANIMATIONSET m_pAnimSet[MAX_ANIM_SET];

	HRESULT LoadMeshFromX( LPDIRECT3DDEVICE9, LPSTR );
	HRESULT AllocateBoneMatrix( LPD3DXMESHCONTAINER );
	HRESULT AllocateAllBoneMatrices( LPD3DXFRAME );
	VOID UpdateFrameMatrices( LPD3DXFRAME, LPD3DXMATRIX );


	int GetNumVertices( MYMESHCONTAINER* pContainer );
	int GetNumFaces( MYMESHCONTAINER* pContainer );
	int GetNumMaterials( MYMESHCONTAINER* pContainer );
	int GetNumUVs( MYMESHCONTAINER* pContainer );
	int GetNumBones( MYMESHCONTAINER* pContainer );
	int GetNumBoneVertices( MYMESHCONTAINER* pContainer, int iBoneIndex);
	DWORD GetBoneVerticesIndices( MYMESHCONTAINER* pContainer, int iBoneIndex, int iIndexInGroup );
	double GetBoneVerticesWeights( MYMESHCONTAINER* pContainer, int iBoneIndex, int iIndexInGroup );
	D3DXVECTOR3 GetVertexCoord( MYMESHCONTAINER* pContainer, DWORD iIndex );
	D3DXVECTOR3 GetNormal( MYMESHCONTAINER* pContainer, DWORD iIndex );
	D3DXVECTOR2 GetUV( MYMESHCONTAINER* pContainer, DWORD iIndex );
	int GetIndex( MYMESHCONTAINER* pContainer, DWORD iIndex );
	D3DXVECTOR4 GetAmbient( MYMESHCONTAINER* pContainer, int iIndex );
	D3DXVECTOR4 GetDiffuse( MYMESHCONTAINER* pContainer, int iIndex );
	D3DXVECTOR4 GetSpecular( MYMESHCONTAINER* pContainer, int iIndex );
	CHAR* GetTexturePath( MYMESHCONTAINER* pContainer, int index );
	float GetSpecularPower( MYMESHCONTAINER* pContainer, int iIndex );
	int GeFaceMaterialIndex( MYMESHCONTAINER* pContainer, int iFaceIndex );
	int GetFaceVertexIndex( MYMESHCONTAINER* pContainer, int iFaceIndex, int iIndexInFace );
	D3DXMATRIX GetBindPose( MYMESHCONTAINER* pContainer, int iBoneIndex );
	D3DXMATRIX GetNewPose( MYMESHCONTAINER* pContainer, int iBoneIndex );
	CHAR* GetBoneName( MYMESHCONTAINER* pContainer, int iBoneIndex );

	// メッシュコンテナを取得する.
	LPD3DXMESHCONTAINER GetMeshContainer( LPD3DXFRAME pFrame );

	// アニメーションセットの切り替え.
	void ChangeAnimSet( int index, LPD3DXANIMATIONCONTROLLER pAC=NULL );
	// アニメーションセットの切り替え(開始フレーム指定可能版).
	void ChangeAnimSet_StartPos( int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// アニメーション停止時間を取得.
	double GetAnimPeriod( int index );
	// アニメーション数を取得.
	int GetAnimMax( LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// 指定したボーン情報(座標・行列)を取得する関数.
	bool GetMatrixFromBone( const char* sBoneName, D3DXMATRIX* const pOutMat ) const;
	bool GetPosFromBone( const char* sBoneName, D3DXVECTOR3* const pOutPos ) const;
	//ボーンがあるか無いかを調べる関数.
	bool ExistsBone( const char* sBoneName );

	// メッシュ解放.
	HRESULT ReleaseMesh( LPD3DXFRAME pFrame );

	// 一括解放処理.
	HRESULT Release();
};

//==================================================================================================
//
//	スキンメッシュクラス.
//
//==================================================================================================
class clsD3DXSKINMESH
{
public:

	// メソッド.
	clsD3DXSKINMESH( 
		const HWND hWnd, 
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11, 
		const char* sFileName );
	~clsD3DXSKINMESH();

	// 描画関数.
	void Render( 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye, 
		const D3DXVECTOR4& vColorBase,
		const D3DXVECTOR4& vColorArmor,
		const bool isAlpha = false, 
		LPD3DXANIMATIONCONTROLLER pAC=NULL );

	//リソースのボーン位置がずれる対策.
	void UpdateBonePos();
//	void UpDateBonePosProduct( LPD3DXFRAME p );
//	void UpDateBonePosProductProduct(
//		SKIN_PARTS_MESH* pMesh,
//		MYMESHCONTAINER* const pContainer );

	double GetAnimSpeed()				{ return m_dAnimSpeed;		}
	void SetAnimSpeed( double dSpeed )	{ m_dAnimSpeed = dSpeed;	}

	double GetAnimTime()				{ return m_dAnimTime; }
	void SetAnimTime(double dTime)	{ m_dAnimTime = dTime; }

	// アニメーションセットの切り替え.
	void ChangeAnimSet( int index, LPD3DXANIMATIONCONTROLLER pAC=NULL );
	// アニメーションセットの切り替え(開始フレーム指定可能版).
	void ChangeAnimSet_StartPos( int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// アニメーション停止時間を取得.
	double GetAnimPeriod( int index );
	// アニメーション数を取得.
	int GetAnimMax( LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// 指定したボーン情報(座標・行列)を取得する関数.
	bool GetMatrixFromBone( char* sBoneName, D3DXMATRIX* pOutMat );
	bool GetPosFromBone( const char* sBoneName, D3DXVECTOR3* const pOutPos, const bool isLocalPos = false ) const;
	bool GetDeviaPosFromBone(char* sBoneName, D3DXVECTOR3* pOutPos, D3DXVECTOR3 vDeviation = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//ボーンがあるか無いかを調べる関数.
	bool ExistsBone( const char* sBoneName );

	//パーサークラスからアニメーションコントローラーを取得する.
	LPD3DXANIMATIONCONTROLLER GetAnimController()
	{
		return m_pD3dxMesh->m_pAnimController;
	}

	TRANSFORM m_Trans;


	//X軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisX;
	//Z軸ﾍﾞｸﾄﾙを用意.
	D3DXVECTOR3 vecAxisZ;

	//方向列挙体.
	enDirection m_enDir;

	D3DXMATRIX m_mWorld;
	D3DXMATRIX m_mRotation;

	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;
	D3DXVECTOR3 m_vLight;
	D3DXVECTOR3 m_vEye;

	//アニメーション速度.
	double m_dAnimSpeed;
	double m_dAnimTime;

private:
	//リソースのボーン位置がずれる対策.
	void UpDateBonePosProduct( LPD3DXFRAME p );
	void UpDateBonePosProductProduct(
		SKIN_PARTS_MESH* pMesh,
		MYMESHCONTAINER* const pContainer );

	//ブレンドステート作成.
	HRESULT CreateBlendState();
	// 解放関数.
	HRESULT Release();

	HRESULT CreateDeviceDx9( HWND hWnd, LPDIRECT3DDEVICE9* ppOutDevice9 );
	HRESULT InitShader();
	// Xファイルからスキンメッシュを作成する.
	HRESULT CreateFromX( CHAR* sFileName, LPDIRECT3DDEVICE9 pDevice9 );
	HRESULT CreateIndexBuffer( DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer );
	void RecursiveSetNewPoseMatrices( BONE* pBone,D3DXMATRIX* pmParent );

	// 全てのメッシュを作成する.
	void BuildAllMesh( D3DXFRAME* pFrame );
	
	// メッシュを作成する.
	HRESULT CreateAppMeshFromD3DXMesh( LPD3DXFRAME pFrame );

	// Xファイルからスキン関連の情報を読み出す関数.
	HRESULT ReadSkinInfo( MYMESHCONTAINER* pContainer, MY_SKINVERTEX* pvVB, SKIN_PARTS_MESH* pParts );

	// ボーンを次のポーズ位置にセットする関数.
	void SetNewPoseMatrices( SKIN_PARTS_MESH* pParts, int frame, MYMESHCONTAINER* pContainer );
	// 次の(現在の)ポーズ行列を返す関数.
	D3DXMATRIX GetCurrentPoseMatrix( SKIN_PARTS_MESH* pParts, int index );

	// フレーム描画.
	void DrawFrame( 
		LPD3DXFRAME pFrame,
		const D3DXVECTOR4& vColorBase,
		const D3DXVECTOR4& vColorArmor,
		const bool isAlpha );
	// パーツ描画.
	void DrawPartsMesh( 
		SKIN_PARTS_MESH* p, 
		D3DXMATRIX World, 
		MYMESHCONTAINER* const pContainer,
		const D3DXVECTOR4& vColorBase,
		const D3DXVECTOR4& vColorArmor,
		const bool isAlpha );
	void DrawPartsMeshStatic( SKIN_PARTS_MESH* pMesh, D3DXMATRIX World, MYMESHCONTAINER* pContainer );

	// 全てのメッシュを削除.
	void DestroyAllMesh( D3DXFRAME* pFrame );
	HRESULT DestroyAppMeshFromD3DXMesh( LPD3DXFRAME p );

	//透過(アルファブレンド)設定の切り替え.
	void SetBlend( const bool isAlpha );
	ID3D11BlendState*	m_pBlendState[ enBLEND_STATE_size ];		//ブレンドステート.


	struct MASK_TEXTURE
	{
		ID3D11ShaderResourceView*	pTex;
//		ID3D11SamplerState*			pSample;

		MASK_TEXTURE(){
			pTex = nullptr;
//			pSample = nullptr;
		}
		~MASK_TEXTURE(){
//			SAFE_RELEASE( pSample );
			SAFE_RELEASE( pTex );
		}
	};
	MASK_TEXTURE* m_pMaskBase;
	MASK_TEXTURE* m_pMaskArmor;


	HWND m_hWnd;
	// Dx9.
//	LPDIRECT3D9			m_pD3d9;
//	LPDIRECT3DDEVICE9	m_pDevice9;
	// Dx11.
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
	ID3D11SamplerState*		m_pSampleLinear;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pVertexLayout;
	ID3D11Buffer*			m_pConstantBuffer0;
	ID3D11Buffer*			m_pConstantBuffer1;
	ID3D11Buffer*			m_pConstantBufferBone;

	// メッシュ.
	D3DXPARSER* m_pD3dxMesh;

	// XFileのパス.
	CHAR	m_FilePath[256];

	// アニメーションフレーム.
	int		m_iFrame;

};

#endif//#ifndef C_D3DXSKINMESH_H_