#include "Building.h"



#define _USE_MATH_DEFINES
#include <math.h>



//下が生きているなら複数枚の板を貼るのをシェーダーに任せずに各板の座標をこのクラスが保持する.
//#define BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS



using namespace std;


namespace{
	
	//最低限の板の数.
	const int iTEX_NUM_MIN = 1;

#ifdef _DEBUG
	const char* sTEX_NAME_SIDE = "Data\\Image\\Building\\BuildingTexSide_.png";
	const char* sTEX_NAME_TOP  = "Data\\Image\\Building\\BuildingTexTop_.png";
#else//#ifdef _DEBUG
	const char* sTEX_NAME_SIDE = "Data\\Image\\Building\\BuildingTexSide.png";
	const char* sTEX_NAME_TOP  = "Data\\Image\\Building\\BuildingTexTop.png";
#endif//#ifdef _DEBUG


	const int iRESURVE_NUM = 256;

//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
//	const D3DXVECTOR3 vTILE_SIZE_MIN = { 25.0f, 25.0f, 1.0f };
//	const D3DXVECTOR3 vTILE_SIZE_MAX = { 30.0f, 30.0f, 1.0f };
//#else//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
//	const D3DXVECTOR3 vTILE_SIZE_MIN = { 2.5f, 2.5f, 1.0f };
//	const D3DXVECTOR3 vTILE_SIZE_MAX = { 3.0f, 3.0f, 1.0f };
//#endif//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
	const D3DXVECTOR3 vTILE_SIZE_MIN = { 2.5f, 2.5f, 1.0f };
	const D3DXVECTOR3 vTILE_SIZE_MAX = { 3.0f, 3.0f, 1.0f };

	//側面のfor文の増加量.
	const int iSIDE_TILE_COUNT_NUM = 2;
}

clsBUILDING::clsBUILDING( 
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11,
		clsDX9Mesh* const pModel )
	:m_wpDevice( pDevice11 )
	,m_wpContext( pContext11 )
	,m_Trans( { 
		{ 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.0f },
		{ 100.0f, 100.0f, 100.0f } } )
{
	m_upBox = make_unique< clsObjStaticMesh >();
	m_upBox->AttachModel( pModel );



	m_vecvecTop.resize( iTEX_NUM_MIN );
	m_vecvecTop.reserve( iRESURVE_NUM );
	for( unsigned int Row=0; Row<m_vecvecTop.size(); Row++ ){
		m_vecvecTop[ Row ].resize( iTEX_NUM_MIN );
		m_vecvecTop[ Row ].reserve( iRESURVE_NUM );
	}
	m_upTop = make_unique< clsSprite >();
	m_upTop->Create( pDevice11, pContext11, sTEX_NAME_TOP );


	for( int SideNum=0; SideNum<enWALL_DIRECTION_size; SideNum++  ){
		m_vecvecSideArray[ SideNum ].resize( iTEX_NUM_MIN );
		m_vecvecSideArray[ SideNum ].reserve( iRESURVE_NUM );
		for( unsigned int Row=0; Row<m_vecvecSideArray[ SideNum ].size(); Row++ ){
			m_vecvecSideArray[ SideNum ][ Row ].resize( iTEX_NUM_MIN );
			m_vecvecSideArray[ SideNum ][ Row ].reserve( iRESURVE_NUM );
		}
	}
	m_upSide = make_unique< clsSprite >();
	m_upSide->Create( pDevice11, pContext11, sTEX_NAME_SIDE );
}

clsBUILDING::~clsBUILDING()
{
	m_upBox->DetatchModel();
	m_wpDevice = nullptr;
	m_wpContext = nullptr;
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
	{
		const int iEND_LOOP = 1;
		unsigned int uiROW = m_vecvecTop.size();
		unsigned int uiCOL = m_vecvecTop[0].size();
#if 0
		//同じものを繰り返さないためのチェック用配列.
		vector<unsigned int> vuiRow;
		vector<unsigned int> vuiCol;
		vuiRow.reserve( iRESURVE_NUM );
		vuiCol.reserve( iRESURVE_NUM );
		bool isRowEnd = false;
		for( int i=0; i<iEND_LOOP;  ){
			//タイルの目標数を作る.
			SetTileNumTargetTop( uiROW, uiCOL );
			//目標の数に合わせてタイルを増減する.
			SetTileNumTop( uiROW, uiCOL );
			//タイルを並べる.
			SetTransformTop();
			//増やす.
			vuiRow.push_back( uiROW );
			vuiCol.push_back( uiCOL );
			if( !isRowEnd ){
				//最後のものは現状と同じなので比較しない.
				for( unsigned int j=0; j<vuiRow.size() - 1; j++ )
				{
					if( uiROW == vuiRow[j] ){
						isRowEnd = true;
						break;
					}
				}
				continue;
			}
			else{
				for( unsigned int j=0; j<vuiCol.size() - 1; j++ ){
					if( uiCOL == vuiCol[j] ){
						i = iEND_LOOP;
						break;
					}
				}
			}
		}
#else
		//タイルの目標数を作る.
		SetTileNumTargetTop( uiROW, uiCOL );
		//目標の数に合わせてタイルを増減する.
		SetTileNumTop( uiROW, uiCOL );
		//タイルを並べる.
		SetTransformTop();
#endif
	}
	//側面.
	{
		unsigned int uiROW_Z = m_vecvecSideArray[ enWD_SOUTH ].size();
		unsigned int uiCOL_Z = m_vecvecSideArray[ enWD_SOUTH ][0].size();
		unsigned int uiROW_X = m_vecvecSideArray[ enWD_EAST ].size();
		unsigned int uiCOL_X = m_vecvecSideArray[ enWD_EAST ][0].size();
		//タイルの目標数を作る.
		SetTileNumTargetSide( uiROW_Z, uiCOL_Z, uiROW_X, uiCOL_X );
		//目標の数に合わせてタイルを増減する.
		SetTileNumSide( uiROW_Z, uiCOL_Z, uiROW_X, uiCOL_X );
		//タイルを並べる.
		SetTransformSide();
	}

}

void clsBUILDING::Render(
	const D3DXMATRIX &mView, 
	const D3DXMATRIX &mProj,
	const D3DXVECTOR3 &vLight, 
	const D3DXVECTOR3 &vEye ) const
{
#ifdef _DEBUG
	m_upBox->Render( mView, mProj, vLight, vEye );
#else//#ifdef _DEBUG
	for( unsigned int Row=0; Row<m_vecvecTop.size(); Row++ )
	{
		for( unsigned int Col=0; Col<m_vecvecTop[ Row ].size(); Col++ ){
			m_upTop->SetPos( m_vecvecTop[ Row ][ Col ].vPos );
			m_upTop->SetRot( m_vecvecTop[ Row ][ Col ].vRot );
			m_upTop->SetScale( m_vecvecTop[ Row ][ Col ].vScale );
			m_upTop->Render( mView, mProj, vEye );
		}
	}

	for( int SideNum=0; SideNum<enWALL_DIRECTION_size; SideNum++  )
	{
		for( unsigned int Row=0; Row<m_vecvecSideArray[ SideNum ].size(); Row++ )
		{
			for( unsigned int Col=0; Col<m_vecvecSideArray[ SideNum ][ Row ].size(); Col++ ){
				m_upSide->SetPos( m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos );
				m_upSide->SetRot( m_vecvecSideArray[ SideNum ][ Row ][ Col ].vRot );
				m_upSide->SetScale( m_vecvecSideArray[ SideNum ][ Row ][ Col ].vScale );
				m_upSide->Render( mView, mProj, vEye );
			}
		}
	}
#endif//#ifdef _DEBUG

}


//タイルの目標数を作る.
void clsBUILDING::SetTileNumTargetTop( unsigned int& puiRow, unsigned int& puiCol )
{
	//----- 数を合わせる -----//.

	if( !m_vecvecTop.size() ){
		return;
	}
	for( unsigned int i=0; i<m_vecvecTop.size(); i++ ){
		if( !m_vecvecTop[i].size() ){
			return;
		}
	}

	m_vecvecTop[ 0 ][ 0 ].vScale.x = m_Trans.vScale.x / m_vecvecTop[ 0 ].size();
	m_vecvecTop[ 0 ][ 0 ].vScale.y = m_Trans.vScale.z / m_vecvecTop.size();
	D3DXVECTOR3 vScale = m_vecvecTop[0][0].vScale;

#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS

	if( vScale.y > vTILE_SIZE_MAX.y ){
		puiRow ++;	
	}
	else if( vScale.y < vTILE_SIZE_MIN.y ){
		if( puiRow > iTEX_NUM_MIN ){
			puiRow --;
		}
	}

	if( vScale.x > vTILE_SIZE_MAX.x ){
		puiCol ++;	
	}
	else if( vScale.x < vTILE_SIZE_MIN.x ){
		if( puiCol > iTEX_NUM_MIN ){
			puiCol --;
		}
	}
#else//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS

	puiRow = static_cast<int>( vScale.y / vTILE_SIZE_MAX.y );
	puiCol = static_cast<int>( vScale.x / vTILE_SIZE_MAX.x );

#endif//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
}

//目標の数に合わせてタイルを増減する.
void clsBUILDING::SetTileNumTop( const unsigned int uiROW, const unsigned int uiCOL )
{
#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
	//----- 個数 -----//.
	//縦.
	//足りないから増やす.
	if( m_vecvecTop.size() < uiROW ){
		vector< TRANSFORM > tmpInit;
		m_vecvecTop.push_back( tmpInit );//行を増やす.
		const unsigned int uiNEW_ROW_NUM = m_vecvecTop.size() - 1;
		m_vecvecTop[ uiNEW_ROW_NUM ].resize( m_vecvecTop[0].size() );//増やした行の列の数をそろえる.
		m_vecvecTop[ uiNEW_ROW_NUM ].reserve( iRESURVE_NUM );
	}
	//多いから減らす.
	else if( m_vecvecTop.size() > uiROW ){
		const unsigned int uiDELETE_RAW_NUM = m_vecvecTop.size() - 1;
		m_vecvecTop.pop_back();//行を減らす.
	}
	//横.
	//足りないから増やす.
	if( m_vecvecTop[0].size() < uiCOL ){
		TRANSFORM tmpInit;
		for( unsigned int Row=0; Row<m_vecvecTop.size(); Row++ ){
			m_vecvecTop[ Row ].push_back( tmpInit );
		}
	}
	//多いから減らす.
	else if( m_vecvecTop[0].size() > uiCOL ){
		for( unsigned int Row=0; Row<m_vecvecTop.size(); Row++ ){
			m_vecvecTop[ Row ].pop_back();
		}
	}
#else//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
	m_upTop->SetSplit( D3DXVECTOR2(
		static_cast<float>( uiCOL ),
		static_cast<float>( uiROW ) ) );
#endif//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
}

//タイルを並べる.
void clsBUILDING::SetTransformTop()
{
	const float fHALF = 0.5f;

	//----- トランスフォームを整える -----//.
	//上面.
	for( unsigned int Row=0; Row<m_vecvecTop.size(); Row++ )
	{
		for( unsigned int Col=0; Col<m_vecvecTop[ Row ].size(); Col++ ){
			//大きさをそろえる.
			m_vecvecTop[ Row ][ Col ].vScale.x = m_Trans.vScale.x / m_vecvecTop[ Row ].size();
			m_vecvecTop[ Row ][ Col ].vScale.y = m_Trans.vScale.z / m_vecvecTop.size();
			//中心寄せ.
			m_vecvecTop[ Row ][ Col ].vPos = m_Trans.vPos;
			//左寄せ.	
			m_vecvecTop[ Row ][ Col ].vPos.x -= m_Trans.vScale.x * fHALF;				//板の真ん中が端に来る.
			m_vecvecTop[ Row ][ Col ].vPos.x += m_vecvecTop[ Row ][ Col ].vScale.x * fHALF;	//板の半分だけずらす.
			//順番に貼る.		
			m_vecvecTop[ Row ][ Col ].vPos.x += ( m_Trans.vScale.x / ( m_vecvecTop[ Row ].size() ) ) * Col;
			//下寄せ.	
			m_vecvecTop[ Row ][ Col ].vPos.z -= m_Trans.vScale.z * fHALF;
			m_vecvecTop[ Row ][ Col ].vPos.z += m_vecvecTop[ Row ][ Col ].vScale.y * fHALF;
			//順番に貼る.		
			m_vecvecTop[ Row ][ Col ].vPos.z += ( m_Trans.vScale.z / ( m_vecvecTop.size() ) ) * Row;
			//共通.			   
			m_vecvecTop[ Row ][ Col ].vPos.y += m_Trans.vScale.y;
			m_vecvecTop[ Row ][ Col ].vRot = { 0.0f, 0.0f, 0.0f };
			m_vecvecTop[ Row ][ Col ].vRot.x = m_Trans.vRot.y;
			m_vecvecTop[ Row ][ Col ].vRot.x += static_cast<float>( M_PI_2 );
			m_vecvecTop[ Row ][ Col ].vRot.y -= static_cast<float>( M_PI_2 );
			m_vecvecTop[ Row ][ Col ].vRot.z -= static_cast<float>( M_PI_2 + M_PI );

			//本体の回転に応じて移動.
			float fTheta, fDistance;
			GetTileTheta( m_vecvecTop[ Row ][ Col ], m_Trans, &fTheta, &fDistance );
			GetTilePosForRotation( &m_vecvecTop[ Row ][ Col ].vPos, m_Trans.vPos, fTheta, fDistance );
		}
	}
}

//タイルの目標数を作る.//引数は軸方向.
void clsBUILDING::SetTileNumTargetSide( 
	unsigned int& puiRowZ, unsigned int&  puiColZ,
	unsigned int& puiRowX, unsigned int&  puiColX )
{
	//----- 数を合わせる -----//.

	for( int SideNum=0; SideNum<enWALL_DIRECTION_size; SideNum++  ){
		if( !m_vecvecSideArray[ SideNum ].size() ){
			return;
		}
		for( unsigned int i=0; i<m_vecvecSideArray[ SideNum ].size(); i++ ){
			if( !m_vecvecSideArray[ SideNum ][i].size() ){
				return;
			}
		}
	}

	//========== 南北 ==========//.
	for( int SideNum=0; SideNum<enWALL_DIRECTION_size; SideNum+=iSIDE_TILE_COUNT_NUM  ){
		m_vecvecSideArray[ SideNum ][ 0 ][ 0 ].vScale.x = m_Trans.vScale.x / m_vecvecSideArray[ SideNum ][ 0 ].size();
		m_vecvecSideArray[ SideNum ][ 0 ][ 0 ].vScale.y = m_Trans.vScale.y / m_vecvecSideArray[ SideNum ].size();
		D3DXVECTOR3 vScale = m_vecvecSideArray[ SideNum ][0][0].vScale;

#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
		if( vScale.y > vTILE_SIZE_MAX.y ){
			puiRowZ ++;
		}
		else if( vScale.y < vTILE_SIZE_MIN.y ){
			if( puiRowZ > iTEX_NUM_MIN ){
				puiRowZ --;
			}
		}

		if( vScale.x > vTILE_SIZE_MAX.x ){
			puiColZ ++;
		}
		else if( vScale.x < vTILE_SIZE_MIN.x ){
			if( puiColZ > iTEX_NUM_MIN ){
				puiColZ --;
			}
		}
#else//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS

	puiRowZ = static_cast<int>( vScale.y / vTILE_SIZE_MAX.y );
	puiColZ = static_cast<int>( vScale.x / vTILE_SIZE_MAX.x );

#endif//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
	}
	//========== 東西 ==========//.
	for( int SideNum=1; SideNum<enWALL_DIRECTION_size; SideNum+=iSIDE_TILE_COUNT_NUM  ){
		m_vecvecSideArray[ SideNum ][ 0 ][ 0 ].vScale.x = m_Trans.vScale.z / m_vecvecSideArray[ SideNum ][ 0 ].size();
		m_vecvecSideArray[ SideNum ][ 0 ][ 0 ].vScale.y = m_Trans.vScale.y / m_vecvecSideArray[ SideNum ].size();
		D3DXVECTOR3 vScale = m_vecvecSideArray[ SideNum ][0][0].vScale;

#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
		if( vScale.y > vTILE_SIZE_MAX.y ){
			puiRowX ++;
		}
		else if( vScale.y < vTILE_SIZE_MIN.y ){
			if( puiRowX > iTEX_NUM_MIN ){
				puiRowX --;
			}
		}

		if( vScale.x > vTILE_SIZE_MAX.x ){
			puiColX ++;
		}
		else if( vScale.x < vTILE_SIZE_MIN.x ){
			if( puiColX > iTEX_NUM_MIN ){
				puiColX --;
			}
		}
#else//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS

	puiRowX = static_cast<int>( vScale.y / vTILE_SIZE_MAX.y );
	puiColX = static_cast<int>( vScale.x / vTILE_SIZE_MAX.x );

#endif//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
	}
}

//目標の数に合わせてタイルを増減する.//引数は軸方向.
void clsBUILDING::SetTileNumSide( 
	const unsigned int uiROW_Z, const unsigned int uiCOL_Z,
	const unsigned int uiROW_X, const unsigned int uiCOL_X )
{
#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
	//========== 南北 ==========//.
	for( int SideNum=0; SideNum<enWALL_DIRECTION_size; SideNum+=iSIDE_TILE_COUNT_NUM  ){
		//----- 縦 -----//.
		//足りないから増やす.
		if( m_vecvecSideArray[ SideNum ].size() < uiROW_Z ){
			vector< TRANSFORM > tmpInit;
			m_vecvecSideArray[ SideNum ].push_back( tmpInit );
			const unsigned int uiNEW_ROW_NUM = m_vecvecSideArray[ SideNum ].size() - 1;
			m_vecvecSideArray[ SideNum ][ uiNEW_ROW_NUM ].resize( m_vecvecSideArray[ SideNum ][0].size() );
			m_vecvecSideArray[ SideNum ][ uiNEW_ROW_NUM ].reserve( iRESURVE_NUM );
		}
		//多いから減らす.
		else if( m_vecvecSideArray[ SideNum ].size() > uiROW_Z ){
			const unsigned int uiDELETE_RAW_NUM = m_vecvecSideArray[ SideNum ].size() - 1;
			m_vecvecSideArray[ SideNum ].pop_back();
		}
		//----- 横 -----//.
		//足りないから増やす.
		if( m_vecvecSideArray[ SideNum ][0].size() < uiCOL_Z ){
			TRANSFORM tmpInit;
			for( unsigned int Row=0; Row<m_vecvecSideArray[ SideNum ].size(); Row++ ){
				m_vecvecSideArray[ SideNum ][ Row ].push_back( tmpInit );
			}			
		}
		//多いから減らす.
		else if( m_vecvecSideArray[ SideNum ][0].size() > uiCOL_Z ){
			for( unsigned int Row=0; Row<m_vecvecSideArray[ SideNum ].size(); Row++ ){
				m_vecvecSideArray[ SideNum ][ Row ].pop_back();
			}
		}
	}
	//========== 東西 ==========//.
	for( int SideNum=1; SideNum<enWALL_DIRECTION_size; SideNum+=iSIDE_TILE_COUNT_NUM  ){
		//----- 縦 -----//.
		//足りないから増やす.
		if( m_vecvecSideArray[ SideNum ].size() < uiROW_X ){
			vector< TRANSFORM > tmpInit;
			m_vecvecSideArray[ SideNum ].push_back( tmpInit );
			const unsigned int uiNEW_ROW_NUM = m_vecvecSideArray[ SideNum ].size() - 1;
			m_vecvecSideArray[ SideNum ][ uiNEW_ROW_NUM ].resize( m_vecvecSideArray[ SideNum ][0].size() );
			m_vecvecSideArray[ SideNum ][ uiNEW_ROW_NUM ].reserve( iRESURVE_NUM );
		}
		//多いから減らす.
		else if( m_vecvecSideArray[ SideNum ].size() > uiROW_X ){
			const unsigned int uiDELETE_RAW_NUM = m_vecvecSideArray[ SideNum ].size() - 1;
			m_vecvecSideArray[ SideNum ].pop_back();
		}
		//----- 横 -----//.
		//足りないから増やす.
		if( m_vecvecSideArray[ SideNum ][0].size() < uiCOL_X ){
			TRANSFORM tmpInit;
			for( unsigned int Row=0; Row<m_vecvecSideArray[ SideNum ].size(); Row++ ){
				m_vecvecSideArray[ SideNum ][ Row ].push_back( tmpInit );
			}			
		}
		//多いから減らす.
		else if( m_vecvecSideArray[ SideNum ][0].size() > uiCOL_X ){
			for( unsigned int Row=0; Row<m_vecvecSideArray[ SideNum ].size(); Row++ ){
				m_vecvecSideArray[ SideNum ][ Row ].pop_back();
			}
		}
	}
#else//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS
	//========== 南北 ==========//.
	for( int SideNum=0; SideNum<enWALL_DIRECTION_size; SideNum+=iSIDE_TILE_COUNT_NUM  ){
		m_upSide->SetSplit( D3DXVECTOR2(
			static_cast<float>( uiCOL_Z ),
			static_cast<float>( uiROW_Z ) ) );
	}
	//========== 東西 ==========//.
	for( int SideNum=1; SideNum<enWALL_DIRECTION_size; SideNum+=iSIDE_TILE_COUNT_NUM  ){
		m_upSide->SetSplit( D3DXVECTOR2(
			static_cast<float>( uiCOL_X ),
			static_cast<float>( uiROW_X ) ) );
	}
#endif//#ifdef BUILDING_TEXCHARS_POS_HAVE_THIS_CLASS

}

//タイルを並べる.
void clsBUILDING::SetTransformSide()
{
	const float fHALF = 0.5f;

	//側面.
	const D3DXVECTOR3 vOFFSET_SIDE[ enWALL_DIRECTION_size ] ={
		D3DXVECTOR3( 0.0f,						0.0f, -m_Trans.vScale.z * fHALF ),
		D3DXVECTOR3( -m_Trans.vScale.x * fHALF,	0.0f, 0.0f						),
		D3DXVECTOR3( 0.0f,						0.0f, +m_Trans.vScale.z * fHALF ),
		D3DXVECTOR3( +m_Trans.vScale.x * fHALF,	0.0f, 0.0f						),
	};

	//南北.
	for( int SideNum=0; SideNum<enWALL_DIRECTION_size; SideNum+=iSIDE_TILE_COUNT_NUM  )
	{
		for( unsigned int Row=0; Row<m_vecvecSideArray[ SideNum ].size(); Row++ )
		{
			for( unsigned int Col=0; Col<m_vecvecSideArray[ SideNum ][ Row ].size(); Col++ ){
				//大きさをそろえる.
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vScale.x	 = m_Trans.vScale.x / m_vecvecSideArray[ SideNum ][ Row ].size();
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vScale.y	 = m_Trans.vScale.y / m_vecvecSideArray[ SideNum ].size();
				//中心寄せ.
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos		 = m_Trans.vPos;
				//各面に寄せる.
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos		+= vOFFSET_SIDE[ SideNum ];
				//左寄せ.
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos.x	-= m_Trans.vScale.x * fHALF;
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos.x	+= m_vecvecSideArray[ SideNum ][ Row ][ Col ].vScale.x * fHALF;
				//順番に貼る.		
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos.x	+= ( m_Trans.vScale.x / ( m_vecvecSideArray[ SideNum ][ Row ].size() ) ) * Col;
				//下寄せ.	
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos.y	+= m_vecvecSideArray[ SideNum ][ Row ][ Col ].vScale.y * fHALF;
				//順番に貼る.		
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos.y	+= ( m_Trans.vScale.y / ( m_vecvecSideArray[ SideNum ].size() ) ) * Row;
				//共通.
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vRot		 = m_Trans.vRot;
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vRot.y	+= static_cast<float>( M_PI_2 * SideNum + M_PI );
			
				//本体の回転に応じて移動.
				float fTheta, fDistance;
				GetTileTheta( m_vecvecSideArray[ SideNum ][ Row ][ Col ], m_Trans, &fTheta, &fDistance );
				GetTilePosForRotation( &m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos, m_Trans.vPos, fTheta, fDistance );
			}
		}
	}
	//東西.
	for( int SideNum=enWD_EAST; SideNum<enWALL_DIRECTION_size; SideNum+=iSIDE_TILE_COUNT_NUM  )
	{
		for( unsigned int Row=0; Row<m_vecvecSideArray[ SideNum ].size(); Row++ )
		{
			for( unsigned int Col=0; Col<m_vecvecSideArray[ SideNum ][ Row ].size(); Col++ )
			{
				//大きさをそろえる.
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vScale.x	 = m_Trans.vScale.z / m_vecvecSideArray[ SideNum ][ Row ].size();
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vScale.y	 = m_Trans.vScale.y / m_vecvecSideArray[ SideNum ].size();
				//中心寄せ.
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos		 = m_Trans.vPos;
				//各面に寄せる.
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos		+= vOFFSET_SIDE[ SideNum ];
				//左寄せ.
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos.z	-= m_Trans.vScale.z * fHALF;
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos.z	+= m_vecvecSideArray[ SideNum ][ Row ][ Col ].vScale.x * fHALF;
				//順番に貼る.		
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos.z	+= ( m_Trans.vScale.z / ( m_vecvecSideArray[ SideNum ][ Row ].size() ) ) * Col;
				//下寄せ.	
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos.y	+= m_vecvecSideArray[ SideNum ][ Row ][ Col ].vScale.y * fHALF;
				//順番に貼る.		
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos.y	+= ( m_Trans.vScale.y / ( m_vecvecSideArray[ SideNum ].size() ) ) * Row;
				//共通.
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vRot		 = m_Trans.vRot;
				m_vecvecSideArray[ SideNum ][ Row ][ Col ].vRot.y	+= static_cast<float>( M_PI_2 * SideNum + M_PI );
			
				//本体の回転に応じて移動.
				float fTheta, fDistance;
				GetTileTheta( m_vecvecSideArray[ SideNum ][ Row ][ Col ], m_Trans, &fTheta, &fDistance );
				GetTilePosForRotation( &m_vecvecSideArray[ SideNum ][ Row ][ Col ].vPos, m_Trans.vPos, fTheta, fDistance );
			}
		}
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







