#include "Stage.h"
#include "File.h"
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

	const double dDOOR_ANIM_SPD = 0.05;
}

clsStage::clsStage( clsPOINTER_GROUP* const pPtrGroup )
	:m_vLightColor( { 1.0f, 1.0f, 1.0f, 1.0f } )
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
		m_pLia->SetAnimChange( 1 );
		m_pLia->SetAnimSpeed( dDOOR_ANIM_SPD );

	for( int i=0; i<enDOOR_NUM_size; i++ ){
		m_pDoorArray[i] = make_unique< clsObjSkinMesh >();
		m_pDoorArray[i]->AttachModel(
			pPtrGroup->GetResource()->GetSkinModels( clsResource::enSkinModel_Door ) );
		m_pDoorArray[i]->SetPosition( m_pStageCollision->GetPosition() );
		//二つ目は反対の位置にするために回す.
		m_pDoorArray[i]->SetRotation( 
			m_pStageCollision->GetRotation()
			+ ( D3DXVECTOR3( 0.0f, static_cast<float>( D3DX_PI * i ), 0.0f ) )
			);
		m_pDoorArray[i]->SetScale( {
			file.GetDataFloat( 0, cINDEX_SCALE_X ),
			file.GetDataFloat( 0, cINDEX_SCALE_Y ),
			file.GetDataFloat( 0, cINDEX_SCALE_Z ) } );

		m_pDoorArray[i]->SetAnimSpeed( dDOOR_ANIM_SPD );
	}
	m_pDoorArray[ enDOOR_NUM_0 ]->SetAnimChange( 1 );
	m_pDoorArray[ enDOOR_NUM_1 ]->SetAnimChange( 3 );

	file.Close();
	//========== ドア 終わり ==========//.

	//障害物.
	file.Open( sBUILDING_DATA_PATH );
	m_vpBuilding.resize( file.GetSizeRow() );
	for( unsigned int i=0; i<m_vpBuilding.size(); i++ ){
		m_vpBuilding[i] = make_unique< clsBUILDING >( 
			pPtrGroup->GetDevice(), 
			pPtrGroup->GetContext(), 
			pPtrGroup->GetResource()->GetStaticModels( clsResource::enStaticModel_Building ) );

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
}
//enStaticModel_Building
clsStage::~clsStage()
{
//	m_pStageCollision->DetatchModel();

//	for( unsigned int i=0; i<m_vpBuilding.size(); i++ ){
//		m_vpBuilding[i]->DetatchModel();
//	}
}

void clsStage::Render(
	const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
	const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye )
{
	//当たり判定用モデル.
//	m_pStageCollision->Render( mView, mProj, vLight, vEye );

	//床と天井.
	m_pStageFloor->Render(	 mView, mProj, vLight, vEye, m_vLightColor );
	m_pStageCelling->Render( mView, mProj, vLight, vEye, m_vLightColor );

	assert( m_pLia );
	m_pLia->ModelRender( mView, mProj, vLight, vEye, m_vLightColor );

	for( int i=0; i<enDOOR_NUM_size; i++ ){
		assert( m_pDoorArray[i] );
		m_pDoorArray[i]->ModelUpdate( m_pDoorArray[i]->m_Trans );
		m_pDoorArray[i]->ModelRender( mView, mProj, vLight, vEye, m_vLightColor );
	}

	for( unsigned int i=0; i<m_vpBuilding.size(); i++ ){
		m_vpBuilding[i]->Render( mView, mProj, vLight, vEye, m_vLightColor );
	}

#ifdef _DEBUG
	//色.
	const float fSTATIC_MODEL_COLOR_RGB_ADD = 0.025f; 
	if( GetAsyncKeyState( 'F' ) & 0x8000 )m_vLightColor.x += fSTATIC_MODEL_COLOR_RGB_ADD;
	if( GetAsyncKeyState( 'V' ) & 0x8000 )m_vLightColor.x -= fSTATIC_MODEL_COLOR_RGB_ADD;
	if( GetAsyncKeyState( 'G' ) & 0x8000 )m_vLightColor.y += fSTATIC_MODEL_COLOR_RGB_ADD;
	if( GetAsyncKeyState( 'B' ) & 0x8000 )m_vLightColor.y -= fSTATIC_MODEL_COLOR_RGB_ADD;
	if( GetAsyncKeyState( 'H' ) & 0x8000 )m_vLightColor.z += fSTATIC_MODEL_COLOR_RGB_ADD;
	if( GetAsyncKeyState( 'N' ) & 0x8000 )m_vLightColor.z -= fSTATIC_MODEL_COLOR_RGB_ADD;
#endif//#ifdef _DEBUG

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

void clsStage::SetStageObjTransform(const int iObjNo)
{
	if (iObjNo >= m_vpBuilding.size())
	{
		return;
	}

	m_vpBuilding[iObjNo]->UpdateModel();
}