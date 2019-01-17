#include "Building.h"

#define _USE_MATH_DEFINES
#include <math.h>



using namespace std;


namespace{
	
#ifdef _DEBUG
//	const char* sTEX_NAME_TOP  = "Data\\Stage\\Building\\BuildingTexTopDebug.png";
//	const char* sTEX_NAME_SIDE = "Data\\Stage\\Building\\BuildingTexSideDebug.png";
	const char* sTEX_NAME_TOP  = "Data\\Stage\\Building\\BuildingTexTop.png";
	const char* sTEX_NAME_SIDE = "Data\\Stage\\Building\\BuildingTexSide.png";
	const char* sTEX_NAME_SIDEINSIDE = "Data\\Stage\\Building\\BuildingTexSideMask0.png";
	const char* sTEX_NAME_TOPINSIDE = "Data\\Image\\maskEmpty.png";
#else//#ifdef _DEBUG
	const char* sTEX_NAME_TOP  = "Data\\Stage\\Building\\BuildingTexTop.png";
	const char* sTEX_NAME_SIDE = "Data\\Stage\\Building\\BuildingTexSide.png";
	const char* sTEX_NAME_SIDEINSIDE = "Data\\Stage\\Building\\BuildingTexSideMask0.png";
	const char* sTEX_NAME_TOPINSIDE = "Data\\Image\\maskEmpty.png";
#endif//#ifdef _DEBUG

	//最低限の板の数.
	const int iTEX_NUM_MIN = 1;

	const float fTILE_SIZE_MAX_SIZE = 3.0f;//3.0f.//15.0f.
	const D3DXVECTOR3 vTILE_SIZE_MAX = { fTILE_SIZE_MAX_SIZE, fTILE_SIZE_MAX_SIZE, 1.0f };

	//側面のfor文の増加量.
	const int iSIDE_TILE_COUNT_NUM = 2;

	//裏面上下.
	const D3DXVECTOR3 vTOPINSIDE_ADD_ROT = { 0.0f, static_cast<float>( M_PI ), 0.0f };
	const D3DXVECTOR3 vBOTTOMINSIDE_ADD_POS = { 0.0f, 0.03125f, 0.0f };

}

clsBUILDING::clsBUILDING( 
	ID3D11Device* const pDevice11,
	ID3D11DeviceContext* const pContext11,
	clsDX9Mesh* const pModel,
	std::shared_ptr< clsSprite > spTop,
	std::shared_ptr< clsSprite > spSide,
	std::shared_ptr< clsSprite > spSideInside,
	std::shared_ptr< clsSprite > spTopInside,
	std::shared_ptr< clsSprite > spBottomInside )
	:m_wpDevice( pDevice11 )
	,m_wpContext( pContext11 )
	,m_Trans( { 
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 100.0f, 100.0f, 100.0f } } )
	,m_TopTrans( m_TopTrans )
{
	for( int i=0; i<enWALL_DIRECTION_size; i++  ){
		m_SideTransArray[i] = m_Trans;
	}

	m_upBox = make_unique< clsObjStaticMesh >();
	m_upBox->AttachModel( pModel );

	m_spTop			= spTop;
	m_spSide		= spSide;
	m_spSideInside	= spSideInside;
	m_spTopInside	= spTopInside;
	m_spBottomInside= spBottomInside;


//	m_spTop = make_shared< clsSprite >();
//	m_spTop->Create( pDevice11, pContext11, sTEX_NAME_TOP );
//
//	m_spSide = make_shared< clsSprite >();
//	m_spSide->Create( pDevice11, pContext11, sTEX_NAME_SIDE );
//
//	const float fINSIDESIDE_ALPHA = 0.5f;
//	m_spSideInside = make_shared< clsSprite >();
//	m_spSideInside->Create( pDevice11, pContext11, sTEX_NAME_SIDEINSIDE );
//	m_spSideInside->SetAlpha( fINSIDESIDE_ALPHA );
//
//	m_spTopInside = make_shared< clsSprite >();
//	m_spTopInside->Create( pDevice11, pContext11, sTEX_NAME_TOPINSIDE );
//	m_spTopInside->SetAlpha( fINSIDESIDE_ALPHA );
//
//	m_spBottomInside = make_shared< clsSprite >();
//	m_spBottomInside->Create( pDevice11, pContext11, sTEX_NAME_TOPINSIDE );

}

clsBUILDING::~clsBUILDING()
{
	m_upBox->DetatchModel();
	m_wpDevice = nullptr;
	m_wpContext = nullptr;
}

void clsBUILDING::CreateTexture(
	ID3D11Device* const pDevice11,
	ID3D11DeviceContext* const pContext11,
	std::shared_ptr< clsSprite >* const pspTop,
	std::shared_ptr< clsSprite >* const pspSide,
	std::shared_ptr< clsSprite >* const pspSideInside,
	std::shared_ptr< clsSprite >* const pspTopInside,
	std::shared_ptr< clsSprite >* const pspBottomInside )
{
	*pspTop = make_shared< clsSprite >();
	( *pspTop )->Create( pDevice11, pContext11, sTEX_NAME_TOP );

	*pspSide = make_shared< clsSprite >();
	( *pspSide )->Create( pDevice11, pContext11, sTEX_NAME_SIDE );

	const float fINSIDESIDE_ALPHA = 0.5f;
	*pspSideInside = make_shared< clsSprite >();
	( *pspSideInside )->Create( pDevice11, pContext11, sTEX_NAME_SIDEINSIDE );
	( *pspSideInside )->SetAlpha( fINSIDESIDE_ALPHA );

	*pspTopInside = make_shared< clsSprite >();
	( *pspTopInside )->Create( pDevice11, pContext11, sTEX_NAME_TOPINSIDE );
	( *pspTopInside )->SetAlpha( fINSIDESIDE_ALPHA );

	*pspBottomInside = make_shared< clsSprite >();
	( *pspBottomInside )->Create( pDevice11, pContext11, sTEX_NAME_TOPINSIDE );
}






void clsBUILDING::UpdateModel() const
{
	m_upBox->SetPosition( m_Trans.vPos );
	m_upBox->SetRotation( m_Trans.vRot );
	m_upBox->SetScale( m_Trans.vScale );

	m_upBox->ModelTransUpdate();
}

void clsBUILDING::UpdateTile()
{
	UpdateModel();

	//上面.
	unsigned int uiROW = iTEX_NUM_MIN;
	unsigned int uiCOL = iTEX_NUM_MIN;
	//タイルの目標数を作る.
	SetTileNumTargetTop( uiROW, uiCOL );
	//目標の数に合わせてタイルを増減する.
	SetTileNumTop( uiROW, uiCOL );
	//タイルを並べる.
	SetTransformTop();
	//座標のセット.
	m_spTop->SetPos( m_TopTrans.vPos );
	m_spTop->SetRot( m_TopTrans.vRot );
	m_spTop->SetScale( m_TopTrans.vScale );

	//側面.
	unsigned int uiROW_Z = iTEX_NUM_MIN;
	unsigned int uiCOL_Z = iTEX_NUM_MIN;
	unsigned int uiROW_X = iTEX_NUM_MIN;
	unsigned int uiCOL_X = iTEX_NUM_MIN;
	//タイルの目標数を作る.
	SetTileNumTargetSide( uiROW_Z, uiCOL_Z, uiROW_X, uiCOL_X );
	//目標の数に合わせてタイルを増減する.
	SetTileNumSide( uiROW_Z, uiCOL_Z, uiROW_X, uiCOL_X );
	//タイルを並べる.
	SetTransformSide();

	//裏面( 上面、底面 ).
	m_spTopInside->SetPos( m_TopTrans.vPos );
	m_spTopInside->SetRot( m_TopTrans.vRot + vTOPINSIDE_ADD_ROT );
	m_spTopInside->SetScale( m_TopTrans.vScale );

	m_spBottomInside->SetPos( m_Trans.vPos + vBOTTOMINSIDE_ADD_POS );
	m_spBottomInside->SetRot( m_TopTrans.vRot );
	m_spBottomInside->SetScale( m_TopTrans.vScale );
}

void clsBUILDING::Render(
	const D3DXMATRIX &mView, 
	const D3DXMATRIX &mProj,
	const D3DXVECTOR3 &vLight, 
	const D3DXVECTOR3 &vEye,
	const D3DXVECTOR4& vColor ) const
{
//	m_upBox->Render( mView, mProj, vLight, vEye );

	m_spTop->SetPos( m_TopTrans.vPos );
	m_spTop->SetRot( m_TopTrans.vRot );
	m_spTop->SetScale( m_TopTrans.vScale );
	m_spTop->SetSplit( m_vSplitTop );
	m_spTop->Render( mView, mProj, vEye, vColor );

	//========== 南北 ==========//.
	for( int i=enWD_SOUTH; i<enWALL_DIRECTION_size; i+=iSIDE_TILE_COUNT_NUM ){
		m_spSide->SetPos( m_SideTransArray[i].vPos );
		m_spSide->SetRot( m_SideTransArray[i].vRot );
		m_spSide->SetScale( m_SideTransArray[i].vScale );
		m_spSide->SetSplit( m_vSplitNorthSouth );
		m_spSide->Render( mView, mProj, vEye, vColor );
	}
	//========== 東西 ==========//.
	for( int i=enWD_EAST; i<enWALL_DIRECTION_size; i+=iSIDE_TILE_COUNT_NUM ){
		m_spSide->SetPos( m_SideTransArray[i].vPos );
		m_spSide->SetRot( m_SideTransArray[i].vRot );
		m_spSide->SetScale( m_SideTransArray[i].vScale );
		m_spSide->SetSplit( m_vSplitEastWest );
		m_spSide->Render( mView, mProj, vEye, vColor );
	}

}

void clsBUILDING::RenderInside(
	const D3DXMATRIX &mView, 
	const D3DXMATRIX &mProj,
	const D3DXVECTOR3 &vLight, 
	const D3DXVECTOR3 &vEye ) const
{
	const D3DXVECTOR3 vSIDEINSIDE_ADD_ROT = { 0.0f, static_cast<float>( M_PI ), 0.0f };
//	for( int i=0; i<enWALL_DIRECTION_size; i++  ){
//		m_spSideInside->SetPos( m_SideTransArray[i].vPos );
//		m_spSideInside->SetRot( m_SideTransArray[i].vRot + vSIDEINSIDE_ADD_ROT );
//		m_spSideInside->SetScale( m_SideTransArray[i].vScale );
//		m_spSideInside->Render( mView, mProj, vEye );
//	}
	//========== 南北 ==========//.
	for( int i=enWD_SOUTH; i<enWALL_DIRECTION_size; i+=iSIDE_TILE_COUNT_NUM ){
		m_spSideInside->SetPos( m_SideTransArray[i].vPos );
		m_spSideInside->SetRot( m_SideTransArray[i].vRot + vSIDEINSIDE_ADD_ROT );
		m_spSideInside->SetScale( m_SideTransArray[i].vScale );
		m_spSideInside->SetSplit( m_vSplitNorthSouth );
		m_spSideInside->Render( mView, mProj, vEye );
	}
	//========== 東西 ==========//.
	for( int i=enWD_EAST; i<enWALL_DIRECTION_size; i+=iSIDE_TILE_COUNT_NUM ){
		m_spSideInside->SetPos( m_SideTransArray[i].vPos );
		m_spSideInside->SetRot( m_SideTransArray[i].vRot + vSIDEINSIDE_ADD_ROT );
		m_spSideInside->SetScale( m_SideTransArray[i].vScale );
		m_spSideInside->SetSplit( m_vSplitEastWest );
		m_spSideInside->Render( mView, mProj, vEye );
	}

	m_spBottomInside->SetPos( m_Trans.vPos + vBOTTOMINSIDE_ADD_POS );
	m_spBottomInside->SetRot( m_TopTrans.vRot );
	m_spBottomInside->SetScale( m_TopTrans.vScale );
//	m_spBottomInside->SetSplit( m_vSplitTop );
	m_spBottomInside->Render( mView, mProj, vEye );

	m_spTopInside->SetPos( m_TopTrans.vPos );
	m_spTopInside->SetRot( m_TopTrans.vRot + vTOPINSIDE_ADD_ROT );
	m_spTopInside->SetScale( m_TopTrans.vScale );
//	m_spTopInside->SetSplit( m_vSplitTop );
	m_spTopInside->Render( mView, mProj, vEye );
}

//ビルの近くにいるか( 上から見た円の判定 ).
bool clsBUILDING::isNearBuilding(
	const D3DXVECTOR3& vPosObjOtherBuilding )//ビルと判定を取りたいモノの座標.
{
#if 0
	const float fOFFSET = 2.5f;
	D3DXVECTOR3 vTmp = m_Trans.vScale;
	vTmp *= 0.5f;
	//この距離に入ったらtrue.
	const float fRANGE = fOFFSET + sqrtf( 
			vTmp.x * vTmp.x +
			vTmp.z * vTmp.z );

	vTmp = vPosObjOtherBuilding - m_Trans.vPos;
	//二点間の距離.
	const float fDISTANCE = sqrtf( 
		vTmp.x * vTmp.x +
		vTmp.z * vTmp.z );


	if( fDISTANCE < fRANGE ){
		return true;
	}
#else
	const float fOFFSET = 2.5f;
	const float fHARF = 0.5f;
	const float fZN = m_Trans.vPos.z + m_Trans.vScale.z * fHARF + fOFFSET;
	const float fZS = m_Trans.vPos.z - m_Trans.vScale.z * fHARF - fOFFSET;
	const float fXE = m_Trans.vPos.x + m_Trans.vScale.x * fHARF + fOFFSET;
	const float fXW = m_Trans.vPos.x - m_Trans.vScale.x * fHARF - fOFFSET;
	const float fYT = m_Trans.vPos.y + m_Trans.vScale.y			+ fOFFSET;
	const float fYB = m_Trans.vPos.y							- fOFFSET;

	if( fZS < vPosObjOtherBuilding.z &&
		vPosObjOtherBuilding.z < fZN )
	{
		if( fXW < vPosObjOtherBuilding.x &&
			vPosObjOtherBuilding.x < fXE )
		{
			if( fYB < vPosObjOtherBuilding.y &&
				vPosObjOtherBuilding.y < fYT )
			{
				return true;
			}
		}
	}
#endif

	return false;
}



//タイルの目標数を作る.
void clsBUILDING::SetTileNumTargetTop( unsigned int& puiRow, unsigned int& puiCol )
{
	//----- 数を合わせる -----//.
	puiRow = static_cast<int>( m_TopTrans.vScale.y / vTILE_SIZE_MAX.y );
	puiCol = static_cast<int>( m_TopTrans.vScale.x / vTILE_SIZE_MAX.x );
}

//目標の数に合わせてタイルを増減する.
void clsBUILDING::SetTileNumTop( const unsigned int uiROW, const unsigned int uiCOL )
{
	unsigned int uiTmpCol = uiCOL;
	if( !uiCOL )	uiTmpCol = iTEX_NUM_MIN; 
	unsigned int uiTmpRow = uiROW;
	if( !uiROW )	uiTmpRow = iTEX_NUM_MIN; 

//	m_spTop->SetSplit( D3DXVECTOR2(
//		static_cast<float>( uiTmpCol ),
//		static_cast<float>( uiTmpRow ) ) );
	m_vSplitTop = D3DXVECTOR2(
		static_cast<float>( uiTmpCol ),
		static_cast<float>( uiTmpRow ) );
}

//タイルを並べる.
void clsBUILDING::SetTransformTop()
{
	//----- トランスフォームを整える -----//.
	m_TopTrans.vPos = m_Trans.vPos;

	m_TopTrans.vPos.y += m_Trans.vScale.y;
	m_TopTrans.vRot = { 0.0f, 0.0f, 0.0f };
	m_TopTrans.vRot.x = m_Trans.vRot.y;
	m_TopTrans.vRot.x += static_cast<float>( M_PI_2 );
	m_TopTrans.vRot.y -= static_cast<float>( M_PI_2 );
	m_TopTrans.vRot.z -= static_cast<float>( M_PI_2 + M_PI );

	const float fDEFALT_SCALE = 1.0f;
	m_TopTrans.vScale.z = fDEFALT_SCALE;
	m_TopTrans.vScale.x = m_Trans.vScale.x;
	m_TopTrans.vScale.y = m_Trans.vScale.z;

	//本体の回転に応じて移動.
	float fTheta, fDistance;
	GetTileTheta( m_TopTrans, m_Trans, &fTheta, &fDistance );
	GetTilePosForRotation( &m_TopTrans.vPos, m_Trans.vPos, fTheta, fDistance );
}

//タイルの目標数を作る.//引数は軸方向.
void clsBUILDING::SetTileNumTargetSide( 
	unsigned int& puiRowZ, unsigned int&  puiColZ,
	unsigned int& puiRowX, unsigned int&  puiColX )
{
	//----- 数を合わせる -----//.
	//========== 南北 ==========//.
	puiRowZ = static_cast<int>( m_SideTransArray[ enWD_SOUTH ].vScale.y / vTILE_SIZE_MAX.y );
	puiColZ = static_cast<int>( m_SideTransArray[ enWD_SOUTH ].vScale.x / vTILE_SIZE_MAX.x );

	//========== 東西 ==========//.
	puiRowX = static_cast<int>( m_SideTransArray[ enWD_EAST ].vScale.y / vTILE_SIZE_MAX.y );
	puiColX = static_cast<int>( m_SideTransArray[ enWD_EAST ].vScale.x / vTILE_SIZE_MAX.x );
}

//目標の数に合わせてタイルを増減する.//引数は軸方向.
void clsBUILDING::SetTileNumSide( 
	const unsigned int uiROW_Z, const unsigned int uiCOL_Z,
	const unsigned int uiROW_X, const unsigned int uiCOL_X )
{
	//========== 南北 ==========//.
	unsigned int uiTmpCol = uiCOL_Z;
	if( !uiTmpCol )	uiTmpCol = iTEX_NUM_MIN; 
	unsigned int uiTmpRow = uiROW_Z;
	if( !uiTmpRow )	uiTmpRow = iTEX_NUM_MIN; 

	m_vSplitNorthSouth = D3DXVECTOR2(
		static_cast<float>( uiTmpCol ),
		static_cast<float>( uiTmpRow ) );
	//========== 東西 ==========//.
	uiTmpCol = uiCOL_X;
	if( !uiTmpCol )	uiTmpCol = iTEX_NUM_MIN; 
	uiTmpRow = uiROW_X;
	if( !uiTmpRow )	uiTmpRow = iTEX_NUM_MIN; 

	m_vSplitEastWest = D3DXVECTOR2(
		static_cast<float>( uiTmpCol ),
		static_cast<float>( uiTmpRow ) );
}

//タイルを並べる.
void clsBUILDING::SetTransformSide()
{
	const float fHALF = 0.5f;

	//側面.
	const D3DXVECTOR3 vOFFSET_SIDE[ enWALL_DIRECTION_size ] ={
		D3DXVECTOR3( 0.0f,						m_Trans.vScale.y * fHALF, -m_Trans.vScale.z * fHALF ),
		D3DXVECTOR3( -m_Trans.vScale.x * fHALF,	m_Trans.vScale.y * fHALF, 0.0f						),
		D3DXVECTOR3( 0.0f,						m_Trans.vScale.y * fHALF, +m_Trans.vScale.z * fHALF ),
		D3DXVECTOR3( +m_Trans.vScale.x * fHALF,	m_Trans.vScale.y * fHALF, 0.0f						),
	};

	//南北.
	for( int i=0; i<enWALL_DIRECTION_size; i+=iSIDE_TILE_COUNT_NUM  ){
		//大きさをそろえる.
		const float fDEFALT_SCALE	 = 1.0f;
		m_SideTransArray[i].vScale.z = fDEFALT_SCALE;
		m_SideTransArray[i].vScale.x = m_Trans.vScale.x;
		m_SideTransArray[i].vScale.y = m_Trans.vScale.y;
		//中心寄せ.
		m_SideTransArray[i].vPos	 = m_Trans.vPos;
		//各面に寄せる.
		m_SideTransArray[i].vPos	+= vOFFSET_SIDE[ i ];
		//共通.
		m_SideTransArray[i].vRot	 = m_Trans.vRot;
		m_SideTransArray[i].vRot.y	+= static_cast<float>( M_PI_2 * i + M_PI );
			
		//本体の回転に応じて移動.
		float fTheta, fDistance;
		GetTileTheta( m_SideTransArray[i], m_Trans, &fTheta, &fDistance );
		GetTilePosForRotation( &m_SideTransArray[i].vPos, m_Trans.vPos, fTheta, fDistance );
	}
	//東西.
	for( int i=enWD_EAST; i<enWALL_DIRECTION_size; i+=iSIDE_TILE_COUNT_NUM  ){
		//大きさをそろえる.
		const float fDEFALT_SCALE	 = 1.0f;
		m_SideTransArray[i].vScale.z = fDEFALT_SCALE;
		m_SideTransArray[i].vScale.x = m_Trans.vScale.z;
		m_SideTransArray[i].vScale.y = m_Trans.vScale.y;
		//中心寄せ.
		m_SideTransArray[i].vPos	 = m_Trans.vPos;
		//各面に寄せる.
		m_SideTransArray[i].vPos	+= vOFFSET_SIDE[ i ];
		//共通.
		m_SideTransArray[i].vRot	 = m_Trans.vRot;
		m_SideTransArray[i].vRot.y	+= static_cast<float>( M_PI_2 * i + M_PI );
			
		//本体の回転に応じて移動.
		float fTheta, fDistance;
		GetTileTheta( m_SideTransArray[i], m_Trans, &fTheta, &fDistance );
		GetTilePosForRotation( &m_SideTransArray[i].vPos, m_Trans.vPos, fTheta, fDistance );
	}
}

//引数に入れたタイルが中心から見て何度の位置にあるか.
float clsBUILDING::GetTileTheta( 
	const TRANSFORM& Tile, const TRANSFORM& Center,
	float* const pfTheta, float* const pfDistance ) const
{
	float fReturn = 0;
	if( !pfTheta ){
		return fReturn;
	}
	if( !pfDistance ){
		return fReturn;
	}

	float fX = Center.vPos.x - Tile.vPos.x; 
	float fY = Center.vPos.z - Tile.vPos.z;

	fReturn = atan2f( fY, fX );

	*pfDistance = sqrtf( fY * fY + fX * fX );
	*pfTheta = fReturn;

	return fReturn;
}

//回転に応じて座標を更新する.
D3DXVECTOR3 clsBUILDING::GetTilePosForRotation( 
	D3DXVECTOR3* const vTilePos, const D3DXVECTOR3& vCenterPos,
	const float fTileTheta, const float fTileDistance ) const
{
	D3DXVECTOR3 vReturn = { 0.0f, 0.0f, 0.0f };
	if( !vTilePos ){
		return vReturn;
	}

	vTilePos->x = vCenterPos.x + cosf( fTileTheta - m_Trans.vRot.y ) * fTileDistance;
	vTilePos->z = vCenterPos.z + sinf( fTileTheta - m_Trans.vRot.y ) * fTileDistance;

	vReturn = *vTilePos;

	return vReturn;
}







