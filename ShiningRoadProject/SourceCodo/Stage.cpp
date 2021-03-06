#include "Stage.h"
#include "File.h"
#include "Sprite.h"
#include "Building.h"
#include "PtrGroup.h"
#include "Resource.h"
#include "ObjStaticMesh.h"
#include "ObjSkinMesh.h"

using namespace std;

namespace{
	const char* sSTAGE_BASE_DATA_PATH = "Data\\Stage\\StageBase.csv";
	const char* sBUILDING_DATA_PATH = "Data\\Stage\\Building.csv";

	//ファイル読み取り時の添え字.
	const char cINDEX_POS_X = 0;
	const char cINDEX_POS_Y = 1;
	const char cINDEX_POS_Z = 2;

	const char cINDEX_ROT_X = 3;
	const char cINDEX_ROT_Y = 4;
	const char cINDEX_ROT_Z = 5;

	const char cINDEX_SCALE_X = 6;
	const char cINDEX_SCALE_Y = 7;
	const char cINDEX_SCALE_Z = 8;

	const D3DXVECTOR4 vLIGHT_COLOR_BLUE = D3DXVECTOR4( 0.0f, 1.0f, 1.0f, 1.0f );
	const D3DXVECTOR4 vLIGHT_COLOR_RED  = D3DXVECTOR4( 1.0f, 0.0f, 0.0f, 1.0f );

	const double dDOOR_ANIM_SPD = 0.0125;

}

clsStage::clsStage( clsPOINTER_GROUP* const pPtrGroup )
	:m_vLightColorTarget( { 1.0f, 1.0f, 1.0f, 1.0f } )
	,m_vLightColor( m_vLightColorTarget )
{
	clsFILE file;
	file.Open( sSTAGE_BASE_DATA_PATH );
	//========== 土台 ==========//.
	//----- 当たり判定 -----//.
	m_pStageCollision = make_unique< clsObjStaticMesh >();
	m_pStageCollision->AttachModel( 
		pPtrGroup->GetResource()->GetStaticModels( clsResource::enStaticModel_StageCollision ) );

	//ステータス受け取り.
	//座標.
	m_pStageCollision->SetPosition( {
		file.GetDataFloat( 0, cINDEX_POS_X ),
		file.GetDataFloat( 0, cINDEX_POS_Y ),
		file.GetDataFloat( 0, cINDEX_POS_Z ) } );
	//回転.
	m_pStageCollision->SetRotation( {
		file.GetDataFloat( 0, cINDEX_ROT_X ),
		file.GetDataFloat( 0, cINDEX_ROT_Y ),
		file.GetDataFloat( 0, cINDEX_ROT_Z ) } );
	//縮尺.
	m_pStageCollision->SetScale( {
		file.GetDataFloat( 0, cINDEX_SCALE_X ),
		file.GetDataFloat( 0, cINDEX_SCALE_Y ),
		file.GetDataFloat( 0, cINDEX_SCALE_Z ) } );
	//----- 当たり判定 終わり -----//.

	//----- 見た目 -----//.
	m_pStageFloor = make_unique< clsObjStaticMesh >();
	m_pStageFloor->AttachModel( 
		pPtrGroup->GetResource()->GetStaticModels( clsResource::enStaticModel_StageFloor ) );
	m_pStageFloor->SetPosition( m_pStageCollision->GetPosition() );
	m_pStageFloor->SetRotation( m_pStageCollision->GetRotation() );
	m_pStageFloor->SetScale( {
		file.GetDataFloat( 0, cINDEX_SCALE_X ),
		file.GetDataFloat( 0, cINDEX_SCALE_Y ),
		file.GetDataFloat( 0, cINDEX_SCALE_Z ) } );

	m_pStageCelling = make_unique< clsObjStaticMesh >();
	m_pStageCelling->AttachModel( 
		pPtrGroup->GetResource()->GetStaticModels( clsResource::enStaticModel_StageCelling ) );
	m_pStageCelling->SetPosition( m_pStageCollision->GetPosition() );
	m_pStageCelling->SetRotation( m_pStageCollision->GetRotation() );
	m_pStageCelling->SetScale( {
		file.GetDataFloat( 0, cINDEX_SCALE_X ),
		file.GetDataFloat( 0, cINDEX_SCALE_Y ),
		file.GetDataFloat( 0, cINDEX_SCALE_Z ) } );

	//========== 土台 終わり ==========//.	


	//========== ドア ==========//.
	m_pLia = make_unique< clsObjSkinMesh >();
	m_pLia->AttachModel(
		pPtrGroup->GetResource()->GetSkinModels( clsResource::enSkinModel_Lia ) );
	m_pLia->SetPosition( m_pStageCollision->GetPosition() );
	m_pLia->SetRotation( m_pStageCollision->GetRotation() );
	m_pLia->SetScale( {
		file.GetDataFloat( 0, cINDEX_SCALE_X ),
		file.GetDataFloat( 0, cINDEX_SCALE_Y ),
		file.GetDataFloat( 0, cINDEX_SCALE_Z ) } );

		m_pLia->ModelUpdate( m_pLia->m_Trans );//このモデルはここしか使わないから毎フレームしなくていい.
		m_pLia->SetAnimSpeed( dDOOR_ANIM_SPD );

	for( int i=0; i<enDOOR_NUM_size; i++ ){
		m_pDoorArray[i] = make_unique< clsObjSkinMesh >();
		m_pDoorArray[i]->AttachModel(
			pPtrGroup->GetResource()->GetSkinModels( clsResource::enSkinModel_Door ) );
		m_pDoorArray[i]->SetPosition( m_pStageCollision->GetPosition() );
		//二つ目は反対の位置にするために回す.
		m_pDoorArray[i]->SetRotation( 
			m_pStageCollision->GetRotation() +
			( D3DXVECTOR3( 0.0f, static_cast<float>( D3DX_PI * i ), 0.0f ) ) );
		m_pDoorArray[i]->SetScale( {
			file.GetDataFloat( 0, cINDEX_SCALE_X ),
			file.GetDataFloat( 0, cINDEX_SCALE_Y ),
			file.GetDataFloat( 0, cINDEX_SCALE_Z ) } );

		m_pDoorArray[i]->SetAnimSpeed( dDOOR_ANIM_SPD );
	}

	file.Close();
	//========== ドア 終わり ==========//.

	//========== ビル ==========//.
	//下準備.
	clsBUILDING::CreateTexture(
		pPtrGroup->GetDevice(), 
		pPtrGroup->GetContext(), 
		&m_spBuildingTop,
		&m_spBuildingSide,
		&m_spBuildingSideInside,
		&m_spBuildingTopInside,
		&m_spBuildingBottomInside );

	file.Open( sBUILDING_DATA_PATH );
	m_vpBuilding.resize( file.GetSizeRow() );
	//ビルの作成 & 初期化.
	for( unsigned int i=0; i<m_vpBuilding.size(); i++ ){
		m_vpBuilding[i] = make_unique< clsBUILDING >( 
			pPtrGroup->GetDevice(), 
			pPtrGroup->GetContext(), 
			pPtrGroup->GetResource()->GetStaticModels( clsResource::enStaticModel_Building ),
			m_spBuildingTop,
			m_spBuildingSide,
			m_spBuildingSideInside,
			m_spBuildingTopInside,
			m_spBuildingBottomInside );

		//座標.
		m_vpBuilding[i]->SetPos( {
			file.GetDataFloat( i, cINDEX_POS_X ),
			file.GetDataFloat( i, cINDEX_POS_Y ),
			file.GetDataFloat( i, cINDEX_POS_Z ) } );
		//回転.
		m_vpBuilding[i]->SetRot( {
			file.GetDataFloat( i, cINDEX_ROT_X ),
			file.GetDataFloat( i, cINDEX_ROT_Y ),
			file.GetDataFloat( i, cINDEX_ROT_Z ) } );
		//縮尺.
		m_vpBuilding[i]->SetScale( {
			file.GetDataFloat( i, cINDEX_SCALE_X ),
			file.GetDataFloat( i, cINDEX_SCALE_Y ),
			file.GetDataFloat( i, cINDEX_SCALE_Z ) } );
	}
	file.Close();

	for( unsigned int i=0; i<m_vpBuilding.size(); i++ ){
		m_vpBuilding[i]->UpdateTile();
	}
	//========== ビル 終わり ==========//.

	for( UCHAR i=0; i<enDOOR_size; i++ ){
		SetAnimDoor( static_cast<enDOOR>( i ), enDOOR_ANIM_CLOSED );
	}

	//光を青く.
	SetColor( vLIGHT_COLOR_BLUE );
}
//enStaticModel_Building
clsStage::~clsStage()
{
//	m_pStageCollision->DetatchModel();

//	for( unsigned int i=0; i<m_vpBuilding.size(); i++ ){
//		m_vpBuilding[i]->DetatchModel();
//	}

	for( int i=0; i<enDOOR_NUM_size; i++ ){
		m_pDoorArray[i].reset( nullptr );
	}

}


void clsStage::Render(
	const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
	const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye )
{
	//当たり判定用モデル.
//	m_pStageCollision->Render( mView, mProj, vLight, vEye );

//	const D3DXVECTOR3 vLIGHT = vLight;
	const D3DXVECTOR3 vLIGHT = { 0.0f, 0.0f, 0.0f };

	//床と天井.
	m_pStageFloor->Render(	 mView, mProj, vLIGHT, vEye, m_vLightColor );
	m_pStageCelling->Render( mView, mProj, vLIGHT, vEye, m_vLightColor );

	assert( m_pLia );
	m_pLia->ModelRender( mView, mProj, vLIGHT, vEye, m_vLightColor );

	for( int i=0; i<enDOOR_NUM_size; i++ ){
		assert( m_pDoorArray[i] );
		m_pDoorArray[i]->ModelUpdate( m_pDoorArray[i]->m_Trans );
		m_pDoorArray[i]->ModelRender( mView, mProj, vLIGHT, vEye, m_vLightColor );
	}

	for( unsigned int i=0; i<m_vpBuilding.size(); i++ ){
		m_vpBuilding[i]->Render( mView, mProj, vLIGHT, vEye, m_vLightColor );
	}

	//ドアが開いたら空きっぱなし.
	SafeDoorAnimation();

	//ピカピカ光るよ.
	UpdateLight();

//#ifdef _DEBUG
//	//色.
//	const float fSTATIC_MODEL_COLOR_RGB_ADD = 0.025f; 
//	if( GetAsyncKeyState( 'F' ) & 0x8000 )m_vLightColorTarget.x += fSTATIC_MODEL_COLOR_RGB_ADD;
//	if( GetAsyncKeyState( 'V' ) & 0x8000 )m_vLightColorTarget.x -= fSTATIC_MODEL_COLOR_RGB_ADD;
//	if( GetAsyncKeyState( 'G' ) & 0x8000 )m_vLightColorTarget.y += fSTATIC_MODEL_COLOR_RGB_ADD;
//	if( GetAsyncKeyState( 'B' ) & 0x8000 )m_vLightColorTarget.y -= fSTATIC_MODEL_COLOR_RGB_ADD;
//	if( GetAsyncKeyState( 'H' ) & 0x8000 )m_vLightColorTarget.z += fSTATIC_MODEL_COLOR_RGB_ADD;
//	if( GetAsyncKeyState( 'N' ) & 0x8000 )m_vLightColorTarget.z -= fSTATIC_MODEL_COLOR_RGB_ADD;
//	if( GetAsyncKeyState( 'V' ) & 0x8000 )SetColorTarget( vLIGHT_COLOR_BLUE );
//	if( GetAsyncKeyState( 'B' ) & 0x8000 )SetColorTarget( vLIGHT_COLOR_RED );
//#endif//#ifdef _DEBUG

}

void clsStage::RenderInside(
	const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
	const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye )
{
	for( unsigned int i=0; i<m_vpBuilding.size(); i++ ){
		m_vpBuilding[i]->RenderInside( mView, mProj, vLight, vEye );
	}

}

//光るところの色を定める.
void clsStage::SetColorTarget( const D3DXVECTOR4& vColor )
{ 
	m_vLightColorTarget = vColor;
	m_LightLightingFlg.SetFlg( m_vLightColor, m_vLightColorTarget );
}
void clsStage::SetColor( const D3DXVECTOR4& vColor )
{ 
	m_vLightColor = m_vLightColorTarget = vColor;
	m_LightLightingFlg.Init();
}
void clsStage::SetColorRed()
{
	SetColor( vLIGHT_COLOR_RED );
}


void clsStage::SetScale( const float fScale )
{
	m_pStageCollision->SetScale( fScale );

	//床と天井.
	m_pStageFloor->SetScale( fScale );
	m_pStageCelling->SetScale( fScale );

	assert( m_pLia );
	m_pLia->SetScale( fScale );

	for( int i=0; i<enDOOR_NUM_size; i++ ){
		assert( m_pDoorArray[i] );
		m_pDoorArray[i]->SetScale( fScale );
		m_pDoorArray[i]->SetScale( fScale );
	}

	for( unsigned int i=0; i<m_vpBuilding.size(); i++ ){
		m_vpBuilding[i]->SetPos( m_vpBuilding[i]->GetPos() * fScale );
		m_vpBuilding[i]->SetScale( m_vpBuilding[i]->GetScale() * fScale );
	}
}

//光の明滅.
void clsStage::UpdateLight()
{
	Lighting( &m_vLightColor.x, m_vLightColorTarget.x, &m_LightLightingFlg.isLightingR );
	Lighting( &m_vLightColor.y, m_vLightColorTarget.y, &m_LightLightingFlg.isLightingG );
	Lighting( &m_vLightColor.z, m_vLightColorTarget.z, &m_LightLightingFlg.isLightingB );

}

//光の描く数値.
void clsStage::Lighting( float* const fpColor, const float fTarget, bool* const ispLiting )
{
	const float fLIGHT_OFFSET = 0.25f;
	const float fLIGHT_ADD = fLIGHT_OFFSET * fLIGHT_OFFSET * fLIGHT_OFFSET;

	//明るくなる.
	if( *ispLiting ){
		*fpColor += fLIGHT_ADD;
		if( *fpColor >= fTarget + fLIGHT_OFFSET ){
			*ispLiting = !*ispLiting;
		}
	}
	//暗くなる.
	else{
		*fpColor -= fLIGHT_ADD;
		if( *fpColor <= fTarget - fLIGHT_OFFSET ){
			*ispLiting = !*ispLiting;
		}
	}

}

void clsStage::SetAnimDoor( const enDOOR enDoor, const enDOOR_ANIM enAnimNo )
{
	switch( enDoor )
	{
	case enDOOR_DOOR_0:
		m_pDoorArray[ enDOOR_NUM_0 ]->SetAnimChange( enAnimNo );
		break;
	case enDOOR_DOOR_1:
		m_pDoorArray[ enDOOR_NUM_1 ]->SetAnimChange( enAnimNo );
		break;
	case enDOOR_Lia:
		m_pLia->SetAnimChange( enAnimNo );
		break;
	default:
		assert( !"そんなドアは存在しない" || enDoor || enAnimNo );
		break;
	}
}

//ドアが開いたら空きっぱなし.
void clsStage::SafeDoorAnimation()
{
	clsSkinMesh* pTmpDoorArray[] = {
		m_pDoorArray[ enDOOR_NUM_0 ].get(),
		m_pDoorArray[ enDOOR_NUM_1 ].get(),
		m_pLia.get()
	};

	for( auto pDoor : pTmpDoorArray ){
		//アニメが終了する瞬間じゃなかったらスルー.
		if( !pDoor->IsAnimTimeEnd() ){ continue; }

		enDOOR_ANIM enAnimNo = static_cast<enDOOR_ANIM>( pDoor->GetAnimNo() );
		switch( enAnimNo )
		{
		case enDOOR_ANIM_OPENING:
			pDoor->SetAnimChange( enDOOR_ANIM_OPENED );
			break;
		case enDOOR_ANIM_CLOSING:
			pDoor->SetAnimChange( enDOOR_ANIM_CLOSED );
			break;
		}
	}
}


vector<clsDX9Mesh*> clsStage::GetStageMeshArray()
{
	vector<clsDX9Mesh*> vvpMeshArrayTmp;

	int iSize = sizeof(clsDX9Mesh*) * (m_vpBuilding.size() + 1);

	vvpMeshArrayTmp.reserve(iSize);

	for (unsigned int i = 0; i < m_vpBuilding.size(); i++)
	{
		vvpMeshArrayTmp.push_back(m_vpBuilding[i]->GetModelPtr());
	}

	vvpMeshArrayTmp.push_back(m_pStageCollision->GetStaticMesh());

	vvpMeshArrayTmp.shrink_to_fit();

	return vvpMeshArrayTmp;
}

void clsStage::SetStageObjTransform(const unsigned int uiObjNo)
{
	if (uiObjNo >= m_vpBuilding.size())
	{
		return;
	}

	m_vpBuilding[uiObjNo]->UpdateModel();
}


//ビルの近くにいるか( 上から見た円の判定 ).
//添え字のビルのレイを判定するかどうかの判定として使う.
bool clsStage::isNearBuilding( 
	const D3DXVECTOR3& vPosObjOtherBuilding,//ビルと判定を取りたいモノの座標.
	const unsigned int uiBuildingNo )		//ビルの番号.
{
	//ビルしか判定しません( GetStageMeshArray()に合わせる ).
	if( uiBuildingNo >= m_vpBuilding.size() ){ return true; };

	return m_vpBuilding[ uiBuildingNo ]->isNearBuilding( vPosObjOtherBuilding );
}


//ビルの中に入ったギガポンを追い出す.
void clsStage::KickOutInsideBuilding( clsRobo* const pRobo )
{
	for( auto& Building_i : m_vpBuilding ){
		Building_i->KickOutInside( pRobo );
	}
}
