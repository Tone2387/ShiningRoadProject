/***************************************************************************************************
*	SkinMeshCode Version 1.50
*	LastUpdate	: 2017/06/30
**/
#include <stdlib.h>
#include "CD3DXSKINMESH.h"
#include <string.h>


namespace{
	// シェーダ名(ディレクトリも含む)
	const char SHADER_NAME[] = "Shader\\MeshSkin.hlsl";
	//マスクテクスチャパス.
	const char sMASK_PATH_0[] = "mask0.png";
	const char sMASK_PATH_1[] = "mask1.png";
}

// フレームを作成する.
HRESULT MY_HIERARCHY::CreateFrame( LPCSTR Name, LPD3DXFRAME *ppNewFrame )
{
	HRESULT hr = S_OK;
	MYFRAME *pFrame;

	*ppNewFrame = NULL;

	pFrame = new MYFRAME;
	if (pFrame == NULL)
	{
		return E_OUTOFMEMORY;
	}
	int length = strlen( Name ) + 1;
	pFrame->Name = new CHAR[length];
	if (!pFrame->Name)
	{
		return E_FAIL;
	}
	strcpy_s( pFrame->Name, sizeof(CHAR)*length, Name );

	D3DXMatrixIdentity( &pFrame->TransformationMatrix );
	D3DXMatrixIdentity( &pFrame->CombinedTransformationMatrix );

	*ppNewFrame = pFrame;

	return S_OK;
}


// メッシュコンテナーを作成する.
HRESULT MY_HIERARCHY::CreateMeshContainer(
	LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials, CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials, CONST DWORD *pAdjacency, LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER *ppMeshContainer )
{
	*ppMeshContainer = NULL;
	DWORD dwBoneNum	=0;

	MYMESHCONTAINER *pMeshContainer = new MYMESHCONTAINER;
	ZeroMemory( pMeshContainer, sizeof( MYMESHCONTAINER ) );
	int length = strlen(Name) + 1;
	pMeshContainer->Name=new CHAR[length];
	strcpy_s( pMeshContainer->Name, sizeof( CHAR )*length, Name );	

	memcpy( &pMeshContainer->MeshData, pMeshData, sizeof( pMeshContainer->MeshData ) );
	pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	int NumPoly=pMeshContainer->MeshData.pMesh->GetNumFaces();
	// メッシュを複製する.
	//	なぜかスキンメッシュだと、この関数を抜けた直後にD3DX内部でメッシュポインターがおかしくなってしまうので.
	LPDIRECT3DDEVICE9 pDevice=NULL;
	pMeshContainer->MeshData.pMesh->GetDevice( &pDevice );
	LPD3DXMESH pMesh=NULL;
	pMeshContainer->MeshData.pMesh->CloneMesh( NULL, NULL, pDevice, &pMesh );
	assert( pMesh );
	//SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	pMeshContainer->MeshData.pMesh=pMesh;

	// メッシュのマテリアル設定.
	pMeshContainer->NumMaterials = max( 1, NumMaterials );
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[pMeshContainer->MeshData.pMesh->GetNumFaces()*3];
	if( ( pMeshContainer->pAdjacency == NULL ) || ( pMeshContainer->pMaterials == NULL ) )
	{
		return E_FAIL;
	}
	memcpy( pMeshContainer->pAdjacency, pAdjacency, sizeof( DWORD ) * NumPoly*3 );

	if( NumMaterials > 0 )
	{
		for( DWORD i=0; i<NumMaterials; i++ )
		{			
			memcpy( &pMeshContainer->pMaterials[i], &pMaterials[i], sizeof( D3DXMATERIAL ) );
			if( pMaterials[i].pTextureFilename != NULL )
			{
				int length = 512;
				pMeshContainer->pMaterials[i].pTextureFilename = new CHAR[length];
				strcpy_s( pMeshContainer->pMaterials[i].pTextureFilename,
					sizeof( CHAR ) * length, pMaterials[i].pTextureFilename );
			}
		}
	}
	else
	{
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset( &pMeshContainer->pMaterials[0].MatD3D, 0, sizeof( D3DMATERIAL9 ) );
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}
	// 当該メッシュがスキン情報を持っている場合(スキンメッシュ固有の処理).
	if( pSkinInfo != NULL )
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();
		dwBoneNum = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[dwBoneNum];

		for( DWORD i= 0; i < dwBoneNum; i++ )
		{	
			memcpy( &pMeshContainer->pBoneOffsetMatrices[i],
				pMeshContainer->pSkinInfo->GetBoneOffsetMatrix( i ), sizeof( D3DMATRIX ) );
		}
	}
	// ローカルに生成したメッシュコンテナーを呼び出し側にコピーする.
	*ppMeshContainer = pMeshContainer;

	return S_OK;
}



// フレームを破棄する.
HRESULT MY_HIERARCHY::DestroyFrame( LPD3DXFRAME pFrameToFree )
{
	if( pFrameToFree->Name ){
		delete[] pFrameToFree->Name;
		pFrameToFree->Name = NULL;
	}
	if( pFrameToFree->pMeshContainer ){
		DestroyMeshContainer( pFrameToFree->pMeshContainer );
	}
	if( pFrameToFree->pFrameFirstChild ){
		DestroyFrame( pFrameToFree->pFrameFirstChild );
	}
	if( pFrameToFree->pFrameSibling ){
		DestroyFrame( pFrameToFree->pFrameSibling );
	}
	delete pFrameToFree;

    return S_OK; 
}


// メッシュコンテナーを破棄する.
HRESULT MY_HIERARCHY::DestroyMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase )
{
	DWORD dwMaterial;
	MYMESHCONTAINER *pMeshContainer = ( MYMESHCONTAINER* )pMeshContainerBase;

	SAFE_DELETE_ARRAY( pMeshContainer->Name );
	SAFE_RELEASE( pMeshContainer->pSkinInfo );
	SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
	SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );

	SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrix );

	for( DWORD i=0; i<pMeshContainer->dwBoneNum; i++ ){
		pMeshContainer->ppBoneMatrix[i] = NULL;
	}

	if( pMeshContainer->ppTextures != NULL )
	{
		for( dwMaterial=0; dwMaterial<pMeshContainer->NumMaterials; dwMaterial++ )
		{
			SAFE_RELEASE( pMeshContainer->ppTextures[dwMaterial] );
		}
	}
	SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );

	SAFE_RELEASE( pMeshContainer->MeshData.pMesh );

	if( pMeshContainer->pBoneBuffer != NULL )
	{
		SAFE_RELEASE( pMeshContainer->pBoneBuffer );
		SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );
	}

	SAFE_DELETE( pMeshContainer );

	return S_OK;
}

/*******************************************************************************************************************************************
*
*	以降、パーサークラス.
*
**/

D3DXPARSER::D3DXPARSER()
	:m_pHierarchy( NULL )
	,m_pFrameRoot( NULL )
	,m_pAnimController( NULL )
//	,m_pAnimSet()
{
	ZeroMemory( &cHierarchy, sizeof( cHierarchy ) );
	for( int i=0; i<MAX_ANIM_SET; i++ ){
		m_pAnimSet[i] = NULL;
	}
}

D3DXPARSER::~D3DXPARSER()
{
}

// ボーン行列の領域確保.
HRESULT D3DXPARSER::AllocateBoneMatrix( LPD3DXMESHCONTAINER pMeshContainerBase )
{
	MYFRAME *pFrame=NULL;
	DWORD dwBoneNum=0;

	MYMESHCONTAINER *pMeshContainer = (MYMESHCONTAINER*)pMeshContainerBase;
	if( pMeshContainer->pSkinInfo == NULL )
	{
		return S_OK;
	}
	dwBoneNum = pMeshContainer->pSkinInfo->GetNumBones();
	pMeshContainer->ppBoneMatrix = new D3DXMATRIX*[dwBoneNum];

	for( DWORD i=0; i<dwBoneNum; i++ )
	{
		// まずはNULLで初期化.
		pMeshContainer->ppBoneMatrix[i] = NULL;

		pFrame = (MYFRAME*)D3DXFrameFind( m_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName( i ) );
		if( pFrame == NULL )
		{
			return E_FAIL;
		}
		pMeshContainer->ppBoneMatrix[i] = &pFrame->CombinedTransformationMatrix;

	}
	return S_OK;
}



// 全てのボーン行列の領域を確保(再帰関数).
HRESULT D3DXPARSER::AllocateAllBoneMatrices( LPD3DXFRAME pFrame )
{
	if( pFrame->pMeshContainer != NULL )
	{
		if( FAILED( AllocateBoneMatrix( pFrame->pMeshContainer ) ) )
		{
			return E_FAIL;
		}
	}
	if( pFrame->pFrameSibling != NULL )
	{
		if( FAILED( AllocateAllBoneMatrices( pFrame->pFrameSibling ) ) )
		{
			return E_FAIL;
		}
	}
	if( pFrame->pFrameFirstChild != NULL )
	{
		if( FAILED( AllocateAllBoneMatrices( pFrame->pFrameFirstChild ) ) )
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

// Xファイルからメッシュを読み込む.
HRESULT D3DXPARSER::LoadMeshFromX( LPDIRECT3DDEVICE9 pDevice9, LPSTR FileName )
{
	// Xファイルからアニメーションメッシュを読み込み作成する
	m_pHierarchy=new MY_HIERARCHY;
	if( FAILED( 
		D3DXLoadMeshHierarchyFromXA(
			FileName, D3DXMESH_MANAGED, pDevice9, m_pHierarchy,
			NULL, &m_pFrameRoot, &m_pAnimController)))
	{
			MessageBoxA( NULL, "Xファイルの読み込みに失敗しました", FileName, MB_OK );
			return E_FAIL;
	}
	// ボーンメモリ割りあて.
	AllocateAllBoneMatrices( m_pFrameRoot );


/*
	m_pContainer = (MYMESHCONTAINER*)GetMeshContainer( m_pFrameRoot );

	if( m_pContainer == NULL ){
		MessageBox( NULL, L"メッシュコンテナが見つからなかった", L"error", MB_OK );
		return E_FAIL;
	}
*/


	DWORD iAnimMax = m_pAnimController->GetNumAnimationSets();
	// アニメーションセットを得る.
	for( DWORD i=0; i<iAnimMax; i++ )
	{
		m_pAnimController->GetAnimationSet( i, &m_pAnimSet[i] );
	}

	return S_OK;
}



// フレーム内のメッシュ毎にワールド変換行列を更新する.
VOID D3DXPARSER::UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix )
{
	MYFRAME *pFrame = (MYFRAME*)pFrameBase;	

	if( pParentMatrix != NULL )
	{
		D3DXMatrixMultiply( &pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix );
	}
	else
	{
		pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
	}
	if( pFrame->pFrameSibling != NULL )
	{
		UpdateFrameMatrices( pFrame->pFrameSibling, pParentMatrix );
	}
	if( pFrame->pFrameFirstChild != NULL )
	{
		UpdateFrameMatrices( pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix );
	}
}


int D3DXPARSER::GetNumVertices( MYMESHCONTAINER* pContainer )
{
	return pContainer->MeshData.pMesh->GetNumVertices();
}


int D3DXPARSER::GetNumFaces( MYMESHCONTAINER* pContainer )
{
	return pContainer->MeshData.pMesh->GetNumFaces();
}


int D3DXPARSER::GetNumMaterials( MYMESHCONTAINER* pContainer )
{
	return pContainer->NumMaterials;
}


int D3DXPARSER::GetNumUVs( MYMESHCONTAINER* pContainer )
{
	return pContainer->MeshData.pMesh->GetNumVertices();
}


// 指定されたボーンが影響を及ぼす頂点数を返す.
int D3DXPARSER::GetNumBoneVertices( MYMESHCONTAINER* pContainer, int iBoneIndex )
{	
	return pContainer->pSkinInfo->GetNumBoneInfluences( iBoneIndex );
}


// 指定されたボーンが影響を及ぼす頂点のインデックスを返す 第2引数は、影響を受ける頂点のインデックスグループ内のインデックス（インデックスが若い順）
// 例えばボーンに影響をうける頂点が４つだとして、そのボーンに影響をうける４つの頂点のうち2番目のインデックスを知りたい場合は、iIndexInGroupに1を指定する（0スタートなので）
DWORD D3DXPARSER::GetBoneVerticesIndices( MYMESHCONTAINER* pContainer, int iBoneIndex, int iIndexInGroup )
{
	int Num = pContainer->pSkinInfo->GetNumBoneInfluences( iBoneIndex );
	DWORD* pVerts	= new DWORD[Num];
	float* pWights	= new float[Num];

	DWORD dwRslt = 0;
	
	if( FAILED(
		pContainer->pSkinInfo->GetBoneInfluence( iBoneIndex, pVerts, pWights ) ))
	{
		MessageBox( NULL, "ボーン影響を受ける頂点見つからない", "error", MB_OK );
	}
	else{
		dwRslt = pVerts[iIndexInGroup];
	}
	
	delete[] pWights;
	delete[] pVerts;

	return dwRslt;
}

// 指定されたボーンが影響を及ぼす頂点のボーンウェイトを返す 第2引数は、影響を受ける頂点のインデックスグループ内のインデックス（インデックスが若い順）
// 例えばボーンに影響をうける頂点が４つだとして、そのボーンに影響をうける４つの頂点のうち2番目の頂点のボーンウェイトを知りたい場合は、iIndexInGroupに1を指定する（0スタートなので）
double D3DXPARSER::GetBoneVerticesWeights( MYMESHCONTAINER* pContainer, int iBoneIndex,int iIndexInGroup )
{
	int Num = pContainer->pSkinInfo->GetNumBoneInfluences( iBoneIndex );
	DWORD* pVerts	= new DWORD[Num];
	float* pWights	= new float[Num];

	double dRslt = 0.0f;

	if( FAILED(
		pContainer->pSkinInfo->GetBoneInfluence( iBoneIndex, pVerts, pWights ) ) )
	{
		MessageBox( NULL, "ボーン影響を受ける頂点見つからない", "error", MB_OK );
	}
	else{
		dRslt = (double)pWights[iIndexInGroup];
	}
	delete[] pWights;
	delete[] pVerts;

	return dRslt;
}


// 座標.
D3DXVECTOR3 D3DXPARSER::GetVertexCoord( MYMESHCONTAINER* pContainer, DWORD Index )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	pContainer->MeshData.pMesh->GetVertexBuffer( &pVB );
	DWORD Stride=pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	D3DXVECTOR3* pCoord = NULL;

	if( SUCCEEDED( pVB->Lock( 0, 0, (VOID**)&pVertices, 0 ) ) )
	{
		pVertices += Index * Stride;
		pCoord = (D3DXVECTOR3*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE( pVB );

	return *pCoord;
}


// 法線.
D3DXVECTOR3 D3DXPARSER::GetNormal( MYMESHCONTAINER* pContainer, DWORD Index )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	pContainer->MeshData.pMesh->GetVertexBuffer( &pVB );
	DWORD Stride=pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	D3DXVECTOR3* pNormal = NULL;

	if( SUCCEEDED( pVB->Lock( 0, 0, (VOID**)&pVertices, 0 ) ) )
	{
		pVertices += Index * Stride;
		pVertices += sizeof( D3DXVECTOR3 );	// 座標分進める.
		pNormal=(D3DXVECTOR3*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE( pVB );

	return *pNormal;
}


// テクスチャー座標.
D3DXVECTOR2 D3DXPARSER::GetUV( MYMESHCONTAINER* pContainer, DWORD Index )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = NULL;
	pContainer->MeshData.pMesh->GetVertexBuffer( &pVB );
	DWORD Stride=pContainer->MeshData.pMesh->GetNumBytesPerVertex();
	BYTE *pVertices = NULL;
	D3DXVECTOR2* pUV = NULL;

	if( SUCCEEDED( pVB->Lock( 0, 0, (VOID**)&pVertices, 0 ) ) )
	{
		pVertices += Index * Stride;
		pVertices += sizeof( D3DXVECTOR3 );	// 座標分進める.
		pVertices += sizeof( D3DXVECTOR3 );	// 法線分進める.
		pUV = (D3DXVECTOR2*)pVertices;
		pVB->Unlock();
	}
	SAFE_RELEASE( pVB );

	return *pUV;
}


// インデックスバッファ内の指定した位置（インデックスインデックス）にある頂点インデックスを返す.
int D3DXPARSER::GetIndex( MYMESHCONTAINER* pContainer, DWORD Index )
{
	WORD VertIndex = 0;
	WORD* pIndex = NULL;
	pContainer->MeshData.pMesh->LockIndexBuffer( D3DLOCK_READONLY, (VOID**)&pIndex );

	VertIndex = pIndex[Index];

	pContainer->MeshData.pMesh->UnlockIndexBuffer();

	return VertIndex;
}


D3DXVECTOR4 D3DXPARSER::GetAmbient( MYMESHCONTAINER* pContainer, int iIndex )
{
	D3DXCOLOR color;
	color = pContainer->pMaterials[iIndex].MatD3D.Ambient;
	return D3DXVECTOR4( color.a, color.r, color.g, color.b );
}


D3DXVECTOR4 D3DXPARSER::GetDiffuse( MYMESHCONTAINER* pContainer, int iIndex )
{
	D3DXCOLOR color;
	color = pContainer->pMaterials[iIndex].MatD3D.Diffuse;
	return D3DXVECTOR4( color.a, color.r, color.g, color.b );
}


D3DXVECTOR4 D3DXPARSER::GetSpecular( MYMESHCONTAINER* pContainer, int iIndex )
{
	D3DXCOLOR color;
	
	color = pContainer->pMaterials[iIndex].MatD3D.Specular;
	return D3DXVECTOR4( color.a, color.r, color.g, color.b );
}


CHAR* D3DXPARSER::GetTexturePath( MYMESHCONTAINER* pContainer, int index )
{
	CHAR* p = NULL;
	p = pContainer->pMaterials[index].pTextureFilename;
	return p;
}



float D3DXPARSER::GetSpecularPower( MYMESHCONTAINER* pContainer, int iIndex )
{
	float power;
	
	power = pContainer->pMaterials[iIndex].MatD3D.Power;
	return power;
}


// そのポリゴンが、どのマテリアルであるかを返す.
int D3DXPARSER::GeFaceMaterialIndex( MYMESHCONTAINER* pContainer, int iFaceIndex )
{
	int MaterialIndex = 0;
	DWORD* pBuf = NULL;
	if( SUCCEEDED(
		pContainer->MeshData.pMesh->LockAttributeBuffer( D3DLOCK_READONLY, &pBuf ) ) )
	{
		MaterialIndex = pBuf[iFaceIndex];
	}
	else
	{
		MessageBoxA( 0, "属性バッファのロック失敗", "", MB_OK );
	}
	return MaterialIndex;
}


// iFaceIndex番目のポリゴンを形成する3頂点の中で、iIndexInFace番目[0,2]の頂点のインデックスを返す.
int D3DXPARSER::GetFaceVertexIndex( MYMESHCONTAINER* pContainer, int iFaceIndex, int iIndexInFace )
{
	// インデックスバッファーを調べれば分かる.
	WORD VertIndex = 0;
	WORD* pIndex = NULL;
	pContainer->MeshData.pMesh->LockIndexBuffer( D3DLOCK_READONLY, (VOID**)&pIndex );

	VertIndex = pIndex[iFaceIndex*3+iIndexInFace];

	pContainer->MeshData.pMesh->UnlockIndexBuffer();

	return VertIndex;
}


D3DXMATRIX D3DXPARSER::GetBindPose( MYMESHCONTAINER* pContainer, int iBoneIndex )
{
	return *pContainer->pSkinInfo->GetBoneOffsetMatrix( iBoneIndex );
}


// この関数をよぶごとにUpdateすると無駄なので、Render関数実行時にUpdateするものとしてそのまま渡す.
// 親クラスがこのクラスのUpdateMatricesを実行しない場合は、UpdateMatricesしないと行列が最新にならないので留意.
D3DXMATRIX D3DXPARSER::GetNewPose( MYMESHCONTAINER* pContainer, int iBoneIndex )
{
	MYMESHCONTAINER* pMyContaiber = (MYMESHCONTAINER*)pContainer;
	return *pMyContaiber->ppBoneMatrix[iBoneIndex];
}



CHAR* D3DXPARSER::GetBoneName( MYMESHCONTAINER* pContainer, int iBoneIndex )
{
	return (CHAR*)pContainer->pSkinInfo->GetBoneName( iBoneIndex );
}



int D3DXPARSER::GetNumBones( MYMESHCONTAINER* pContainer )
{
	return pContainer->pSkinInfo->GetNumBones();
}




// メッシュコンテナを呼び出す(再帰処理)
LPD3DXMESHCONTAINER D3DXPARSER::GetMeshContainer( LPD3DXFRAME pFrame )
{
	LPD3DXMESHCONTAINER pCon = NULL;

	// メッシュコンテナあれば返す.
	if( pFrame->pMeshContainer ){
		return pFrame->pMeshContainer;
	}

	// 無かったら、子のフレームをチェック.
	// そもそも子フレームある？
	if( pFrame->pFrameFirstChild ){
		// あればチェックする.
		pCon = GetMeshContainer( pFrame->pFrameFirstChild );
	}

	// 子のフレーム最下層までチェックしたが見つからなかった.
	if( pCon == NULL ){
		//兄弟のフレームも探す.
		// そもそも兄弟フレームある？
		if( pFrame->pFrameSibling ){
			// あるればチェックする.
			pCon = GetMeshContainer( pFrame->pFrameSibling );
		}
	}

	// 見つからない場合はNULLが入る.
	return pCon;
}


// アニメーションセットの切り替え.
void D3DXPARSER::ChangeAnimSet(int index, LPD3DXANIMATIONCONTROLLER pAC )
{
	D3DXTRACK_DESC TrackDesc;		// アニメーショントラック構造体.
	ZeroMemory( &TrackDesc, sizeof( TrackDesc ) );

	//※以下3つは、ほぼ固定でOK.
	TrackDesc.Weight	= 1;		// 重み.
	TrackDesc.Speed		= 1;		// 速さ.
	TrackDesc.Enable	= 1;		// 有効.

	TrackDesc.Position	= 0.0;		// フレーム位置(開始位置を指定できる)

	LPD3DXANIMATIONCONTROLLER pTmpAC;
	if( pAC ){
		pTmpAC = pAC;
	}
	else{
		pTmpAC = m_pAnimController;
	}

	// 指定(index）のアニメーショントラックに変更.
	pTmpAC->SetTrackDesc( 0, &TrackDesc );
	pTmpAC->SetTrackAnimationSet( 0, m_pAnimSet[index] );
	pTmpAC->SetTrackEnable( index, true );
}


// アニメーションｾｯﾄの切り替え(開始フレーム指定可能版)
//　第２引数に開始したいフレームを指定する.
//	完全停止したい場合は、前後でアニメーション速度も0.0fに指定してやる必要がある.
void D3DXPARSER::ChangeAnimSet_StartPos( int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC )
{
	D3DXTRACK_DESC TrackDesc;		// アニメーショントラック構造体.
	ZeroMemory( &TrackDesc, sizeof( TrackDesc ) );

	//※以下3つは、ほぼ固定でOK.
	TrackDesc.Weight	= 1;		// 重み.
	TrackDesc.Speed		= 1;		// 速さ.
	TrackDesc.Enable	= 1;		// 有効.

	TrackDesc.Position = dStartFramePos;

	LPD3DXANIMATIONCONTROLLER pTmpAC;
	if( pAC ){
		pTmpAC = pAC;
	}
	else{
		pTmpAC = m_pAnimController;
	}

	// 指定(index）のアニメーショントラックに変更.
	pTmpAC->SetTrackDesc( 0, &TrackDesc );
	pTmpAC->SetTrackAnimationSet( 0, m_pAnimSet[index] );
	pTmpAC->SetTrackEnable( index, true );
}


// アニメーション停止時間を取得.
double D3DXPARSER::GetAnimPeriod( int index )
{
	if( index < 0 || MAX_ANIM_SET <= index ){
		return 0.0;
	}
	return m_pAnimSet[index]->GetPeriod();
}

// 最大アニメーション数を取得.
int D3DXPARSER::GetAnimMax( LPD3DXANIMATIONCONTROLLER pAC )
{
	if( pAC ){
		return pAC->GetNumAnimationSets();
	}
	return m_pAnimController->GetNumAnimationSets();
}




// 指定したボーン情報(行列)を取得する関数.
bool D3DXPARSER::GetMatrixFromBone( const char* sBoneName, D3DXMATRIX* const  pOutMat ) const
{
	LPD3DXFRAME pFrame;
	pFrame = (MYFRAME*)D3DXFrameFind( m_pFrameRoot, sBoneName );

	if( pFrame == NULL ){
		return false;
	}

	MYFRAME* pMyFrame = (MYFRAME*)pFrame;
	// 位置情報を取得.
	*pOutMat = pMyFrame->CombinedTransformationMatrix;

	return true;
}


// 指定したボーン情報(座標)を取得する関数.
bool D3DXPARSER::GetPosFromBone( const char* sBoneName, D3DXVECTOR3* const pOutPos ) const
{
	D3DXMATRIX mBone;
	if( !GetMatrixFromBone( sBoneName, &mBone ) ){
		return false;
	}
	pOutPos->x = mBone._41;
	pOutPos->y = mBone._42;
	pOutPos->z = mBone._43;

	return true;
}

//ボーンがあるか無いかを調べる関数.
bool D3DXPARSER::ExistsBone( const char* sBoneName )
{
	LPD3DXFRAME pFrame;
	pFrame = (MYFRAME*)D3DXFrameFind( m_pFrameRoot, sBoneName );

	if( pFrame == NULL ){
		return false;
	}
	return true;
}



// 一括解放処理.
HRESULT D3DXPARSER::Release()
{
	// 作成したものを最後に作ったものから順解放していく.
	// 基本的には new したものを delete していく.

	// アニメーションセットの解放.
	DWORD iAnimMax = m_pAnimController->GetNumAnimationSets();
	for( DWORD i=0; i<iAnimMax;	i++ )
	{
		SAFE_RELEASE( m_pAnimSet[i] );
	}

	// アニメーションコントローラ削除.
	SAFE_RELEASE( m_pAnimController );

	// フレームとメッシュコンテナの削除.
	m_pHierarchy->DestroyFrame( m_pFrameRoot );

	// 解放処理いる？
	if( m_pFrameRoot ){
		m_pFrameRoot = NULL;
	}

	// Hierarchy削除.
	if( m_pHierarchy ){
		delete m_pHierarchy;
		m_pHierarchy = NULL;
	}

	return S_OK;
}


/******************************************************************************************************************************************
*
*	以降、スキンメッシュクラス.
*
**/
// コンストラクタ.
clsD3DXSKINMESH::clsD3DXSKINMESH( 
	const HWND hWnd, 
	ID3D11Device* const pDevice11,
	ID3D11DeviceContext* const pContext11, 
	const char* sFileName )
	:m_hWnd( hWnd )
	,m_pDevice( pDevice11 )
	,m_pDeviceContext( pContext11 )
//	,m_pD3d9( NULL )
//	,m_pDevice9( NULL )
	,m_pSampleLinear( NULL )
	,m_pVertexShader( NULL )
	,m_pPixelShader( NULL )
	,m_pVertexLayout( NULL )
	,m_pConstantBuffer0( NULL )
	,m_pConstantBuffer1( NULL )
	,m_pConstantBufferBone( NULL )
	,m_pD3dxMesh( NULL )
	,m_pBlendState()
	,m_pMaskBase( nullptr )
	,m_pMaskArmor( nullptr )
{	 
	LPDIRECT3DDEVICE9	pDevice9 = NULL;

	if( FAILED( CreateBlendState() ) ){
		assert( !"CreateBlendState()" );
	}

	// Dx9 のデバイス関係を作成する.
	if( FAILED( CreateDeviceDx9( m_hWnd, &pDevice9 ) ) ){
		assert( !"CreateDeviceDx9()" );
	}
	// シェーダの作成.
	if( FAILED( InitShader() ) ){
		assert( !"InitShader()" );
	}
	//モデルの作成.
	if( FAILED( CreateFromX( const_cast<CHAR*>( sFileName ), pDevice9 ) ) ){
		assert( !"CreateFromX()" );
	}

	SAFE_RELEASE( pDevice9 );


	m_Trans.vPos = vecAxisX = vecAxisZ = m_vLight = m_vEye = { 0.0f, 0.0f, 0.0f };
	m_Trans.fPitch = m_Trans.fYaw = m_Trans.fRoll = 0.0f;
	m_Trans.vScale = D3DXVECTOR3( 1.0f, 1.0f, 1.0f );

	m_enDir = enDirection::enDirection_Stop;

	D3DXMatrixIdentity( &m_mWorld );
	D3DXMatrixIdentity( &m_mRotation );
	D3DXMatrixIdentity( &m_mView );
	D3DXMatrixIdentity( &m_mProj );

	// 一先ず、この値.
	m_dAnimSpeed = 0.0001;
	m_dAnimTime = 0.0;

	ZeroMemory( m_FilePath, sizeof( m_FilePath ) );

	m_iFrame = 0;
}


// デストラクタ.
clsD3DXSKINMESH::~clsD3DXSKINMESH()
{
	SAFE_DELETE( m_pMaskArmor );
	SAFE_DELETE( m_pMaskBase );

	// 解放処理.
	Release();


	// シェーダやサンプラ関係.
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexShader );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexLayout );

	// コンスタントバッファ関係.
	SAFE_RELEASE( m_pConstantBufferBone );
	SAFE_RELEASE( m_pConstantBuffer1 );
	SAFE_RELEASE( m_pConstantBuffer0 );

	// Dx9 デバイス関係.
//	SAFE_RELEASE( m_pDevice9 );
//	SAFE_RELEASE( m_pD3d9 );

	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
		SAFE_RELEASE( m_pBlendState[i] );
	}

	// Dx11 デバイス関係.
	m_pDeviceContext = NULL;
	m_pDevice = NULL;
	m_hWnd = NULL;
}



// Dx9のデバイス・デバイスコンテキストの作成.
HRESULT clsD3DXSKINMESH::CreateDeviceDx9( HWND hWnd, LPDIRECT3DDEVICE9* ppOutDevice9 )
{
	// D3d"9"のデバイスを作る、全てはD3DXMESHの引数に必要だから.
	LPDIRECT3D9 pD3d9 = NULL;
	// Direct3D"9"オブジェクトの作成.
	if( NULL == ( pD3d9 = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		MessageBox( NULL, "Direct3D9の作成に失敗しました", "", MB_OK );
		return E_FAIL;
	}

	// DIRECT3D"9"デバイスオブジェクトの作成
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	if( FAILED( pD3d9->CreateDevice( 
					D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
					D3DCREATE_HARDWARE_VERTEXPROCESSING,
					&d3dpp, ppOutDevice9 ) ) )
	{
		if( FAILED( pD3d9->CreateDevice( 
					D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, ppOutDevice9 ) ) )
		{
			MessageBox( 0,"HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します",NULL,MB_OK);
			if( FAILED( pD3d9->CreateDevice( 
					D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
					D3DCREATE_HARDWARE_VERTEXPROCESSING,
					&d3dpp, ppOutDevice9 ) ) )
			{
				if( FAILED( pD3d9->CreateDevice( 
					D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, ppOutDevice9 ) ) )
				{
					MessageBox( 0,"DIRECT3Dデバイスの作成に失敗しました",NULL,MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	SAFE_RELEASE( pD3d9 );

	return S_OK;
}

// シェーダ初期化.
HRESULT	clsD3DXSKINMESH::InitShader()
{
	//D3D11関連の初期化
	ID3DBlob *pCompiledShader = NULL;
	ID3DBlob *pErrors = NULL;
	UINT	uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag = D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//ブロブからバーテックスシェーダー作成
	if( FAILED( D3DX11CompileFromFile( SHADER_NAME, NULL, NULL, "VSSkin", "vs_5_0", uCompileFlag, 0, NULL, &pCompiledShader, &pErrors, NULL ) ) )
	{
		int size = pErrors->GetBufferSize();
		char* ch = (char*)pErrors->GetBufferPointer();
		MessageBox( 0, "hlsl読み込み失敗", NULL, MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	if( FAILED(
		m_pDevice->CreateVertexShader( pCompiledShader->GetBufferPointer(), pCompiledShader->GetBufferSize(), NULL, &m_pVertexShader ) ) )
	{
		SAFE_RELEASE(pCompiledShader);
		MessageBox( 0, "バーテックスシェーダー作成失敗", NULL, MB_OK );
		return E_FAIL;
	}
	//頂点インプットレイアウトを定義	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 }, 
		{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_INDEX",	0, DXGI_FORMAT_R32G32B32A32_UINT,	0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BONE_WEIGHT",0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, 48, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

	//頂点インプットレイアウトを作成
	if( FAILED(
		m_pDevice->CreateInputLayout(
			layout, numElements, pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(), &m_pVertexLayout ) ) )
	{
		return FALSE;
	}
	//頂点インプットレイアウトをセット
	m_pDeviceContext->IASetInputLayout( m_pVertexLayout );

	//ブロブからピクセルシェーダー作成
	if(FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME, NULL, NULL,
			"PSSkin", "ps_5_0",
			uCompileFlag, 0, NULL,
			&pCompiledShader, &pErrors, NULL ) ) )
	{
		MessageBox( 0, "hlsl読み込み失敗", NULL, MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );
	if( FAILED(
		m_pDevice->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			NULL, &m_pPixelShader ) ) )
	{
		SAFE_RELEASE( pCompiledShader );
		MessageBox( 0, "ピクセルシェーダー作成失敗", NULL, MB_OK );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	D3D11_BUFFER_DESC cb;
	// コンスタントバッファー0作成.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof( SHADER_SKIN_GLOBAL0 );
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage=D3D11_USAGE_DYNAMIC;
	if( FAILED(
		m_pDevice->CreateBuffer( &cb, NULL, &m_pConstantBuffer0 ) ) )
	{
		return E_FAIL;
	}

	// コンスタントバッファー1作成.
	cb.BindFlags= D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth= sizeof( SHADER_SKIN_GLOBAL1 );
	cb.CPUAccessFlags=D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags =0;
	cb.StructureByteStride = 0;
	cb.Usage=D3D11_USAGE_DYNAMIC;
	if( FAILED(
		m_pDevice->CreateBuffer( &cb, NULL, &m_pConstantBuffer1 ) ) )
	{
		return E_FAIL;
	}

	// コンスタントバッファーボーン用作成.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.ByteWidth = sizeof( SHADER_GLOBAL_BONES );
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;
	cb.Usage = D3D11_USAGE_DYNAMIC;
	if( FAILED(
		m_pDevice->CreateBuffer( &cb, NULL, &m_pConstantBufferBone ) ) )
	{
		return E_FAIL;
	}

	// テクスチャー用サンプラー作成.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory( &SamDesc, sizeof( D3D11_SAMPLER_DESC ) );

	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	if( FAILED(
		m_pDevice->CreateSamplerState( &SamDesc, &m_pSampleLinear ) ) )
	{
		return E_FAIL;
	}

	return S_OK;
}



// Xファイルからスキン関連の情報を読み出す関数.
HRESULT clsD3DXSKINMESH::ReadSkinInfo( MYMESHCONTAINER* pContainer, MY_SKINVERTEX* pvVB, SKIN_PARTS_MESH* pParts )
{
	// Xファイルから抽出すべき情報は、
	// 「頂点ごとのﾎﾞｰﾝｲﾝﾃﾞｯｸｽ」「頂点ごとのボーンウェイト」.
	// 「バインド行列」「ポーズ行列」の4項目.

	int i, k, m, n;	// 各種ループ変数.
	int iNumVertex	= 0;	// 頂点数.
	int iNumBone	= 0;	// ボーン数.

	// 頂点数.
	iNumVertex	= m_pD3dxMesh->GetNumVertices( pContainer );
	// ボーン数.
	iNumBone	= m_pD3dxMesh->GetNumBones( pContainer );

	// それぞれのボーンに影響を受ける頂点を調べる.
	// そこから逆に、頂点ベースでボーンインデックス・重みを整頓する.
	for( i=0; i<iNumBone; i++ )
	{
		// このボーンに影響を受ける頂点数.
		int iNumIndex
			= m_pD3dxMesh->GetNumBoneVertices( pContainer, i);

		int*	pIndex = new int[iNumIndex];
		double*	pWeight= new double[iNumIndex];

		for( k=0; k<iNumIndex; k++ )
		{
			pIndex[k]
				= m_pD3dxMesh->GetBoneVerticesIndices( pContainer, i, k );
			pWeight[k]
				= m_pD3dxMesh->GetBoneVerticesWeights( pContainer, i, k );
		}

		// 頂点側からインデックスをたどって、頂点サイドで整理する.
		for( k=0; k<iNumIndex; k++ )
		{
			// XファイルやCGソフトがボーン4本以内とは限らない.
			// 5本以上の場合は、重みの大きい順に4本に絞る.

			// ウェイトの大きさ順にソート(バブルソート)
			for( m=4; m>1; m-- )
			{
				for( n=1; n<m; n++ )
				{
					if( pvVB[pIndex[k]].bBoneWeight[n-1] <
						pvVB[pIndex[k]].bBoneWeight[n] )
					{
						float tmp = pvVB[pIndex[k]].bBoneWeight[n-1];
						pvVB[pIndex[k]].bBoneWeight[n-1] = pvVB[pIndex[k]].bBoneWeight[n];
						pvVB[pIndex[k]].bBoneWeight[n]	= tmp;
						int itmp = pvVB[pIndex[k]].bBoneIndex[n-1];
						pvVB[pIndex[k]].bBoneIndex[n-1] = pvVB[pIndex[k]].bBoneIndex[n];
						pvVB[pIndex[k]].bBoneIndex[n]	= itmp;
					}
				}
			}
			// ソート後は、最後の要素に一番小さいウェイトが入ってるはず.
			bool flag = false;
			for( m=0; m<4; m++ )
			{
				if( pvVB[pIndex[k]].bBoneWeight[m] == 0 )
				{
					flag = true;
					pvVB[pIndex[k]].bBoneIndex[ m] = i;
					pvVB[pIndex[k]].bBoneWeight[m] = (float)pWeight[k];
					break;
				}
			}
			if( flag == false )
			{
				pvVB[pIndex[k]].bBoneIndex[ 3] = i;
				pvVB[pIndex[k]].bBoneWeight[3] = (float)pWeight[k];
				break;
			}
		}
		// 使い終われば削除する.
		delete[] pIndex;
		delete[] pWeight;
	}

	// ボーン生成.
	pParts->iNumBone	= iNumBone;
	pParts->pBoneArray	= new BONE[iNumBone];
	// ポーズ行列を得る(初期ポーズ)
	for( i=0; i<pParts->iNumBone; i++ )
	{
		pParts->pBoneArray[i].mBindPose
			= m_pD3dxMesh->GetBindPose( pContainer, i );
	}

	return S_OK;
}


// Xからスキンメッシュを作成する　　注意）素材（X)のほうは、三角ポリゴンにすること.
HRESULT clsD3DXSKINMESH::CreateFromX( CHAR* szFileName, LPDIRECT3DDEVICE9 pDevice9 )
{
	if( !pDevice9 ) return E_FAIL;

	// ファイル名をパスごと取得.
	strcpy_s( m_FilePath, sizeof( m_FilePath ), szFileName );

	// Xファイル読み込み.
	m_pD3dxMesh = new D3DXPARSER;
	m_pD3dxMesh->LoadMeshFromX( pDevice9, szFileName );


	// 全てのメッシュを作成する.
	BuildAllMesh( m_pD3dxMesh->m_pFrameRoot );

	return S_OK;
}


// Direct3Dのインデックスバッファー作成.
HRESULT clsD3DXSKINMESH::CreateIndexBuffer( DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer )
{
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = dwSize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pIndex;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	if( FAILED( m_pDevice->CreateBuffer( &bd, &InitData, ppIndexBuffer ) ) )
	{
		return FALSE;
	}
	
	return S_OK;
}


// レンダリング.
void clsD3DXSKINMESH::Render(
	const D3DXMATRIX& mView, const D3DXMATRIX& mProj,
	const D3DXVECTOR3& vLight, const D3DXVECTOR3& vEye,
	const D3DXVECTOR4& vColorBase,
	const D3DXVECTOR4& vColorArmor,
	const bool isAlpha, 
	LPD3DXANIMATIONCONTROLLER pAC )
{
	m_mView		= mView;
	m_mProj		= mProj;
	m_vLight	= vLight;
	m_vEye		= vEye;

	if (pAC == NULL)
	{
		if (m_pD3dxMesh->m_pAnimController)
		{
			m_pD3dxMesh->m_pAnimController->AdvanceTime(m_dAnimTime, NULL);
		}
	}

	D3DXMATRIX m;
	D3DXMatrixIdentity( &m );
	m_pD3dxMesh->UpdateFrameMatrices( m_pD3dxMesh->m_pFrameRoot, &m );
	DrawFrame( m_pD3dxMesh->m_pFrameRoot, vColorBase, vColorArmor, isAlpha );
}


//全てのメッシュを作成する.
void clsD3DXSKINMESH::BuildAllMesh( D3DXFRAME* pFrame )
{
	if( pFrame && pFrame->pMeshContainer )
	{
		CreateAppMeshFromD3DXMesh( pFrame );
	}

	// 再帰関数.
	if( pFrame->pFrameSibling != NULL )
	{
		BuildAllMesh( pFrame->pFrameSibling );
	}
	if( pFrame->pFrameFirstChild != NULL )
	{
		BuildAllMesh( pFrame->pFrameFirstChild );
	}
}

// メッシュ作成.
HRESULT clsD3DXSKINMESH::CreateAppMeshFromD3DXMesh( LPD3DXFRAME p )
{
	MYFRAME* pFrame = (MYFRAME*)p;

//	LPD3DXMESH pD3DXMesh = pFrame->pMeshContainer->MeshData.pMesh;//D3DXﾒｯｼｭ(ここから・・・)
	MYMESHCONTAINER* pContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	// ｱﾌﾟﾘﾒｯｼｭ(・・・ここにメッシュデータをコピーする)
	SKIN_PARTS_MESH* pAppMesh = new SKIN_PARTS_MESH;
	pAppMesh->bTex = false;

	// 事前に頂点数、ポリゴン数等を調べる.
	pAppMesh->dwNumVert	= m_pD3dxMesh->GetNumVertices( pContainer );
	pAppMesh->dwNumFace	= m_pD3dxMesh->GetNumFaces( pContainer );
	pAppMesh->dwNumUV	= m_pD3dxMesh->GetNumUVs( pContainer );
	// Direct3DではUVの数だけ頂点が必要.
	if( pAppMesh->dwNumVert < pAppMesh->dwNumUV ){
		// 共有頂点等で、頂点が足りないとき.
		MessageBox( NULL,
			"Direct3Dは、UVの数だけ頂点が必要です(UVを置く場所が必要です)テクスチャは正しく貼られないと思われます",
			"Error", MB_OK );
		return E_FAIL;
	}
	// 一時的なメモリ確保(頂点バッファとインデックスバッファ)
	MY_SKINVERTEX* pvVB = new MY_SKINVERTEX[pAppMesh->dwNumVert];
	int* piFaceBuffer	= new int[pAppMesh->dwNumFace*3];
		// 3頂点ポリゴンなので、1フェイス=3頂点(3インデックス)

	//頂点読み込み.
	for( DWORD i=0; i<pAppMesh->dwNumVert; i++ ){
		D3DXVECTOR3 v	= m_pD3dxMesh->GetVertexCoord( pContainer, i );
		pvVB[i].vPos.x	= v.x;
		pvVB[i].vPos.y	= v.y;
		pvVB[i].vPos.z	= v.z;
	}
	// ポリゴン情報(頂点インデックス)読み込み.
	for( DWORD i=0; i<pAppMesh->dwNumFace*3; i++ ){
		piFaceBuffer[i] = m_pD3dxMesh->GetIndex( pContainer, i );
	}
	// 法線読み込み.
	for( DWORD i=0; i<pAppMesh->dwNumVert; i++ ){
		D3DXVECTOR3 v	= m_pD3dxMesh->GetNormal( pContainer, i );
		pvVB[i].vNorm.x	= v.x;
		pvVB[i].vNorm.y	= v.y;
		pvVB[i].vNorm.z	= v.z;
	}
	// テクスチャ座標読み込み.
	for( DWORD i=0; i<pAppMesh->dwNumVert; i++ ){
		D3DXVECTOR2 v	= m_pD3dxMesh->GetUV( pContainer, i );
		pvVB[i].vTex.x	= v.x;
		pvVB[i].vTex.y	= v.y;
	}
	// マテリアル読み込み.
	pAppMesh->dwNumMaterial	= m_pD3dxMesh->GetNumMaterials( pContainer );
	pAppMesh->pMaterial		= new MY_SKINMATERIAL[pAppMesh->dwNumMaterial];

	// マテリアルの数だけインデックスバッファを作成.
	pAppMesh->ppIndexBuffer = new ID3D11Buffer*[pAppMesh->dwNumMaterial];
			// 掛け算ではなく「ID3D11Buffer*」の配列という意味.
	for( DWORD i=0; i<pAppMesh->dwNumMaterial; i++ )
	{
		// 環境光(アンビエント)
		pAppMesh->pMaterial[i].Ka.x	= m_pD3dxMesh->GetAmbient( pContainer, i ).y;
		pAppMesh->pMaterial[i].Ka.y	= m_pD3dxMesh->GetAmbient( pContainer, i ).z;
		pAppMesh->pMaterial[i].Ka.z	= m_pD3dxMesh->GetAmbient( pContainer, i ).w;
		pAppMesh->pMaterial[i].Ka.w	= m_pD3dxMesh->GetAmbient( pContainer, i ).x;
		// 拡散反射光(ディフューズ)
		pAppMesh->pMaterial[i].Kd.x	= m_pD3dxMesh->GetDiffuse( pContainer, i ).y;
		pAppMesh->pMaterial[i].Kd.y	= m_pD3dxMesh->GetDiffuse( pContainer, i ).z;
		pAppMesh->pMaterial[i].Kd.z	= m_pD3dxMesh->GetDiffuse( pContainer, i ).w;
		pAppMesh->pMaterial[i].Kd.w	= m_pD3dxMesh->GetDiffuse( pContainer, i ).x;
		// 鏡面反射光(スペキュラ)
		pAppMesh->pMaterial[i].Ks.x	= m_pD3dxMesh->GetSpecular( pContainer, i ).y;
		pAppMesh->pMaterial[i].Ks.y	= m_pD3dxMesh->GetSpecular( pContainer, i ).z;
		pAppMesh->pMaterial[i].Ks.z	= m_pD3dxMesh->GetSpecular( pContainer, i ).w;
		pAppMesh->pMaterial[i].Ks.w	= m_pD3dxMesh->GetSpecular( pContainer, i ).x;

		// テクスチャ(ディフューズテクスチャのみ)
		char* name = m_pD3dxMesh->GetTexturePath( pContainer, i );
		if( name ){
			char* ret = strrchr( m_FilePath, '\\' );
			if( ret != NULL ){
				int check = ret - m_FilePath;
				char path[512];
				strcpy_s( path, 512, m_FilePath );
				path[check+1] = '\0';

				strcat_s( path, sizeof( path ), name );
				strcpy_s( pAppMesh->pMaterial[i].szTextureName,
					sizeof( pAppMesh->pMaterial[i].szTextureName ),
					path );
			}
		}
		// テクスチャを作成.
		if( pAppMesh->pMaterial[i].szTextureName[0] != 0
			&& FAILED(
				D3DX11CreateShaderResourceViewFromFileA(
				m_pDevice, pAppMesh->pMaterial[i].szTextureName,
				NULL, NULL, &pAppMesh->pMaterial[i].pTexture, NULL )))
		{
			MessageBox( NULL, "テクスチャ読み込み失敗",
				"Error", MB_OK );
			return E_FAIL;
		}
		// そのマテリアルであるインデックス配列内の開始インデックスを調べる.
		// さらにインデックスの個数を調べる.
		int iCount = 0;
		int* pIndex = new int[pAppMesh->dwNumFace*3];
			// とりあえず、メッシュ内のポリゴン数でﾒﾓﾘ確保.
			// (ここのぽりごんぐるーぷは必ずこれ以下になる)

		for( DWORD k=0; k<pAppMesh->dwNumFace; k++ )
		{
			// もし i==k 番目のポリゴンのマテリアル番号なら.
			if( i == m_pD3dxMesh->GeFaceMaterialIndex( pContainer, k ) )
			{
				// k番目のポリゴンを作る頂点のインデックス.
				pIndex[iCount]
					= m_pD3dxMesh->GetFaceVertexIndex( pContainer, k, 0 );	// 1個目.
				pIndex[iCount+1]
					= m_pD3dxMesh->GetFaceVertexIndex( pContainer, k, 1 );	// 2個目.
				pIndex[iCount+2]
					= m_pD3dxMesh->GetFaceVertexIndex( pContainer, k, 2 );	// 3個目.
				iCount += 3;
			}
		}
		if( iCount > 0 ){
			// インデックスバッファ作成.
			CreateIndexBuffer( iCount*sizeof(int),
				pIndex, &pAppMesh->ppIndexBuffer[i] );
		}
		else{
			// 解放時の処理に不具合が出たため.
			// カウント数が0以下の場合は、インデックスバッファを NULL にしておく.
			pAppMesh->ppIndexBuffer[i] = NULL;
		}

		// そのマテリアル内のポリゴン数.
		pAppMesh->pMaterial[i].dwNumFace = iCount / 3;
		// 不要になったので削除.
		delete[] pIndex;
	}

	// スキン情報ある？
	if( pContainer->pSkinInfo == NULL ){
	/*	char strDbg[128];
		sprintf_s( strDbg, "ContainerName:[%s]", pContainer->Name );
		MessageBox( NULL, strDbg, "Not SkinInfo", MB_OK );*/
		pAppMesh->bEnableBones = false;
	}
	else{
		// スキン情報(ジョイント、ウェイト)読み込み.
		ReadSkinInfo( pContainer, pvVB, pAppMesh );
	}

	// バーテックスバッファを作成.
	D3D11_BUFFER_DESC bd;
	bd.Usage	= D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( MY_SKINVERTEX ) * ( pAppMesh->dwNumVert );
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags= 0;
	bd.MiscFlags	= 0;
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = pvVB;

	HRESULT hRslt = S_OK;
	if( FAILED(
		m_pDevice->CreateBuffer(
			&bd, &InitData, &pAppMesh->pVertexBuffer ) ) )
	{
		hRslt = E_FAIL;
	}

	// パーツメッシュに設定.
	pFrame->pPartsMesh = pAppMesh;

	// 一時的な入れ物は不要なるので削除.
	if( piFaceBuffer ){
		delete[] piFaceBuffer;
	}
	if( pvVB ){
		delete[] pvVB;
	}


	//========== マスク作成 ==========//.
	//----- アーマー -----//.
	if( !m_pMaskArmor ){
		std::string name = sMASK_PATH_0;
		if( name.size() ){
			char* ret = strrchr( m_FilePath, '\\' );
			if( ret != NULL ){
				int check = ret - m_FilePath;
				char path[512];
				strcpy_s( path, 512, m_FilePath );
				path[check+1] = '\0';

				strcat_s( path, sizeof( path ), name.c_str() );
				name = path;
			}
		}
		m_pMaskArmor = new MASK_TEXTURE;
		//テクスチャ作成.
		if( FAILED( D3DX11CreateShaderResourceViewFromFileA(
			m_pDevice, 
			name.c_str(),//テクスチャファイル名.
			NULL, NULL,
			&m_pMaskArmor->pTex, //(out)テクスチャオブジェクト.
			NULL ) ) )
		{
//			MessageBox(NULL, "マスク", "テクスチャ作成失敗", MB_OK);
//			return E_FAIL;
		}
	}
	//----- ベース -----//.
	if( !m_pMaskBase ){
		std::string name = sMASK_PATH_1;
		if( name.size() ){
			char* ret = strrchr( m_FilePath, '\\' );
			if( ret != NULL ){
				int check = ret - m_FilePath;
				char path[512];
				strcpy_s( path, 512, m_FilePath );
				path[check+1] = '\0';

				strcat_s( path, sizeof( path ), name.c_str() );
				name = path;
			}
		}
		m_pMaskBase = new MASK_TEXTURE;
		//テクスチャ作成.
		if( FAILED( D3DX11CreateShaderResourceViewFromFileA(
			m_pDevice, 
			name.c_str(),//テクスチャファイル名.
			NULL, NULL,
			&m_pMaskBase->pTex, //(out)テクスチャオブジェクト.
			NULL ) ) )
		{
//			MessageBox(NULL, "マスク", "テクスチャ作成失敗", MB_OK);
//			return E_FAIL;
		}
	}

	return hRslt;
}


// ボーンを次のポーズ位置にセットする関数.
void clsD3DXSKINMESH::SetNewPoseMatrices( SKIN_PARTS_MESH* pParts, int frame, MYMESHCONTAINER* pContainer )
{
	//望むフレームでUpdateすること.
	//しないと行列が更新されない.
	// m_pD3dxMesh->UpdateFrameMatrices(
	//  m_pD3dxMesh->m_pFrameRoot)をレンダリング時に実行すること.

	// また、アニメーション時間に見合った行列を更新するのはD3DXMESHでは
	// アニメーションコントローラが(裏で)やってくれるものなので、
	// アニメーションコントローラを使ってアニメを進行させることも必要.
	// m_pD3dxMesh->m_pAnimController->AdvanceTime(...)を.
	// レンダリング時に実行すること.

	if( pParts->iNumBone <= 0 ){
		//ボーンが 0　以下の場合.
	}

	for( int i=0; i<pParts->iNumBone; i++ )
	{
		pParts->pBoneArray[i].mNewPose
			= m_pD3dxMesh->GetNewPose( pContainer, i );
	}
}


// 次の(現在の)ポーズ行列を返す関数.
D3DXMATRIX clsD3DXSKINMESH::GetCurrentPoseMatrix( SKIN_PARTS_MESH* pParts, int index )
{
	D3DXMATRIX ret =
		pParts->pBoneArray[index].mBindPose * pParts->pBoneArray[index].mNewPose;
	return ret;
}


// フレームの描画.
VOID clsD3DXSKINMESH::DrawFrame(
	LPD3DXFRAME p,
	const D3DXVECTOR4& vColorBase,
	const D3DXVECTOR4& vColorArmor,
	const bool isAlpha )
{
	MYFRAME*			pFrame	= (MYFRAME*)p;
	SKIN_PARTS_MESH*	pPartsMesh	= pFrame->pPartsMesh;
	MYMESHCONTAINER*	pContainer	= (MYMESHCONTAINER*)pFrame->pMeshContainer;

	if( pPartsMesh != NULL )
	{
		DrawPartsMesh(
			pPartsMesh, 
			pFrame->CombinedTransformationMatrix,
			pContainer,
			vColorBase, 
			vColorArmor,
			isAlpha );
	}

	//再帰関数.
	//(兄弟)
	if( pFrame->pFrameSibling != NULL )
	{
		DrawFrame( pFrame->pFrameSibling, vColorBase, vColorArmor, isAlpha );
	}
	//(親子)
	if( pFrame->pFrameFirstChild != NULL )
	{
		DrawFrame( pFrame->pFrameFirstChild, vColorBase, vColorArmor, isAlpha );
	}
}


// パーツメッシュを描画.
void clsD3DXSKINMESH::DrawPartsMesh( 
	SKIN_PARTS_MESH* pMesh, 
	D3DXMATRIX World, 
	MYMESHCONTAINER* const pContainer,
	const D3DXVECTOR4& vColorBase,
	const D3DXVECTOR4& vColorArmor,
	const bool isAlpha )
{
	D3D11_MAPPED_SUBRESOURCE pData;

	//使用するシェーダのセット.
	m_pDeviceContext->VSSetShader(
		m_pVertexShader, NULL, 0 );
	m_pDeviceContext->PSSetShader(
		m_pPixelShader, NULL, 0 );


	D3DXMATRIX	Scale, Yaw, Pitch, Roll, Tran;
	// 拡縮.
	D3DXMatrixScaling( &Scale, m_Trans.vScale.x, m_Trans.vScale.y, m_Trans.vScale.z );
	//回転.
	D3DXMatrixRotationY( &Yaw, m_Trans.fYaw );		// Y軸回転.
	D3DXMatrixRotationX( &Pitch, m_Trans.fPitch );	// X軸回転.
	D3DXMatrixRotationZ( &Roll, m_Trans.fRoll );		// Z軸回転.

	//=================================================================//

	// 回転行列(合成)
	m_mRotation = Roll * Pitch * Yaw;
	// 平行移動.
	D3DXMatrixTranslation( &Tran, m_Trans.vPos.x, m_Trans.vPos.y, m_Trans.vPos.z );
	// ワールド行列.
	m_mWorld = Scale * m_mRotation * Tran;


	// アニメーションフレームを進める スキンを更新.
	m_iFrame++;
	if( m_iFrame >= 3600 ){
		m_iFrame = 0;
	}
	SetNewPoseMatrices( pMesh, m_iFrame, pContainer );

	if( SUCCEEDED(
		m_pDeviceContext->Map(
			m_pConstantBufferBone, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		SHADER_GLOBAL_BONES sg;
		for( int i=0; i<pMesh->iNumBone; i++ )
		{
			D3DXMATRIX mat = GetCurrentPoseMatrix( pMesh, i );
			D3DXMatrixTranspose( &mat, &mat );
			sg.mBone[i] = mat;
		}
		memcpy_s( pData.pData, pData.RowPitch,
			(void*)&sg, sizeof( SHADER_GLOBAL_BONES ) );
		m_pDeviceContext->Unmap( m_pConstantBufferBone, 0 );
	}
	//使用するシェーダをセット.
	m_pDeviceContext->VSSetConstantBuffers(	2, 1, &m_pConstantBufferBone );
	m_pDeviceContext->PSSetConstantBuffers(	2, 1, &m_pConstantBufferBone );
	
	// バーテックスバッファをセット.
	UINT stride = sizeof( MY_SKINVERTEX );
	UINT offset = 0;
	m_pDeviceContext->IASetVertexBuffers(
		0, 1, &pMesh->pVertexBuffer, &stride, & offset );

	// 頂点インプットレイアウトをセット.
	m_pDeviceContext->IASetInputLayout(
		m_pVertexLayout );

	// プリミティブ・トポロジーをセット.
	m_pDeviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// カメラ位置をシェーダに渡す
	if( SUCCEEDED(
		m_pDeviceContext->Map(
			m_pConstantBuffer0, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) )
	{
		SHADER_SKIN_GLOBAL0 sg;
		sg.vLightDir= D3DXVECTOR4( m_vLight.x, m_vLight.y, m_vLight.z, 0 );
		sg.vEye		= D3DXVECTOR4( m_vEye.x, m_vEye.y, m_vEye.z, 0 );
		memcpy_s( pData.pData, pData.RowPitch,
			(void*)&sg, sizeof( SHADER_SKIN_GLOBAL0 ) );
		m_pDeviceContext->Unmap(
			m_pConstantBuffer0, 0 );
	}
	m_pDeviceContext->VSSetConstantBuffers(	0, 1, &m_pConstantBuffer0 );
	m_pDeviceContext->PSSetConstantBuffers(	0, 1, &m_pConstantBuffer0 );

	//透過.
	SetBlend( isAlpha );
	
	// マテリアルの数だけ、
	// それぞれのマテリアルのインデックスバッファを描画.
	for( DWORD i=0; i<pMesh->dwNumMaterial; i++ )
	{
		// 使用されていないマテリアル対策.
		if( pMesh->pMaterial[i].dwNumFace == 0 )
		{
			continue;
		}
		// インデックスバッファをセット.
		stride	= sizeof( int );
		offset	= 0;
		m_pDeviceContext->IASetIndexBuffer(
			pMesh->ppIndexBuffer[i],
			DXGI_FORMAT_R32_UINT, 0 );

		// マテリアルの各要素と変換行列をシェーダに渡す.
		D3D11_MAPPED_SUBRESOURCE pDat;
		if( SUCCEEDED(
			m_pDeviceContext->Map(
				m_pConstantBuffer1, 0,
				D3D11_MAP_WRITE_DISCARD, 0, &pDat )))
		{
			SHADER_SKIN_GLOBAL1 sg;
			sg.mW	= m_mWorld;
			D3DXMatrixTranspose( &sg.mW, &sg.mW );

			sg.mWVP	= m_mWorld * m_mView * m_mProj;
			D3DXMatrixTranspose( &sg.mWVP, &sg.mWVP );

			sg.vAmbient	= pMesh->pMaterial[i].Ka;
			sg.vDiffuse	= pMesh->pMaterial[i].Kd;
			sg.vSpecular= pMesh->pMaterial[i].Ks;
			sg.vColorBase= vColorBase;
			sg.vColorArmor= vColorArmor;

			memcpy_s( pDat.pData, pDat.RowPitch,
				(void*)&sg, sizeof( SHADER_SKIN_GLOBAL1 ));

			m_pDeviceContext->Unmap(
				m_pConstantBuffer1, 0 );
		}
		m_pDeviceContext->VSSetConstantBuffers(	1, 1, &m_pConstantBuffer1 );
		m_pDeviceContext->PSSetConstantBuffers(	1, 1, &m_pConstantBuffer1 );

		// テクスチャをシェーダに渡す.
		if( pMesh->pMaterial[i].szTextureName[0] != NULL )
		{
			UINT slot = 0;
			//テクスチャ.
			m_pDeviceContext->PSSetSamplers( 
				slot, 1, &m_pSampleLinear );
			m_pDeviceContext->PSSetShaderResources( 
				slot, 1, &pMesh->pMaterial[i].pTexture );
			slot ++;
			if( m_pMaskArmor->pTex ){
				//ベース.
				m_pDeviceContext->PSSetSamplers( 
					slot, 1, &m_pSampleLinear );
				m_pDeviceContext->PSSetShaderResources( 
					slot, 1, &m_pMaskArmor->pTex );
			}
			slot ++;
			if( m_pMaskBase->pTex ){
				//アーマー.
				m_pDeviceContext->PSSetSamplers( 
					slot, 1, &m_pSampleLinear );
				m_pDeviceContext->PSSetShaderResources( 
					slot, 1, &m_pMaskBase->pTex );
			}
		}
		else
		{
			ID3D11ShaderResourceView* Nothing[1] = { 0 };
			m_pDeviceContext->PSSetShaderResources( 0, 1, Nothing );
		}
		// Draw.
		m_pDeviceContext->DrawIndexed( pMesh->pMaterial[i].dwNumFace * 3, 0, 0 );
	}
}

//ブレンドステート作成.
HRESULT clsD3DXSKINMESH::CreateBlendState()
{
	//アルファブレンド用ブレンドステート作成.
	//pngファイル内にアルファ情報があるので、透過するようにブレンドステートで設定する.
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory( &blendDesc, sizeof( D3D11_BLEND_DESC ) );	//初期化.
	blendDesc.IndependentBlendEnable = false;			//false:RenderTarget[0]のメンバーのみ使用する。true:RenderTarget[0〜7]が使用できる(レンダーターゲット毎に独立したブレンド処理).
	blendDesc.AlphaToCoverageEnable = false;			//true:アルファトゥカバレッジを使用する.

	//表示タイプ
//	blendDesc.RenderTarget[0].BlendEnable = true;					//true:アルファブレンドを使用する.
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;		//アルファブレンドを指定.
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;//アルファブレンドの反転を指定.
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;			//ADD：加算合成.
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;		//そのまま使用.
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;		//何もしない.
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	//ADD：加算合成.
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//全ての成分(RGBA)へのデータの格納を許可する.

	bool tmpBlendEnable[ enBLEND_STATE_size ];
	tmpBlendEnable[ enBLEND_STATE_ALPHA_ON ] = true;
	tmpBlendEnable[ enBLEND_STATE_ALPHA_OFF ] = false;

	for( unsigned char i=0; i<enBLEND_STATE_size; i++ )
	{
		blendDesc.RenderTarget[0].BlendEnable = tmpBlendEnable[i];
		if( FAILED( m_pDevice->CreateBlendState( &blendDesc, &m_pBlendState[i] ) ) ){
			assert( !"ブレンドステートの作成に失敗" );
			return E_FAIL;
		}
	}

	return S_OK;
}

//透過(アルファブレンド)設定の切り替え.
void clsD3DXSKINMESH::SetBlend( bool isAlpha )
{
	UINT mask = 0xffffffff;	//マスク値白.

	if( isAlpha ){		
		//ブレンドステートの設定.
		m_pDeviceContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_ON ], NULL, mask );
	}
	else{
		m_pDeviceContext->OMSetBlendState( m_pBlendState[ enBLEND_STATE_ALPHA_OFF ], NULL, mask );
	}

}

// 解放関数.
HRESULT clsD3DXSKINMESH::Release()
{
	if( m_pD3dxMesh ){
		// 全てのメッシュ削除.
		DestroyAllMesh( m_pD3dxMesh->m_pFrameRoot );

		// パーサークラス解放処理.
		m_pD3dxMesh->Release();
		SAFE_DELETE( m_pD3dxMesh );
	}

	return S_OK;
}


// 全てのメッシュを削除.
void clsD3DXSKINMESH::DestroyAllMesh( D3DXFRAME* pFrame )
{
	if( pFrame && pFrame->pMeshContainer )
	{
		DestroyAppMeshFromD3DXMesh( pFrame );
	}

	//再帰関数.
	if( pFrame->pFrameSibling != NULL )
	{
		DestroyAllMesh( pFrame->pFrameSibling );
	}
	if( pFrame->pFrameFirstChild != NULL )
	{
		DestroyAllMesh( pFrame->pFrameFirstChild );
	}
}


// メッシュの削除.
HRESULT clsD3DXSKINMESH::DestroyAppMeshFromD3DXMesh( LPD3DXFRAME p )
{
	MYFRAME* pFrame = (MYFRAME*)p;

//	LPD3DXMESH pD3DXMesh = pFrame->pMeshContainer->MeshData.pMesh;//D3DXﾒｯｼｭ(ここから・・・)
	MYMESHCONTAINER* pContainer = (MYMESHCONTAINER*)pFrame->pMeshContainer;

	// インデックスバッファ解放.
	for( DWORD i=0; i<pFrame->pPartsMesh->dwNumMaterial; i++ ){
		if( pFrame->pPartsMesh->ppIndexBuffer[i] != NULL ){
			pFrame->pPartsMesh->ppIndexBuffer[i]->Release();
			pFrame->pPartsMesh->ppIndexBuffer[i] = NULL;
		}
	}
	delete[] pFrame->pPartsMesh->ppIndexBuffer[0];

	// 頂点バッファ開放.
	pFrame->pPartsMesh->pVertexBuffer->Release();

	// パーツマテリアル開放.
	delete[] pFrame->pPartsMesh;

	return S_OK;
}



// アニメーションセットの切り替え.
void clsD3DXSKINMESH::ChangeAnimSet( int index, LPD3DXANIMATIONCONTROLLER pAC )
{
	if( m_pD3dxMesh == NULL )	return;
	m_pD3dxMesh->ChangeAnimSet( index, pAC );
}


// アニメーションセットの切り替え(開始フレーム指定可能版).
void clsD3DXSKINMESH::ChangeAnimSet_StartPos( int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC )
{
	if( m_pD3dxMesh == NULL )	return;
	m_pD3dxMesh->ChangeAnimSet_StartPos( index, dStartFramePos, pAC );
}


// アニメーション停止時間を取得.
double clsD3DXSKINMESH::GetAnimPeriod( int index )
{
	if( m_pD3dxMesh == NULL ){
		return 0.0f;
	}
	return m_pD3dxMesh->GetAnimPeriod( index );
}


// アニメーション数を取得.
int clsD3DXSKINMESH::GetAnimMax( LPD3DXANIMATIONCONTROLLER pAC )
{
	if( m_pD3dxMesh != NULL ){
		return m_pD3dxMesh->GetAnimMax( pAC );
	}
	return -1;
}


// 指定したボーン情報(行列)を取得する関数.
bool clsD3DXSKINMESH::GetMatrixFromBone( char* sBoneName, D3DXMATRIX* pOutMat )
{
	if( m_pD3dxMesh != NULL ){
		if( m_pD3dxMesh->GetMatrixFromBone( sBoneName, pOutMat ) ){
			return true;
		}
	}
	return false;
}
// 指定したボーン情報(座標)を取得する関数.
bool clsD3DXSKINMESH::GetPosFromBone( const char* sBoneName, D3DXVECTOR3* const pOutPos, bool isLocalPos ) const
{
	if( m_pD3dxMesh != NULL ){
		D3DXVECTOR3 tmpPos;
		if( m_pD3dxMesh->GetPosFromBone( sBoneName, &tmpPos ) ){
			D3DXMATRIX mWorld, mScale, mTran;
			D3DXMATRIX mRot, mYaw, mPitch, mRoll;
			D3DXMatrixScaling( &mScale, m_Trans.vScale.x, m_Trans.vScale.y, m_Trans.vScale.z );
			D3DXMatrixRotationY( &mYaw, m_Trans.fYaw );
			D3DXMatrixRotationX( &mPitch, m_Trans.fPitch );
			D3DXMatrixRotationZ( &mRoll, m_Trans.fRoll);
			D3DXMatrixTranslation(&mTran, tmpPos.x, tmpPos.y, tmpPos.z);

			//ローカル座標を指定されたら.
			if( isLocalPos ){
				D3DXMatrixIdentity( &mRot );
			}
			//ワールド座標なら( 普通はこっち ).
			else{
				mRot = mRoll * mPitch * mYaw;
			}

			mWorld = mTran * mRot * mScale;

			pOutPos->x = mWorld._41;
			pOutPos->y = mWorld._42;
			pOutPos->z = mWorld._43;

			//ワールド座標なら( 普通はこっち ).
			if( !isLocalPos ){
				*pOutPos += m_Trans.vPos;
			}

			return true;
		}
	}
	return false;
}

bool clsD3DXSKINMESH::GetDeviaPosFromBone(char* sBoneName, D3DXVECTOR3* pOutPos, D3DXVECTOR3 vDeviation)
{
	if (m_pD3dxMesh != NULL){
		D3DXMATRIX mtmp;
		if (m_pD3dxMesh->GetMatrixFromBone(sBoneName, &mtmp)){
			D3DXMATRIX mWorld, mScale, mTran, mDevia;
			D3DXMATRIX mRot, mYaw, mPitch, mRoll;
			D3DXMatrixTranslation(&mDevia, vDeviation.x, vDeviation.y, vDeviation.z);
			D3DXMatrixMultiply(&mtmp, &mDevia, &mtmp);
			D3DXVECTOR3 tmpPos = D3DXVECTOR3(mtmp._41, mtmp._42, mtmp._43);

			D3DXMatrixScaling(&mScale, m_Trans.vScale.x, m_Trans.vScale.y, m_Trans.vScale.z);
			D3DXMatrixRotationY(&mYaw, m_Trans.fYaw);
			D3DXMatrixRotationX(&mPitch, m_Trans.fPitch);
			D3DXMatrixRotationZ(&mRoll, m_Trans.fRoll);
			D3DXMatrixTranslation(&mTran, tmpPos.x, tmpPos.y, tmpPos.z);

			mRot = mRoll * mPitch * mYaw;
			mWorld = mTran * mScale * mRot;

			pOutPos->x = mWorld._41 + m_Trans.vPos.x;
			pOutPos->y = mWorld._42 + m_Trans.vPos.y;
			pOutPos->z = mWorld._43 + m_Trans.vPos.z;

			return true;
		}
	}
	return false;
}

//ボーンがあるか無いかを調べる関数.
bool clsD3DXSKINMESH::ExistsBone( const char* sBoneName )
{
	return m_pD3dxMesh->ExistsBone( sBoneName );
}


//リソースのボーン位置がずれる対策.
void clsD3DXSKINMESH::UpdateBonePos()
{
//	if (pAC == NULL)
//	{
		if (m_pD3dxMesh->m_pAnimController)
		{
			m_pD3dxMesh->m_pAnimController->AdvanceTime(m_dAnimTime, NULL);
		}
//	}

	D3DXMATRIX m;
	D3DXMatrixIdentity( &m );
	m_pD3dxMesh->UpdateFrameMatrices( m_pD3dxMesh->m_pFrameRoot, &m );
	UpDateBonePosProduct( m_pD3dxMesh->m_pFrameRoot );

}

void clsD3DXSKINMESH::UpDateBonePosProduct( LPD3DXFRAME p )
{
	MYFRAME*			pFrame	= (MYFRAME*)p;
	SKIN_PARTS_MESH*	pPartsMesh	= pFrame->pPartsMesh;
	MYMESHCONTAINER*	pContainer	= (MYMESHCONTAINER*)pFrame->pMeshContainer;

	if( pPartsMesh != NULL )
	{
		UpDateBonePosProductProduct(
			pPartsMesh,
			pContainer );
	}

	//再帰関数.
	//(兄弟)
	if( pFrame->pFrameSibling != NULL )
	{
		UpDateBonePosProduct( pFrame->pFrameSibling );
	}
	//(親子)
	if( pFrame->pFrameFirstChild != NULL )
	{
		UpDateBonePosProduct( pFrame->pFrameFirstChild );
	}

}

void clsD3DXSKINMESH::UpDateBonePosProductProduct(
		SKIN_PARTS_MESH* pMesh,
		MYMESHCONTAINER* const pContainer )
{
	SetNewPoseMatrices( pMesh, m_iFrame, pContainer );
}
