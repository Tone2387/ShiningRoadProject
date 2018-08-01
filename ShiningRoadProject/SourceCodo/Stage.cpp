#include "Stage.h"
#include "File.h"

using namespace std;

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


clsStage::clsStage( clsResource* const pResource )
{
	clsFILE file;
	file.Open( sSTAGE_BASE_DATA_PATH );
	//土台.
	m_pStageGround = make_unique< clsCharaStatic >();
	m_pStageGround->AttachModel( 
		pResource->GetStaticModels( clsResource::enStaticModel_StageBase ) );

	//ステータス受け取り.
	//座標.
	m_pStageGround->SetPosition( {
		file.GetDataFloat( 0, cINDEX_POS_X ),
		file.GetDataFloat( 0, cINDEX_POS_Y ),
		file.GetDataFloat( 0, cINDEX_POS_Z ) } );
	//回転.
	m_pStageGround->SetRotation( {
		file.GetDataFloat( 0, cINDEX_ROT_X ),
		file.GetDataFloat( 0, cINDEX_ROT_Y ),
		file.GetDataFloat( 0, cINDEX_ROT_Z ) } );
	//縮尺.
	m_pStageGround->SetScale( {
		file.GetDataFloat( 0, cINDEX_SCALE_X ),
		file.GetDataFloat( 0, cINDEX_SCALE_Y ),
		file.GetDataFloat( 0, cINDEX_SCALE_Z ) } );
	file.Close();

	//障害物.
	file.Open( sBUILDING_DATA_PATH );
	m_vpStageObject.resize( file.GetSizeRow() );
	for( unsigned int i=0; i<m_vpStageObject.size(); i++ ){
		m_vpStageObject[i] = make_unique< clsCharaStatic >();
		m_vpStageObject[i]->AttachModel( 
			pResource->GetStaticModels( clsResource::enStaticModel_Building ) );

		//座標.
		m_vpStageObject[i]->SetPosition( {
			file.GetDataFloat( i, cINDEX_POS_X ),
			file.GetDataFloat( i, cINDEX_POS_Y ),
			file.GetDataFloat( i, cINDEX_POS_Z ) } );
		//回転.
		m_vpStageObject[i]->SetRotation( {
			file.GetDataFloat( i, cINDEX_ROT_X ),
			file.GetDataFloat( i, cINDEX_ROT_Y ),
			file.GetDataFloat( i, cINDEX_ROT_Z ) } );
		//縮尺.
		m_vpStageObject[i]->SetScale( {
			file.GetDataFloat( i, cINDEX_SCALE_X ),
			file.GetDataFloat( i, cINDEX_SCALE_Y ),
			file.GetDataFloat( i, cINDEX_SCALE_Z ) } );
	}
	file.Close();
}
//enStaticModel_Building
clsStage::~clsStage()
{
	m_pStageGround->DetatchModel();

	for( unsigned int i=0; i<m_vpStageObject.size(); i++ ){
		m_vpStageObject[i]->DetatchModel();
	}
}

void clsStage::Render(
	const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
	const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye )
{
	m_pStageGround->Render( mView, mProj, vLight, vEye );

	for( unsigned int i=0; i<m_vpStageObject.size(); i++ ){
		m_vpStageObject[i]->Render( mView, mProj, vLight, vEye );
	}
}

vector<clsDX9Mesh*> clsStage::GetStageMeshArray()
{
	vector<clsDX9Mesh*> vvpMeshArrayTmp;

	vvpMeshArrayTmp.push_back(m_pStageGround->GetModelPtr());

	for (int i = 0; i < m_vpStageObject.size(); i++)
	{
		vvpMeshArrayTmp.push_back(m_vpStageObject[i]->GetModelPtr());
	}

	int iSize = m_vpStageObject.size() + 1;

	vvpMeshArrayTmp.resize(iSize);

	return vvpMeshArrayTmp;
}

