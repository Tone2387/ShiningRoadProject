#include "Stage.h"
#include "File.h"

using namespace std;

const char* sSTAGE_BASE_DATA_PATH = "Data\\Stage\\StageBase.csv";
const char* sBUILDING_DATA_PATH = "Data\\Stage\\Building.csv";

clsStage::clsStage( clsResource* const pResource )
{
	clsFILE file;
	file.Open( sSTAGE_BASE_DATA_PATH );
	//ìyë‰.
	m_pStageGround = make_unique< clsCharaStatic >();
	m_pStageGround->AttachModel( 
		pResource->GetStaticModels( clsResource::enStaticModel_StageBase ) );

	//ÉXÉeÅ[É^ÉXéÛÇØéÊÇË.
	int tmpI = 0;
	//ç¿ïW.
	m_pStageGround->SetPosition( {
		file.GetDataFloat( 0, tmpI++ ),
		file.GetDataFloat( 0, tmpI++ ),
		file.GetDataFloat( 0, tmpI++ ) } );
	//âÒì].
	m_pStageGround->SetRotation( {
		file.GetDataFloat( 0, tmpI++ ),
		file.GetDataFloat( 0, tmpI++ ),
		file.GetDataFloat( 0, tmpI++ ) } );
	//èké⁄.
	m_pStageGround->SetScale( {
		file.GetDataFloat( 0, tmpI++ ),
		file.GetDataFloat( 0, tmpI++ ),
		file.GetDataFloat( 0, tmpI++ ) } );
	file.Close();

	//è·äQï®.
	file.Open( sBUILDING_DATA_PATH );
	m_vpStageObject.resize( file.GetSizeRow() );
	for( unsigned int i=0; i<m_vpStageObject.size(); i++ ){
		m_vpStageObject[i] = make_unique< clsCharaStatic >();
		m_vpStageObject[i]->AttachModel( 
			pResource->GetStaticModels( clsResource::enStaticModel_Building ) );

		tmpI = 0;
		//ç¿ïW.
		m_vpStageObject[i]->SetPosition( {
			file.GetDataFloat( i, tmpI++ ),
			file.GetDataFloat( i, tmpI++ ),
			file.GetDataFloat( i, tmpI++ ) } );
		//âÒì].
		m_vpStageObject[i]->SetRotation( {
			file.GetDataFloat( i, tmpI++ ),
			file.GetDataFloat( i, tmpI++ ),
			file.GetDataFloat( i, tmpI++ ) } );
		//èké⁄.
		m_vpStageObject[i]->SetScale( {
			file.GetDataFloat( i, tmpI++ ),
			file.GetDataFloat( i, tmpI++ ),
			file.GetDataFloat( i, tmpI++ ) } );
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


bool clsStage::WallJudge( clsObject* const pObj )
{
	bool bHit = false;

	//StageGroundÇ∆ÇÃìñÇΩÇËîªíË.
	clsDX9Mesh* pGround = m_pStageGround->GetModelPtr();

	if (pObj->WallForward(pGround))if (!bHit)bHit = true;
	if (pObj->WallBack(pGround))if (!bHit)bHit = true;
	if (pObj->WallLeft(pGround))if (!bHit)bHit = true;
	if (pObj->WallRight(pGround))if (!bHit)bHit = true;

	pObj->m_bGround = pObj->WallUnder(pGround);
	if (pObj->m_bGround)if (!bHit)bHit = true;
	if (pObj->WallUp(pGround))if (!bHit)bHit = true;

	for (int i = 0; i < 1; i++)
	{
		clsDX9Mesh* pObjMesh = m_vpStageObject[i]->GetModelPtr();

		//StageObjectÇ∆ÇÃìñÇΩÇËîªíË.
		if (pObj->WallForward(pObjMesh))if (!bHit)bHit = true;
		if (pObj->WallBack(pObjMesh))if (!bHit)bHit = true;
		if (pObj->WallLeft(pObjMesh))if (!bHit)bHit = true;
		if (pObj->WallRight(pObjMesh))if (!bHit)bHit = true;

		if (pObj->WallUnder(pObjMesh))
		{
			pObj->m_bGround = true;
			if (!bHit)bHit = true;

			D3DXVECTOR3 vMoveDir = pObj->GetPosition();

			vMoveDir += {0.0f, 0.0f, 0.0f};

			pObj->SetPosition(vMoveDir);
		}

		if (pObj->WallUp(pObjMesh))if (!bHit)bHit = true;
	}

		

	return bHit;
}


