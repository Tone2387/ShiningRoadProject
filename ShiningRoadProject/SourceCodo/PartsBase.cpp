#include "PartsBase.h"

using namespace std;

//ボーンが見つからないときにメッセージを出すならつける.
//#define ERROR_MESSAGE_MACRO_BONE_NAME_NOT_FIND

clsPARTS_BASE::clsPARTS_BASE()
{	
}

clsPARTS_BASE::~clsPARTS_BASE()
{
}



//作成時.
void clsPARTS_BASE::Create()
{
	m_sPartsName = "NoName";
	CreateProduct();
}

//毎フレーム.
void clsPARTS_BASE::Update()
{
	UpdateProduct();
	ModelUpdate( m_Trans );
}

//描画.
void clsPARTS_BASE::Render(
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj, 
	const D3DXVECTOR3& vLight, 
	const D3DXVECTOR3& vEye,
	const D3DXVECTOR4& vColor,
	const bool isAlpha )
{
	ModelRender( mView, mProj, vLight, vEye, vColor, isAlpha );
}

//パーツの名前を覚える.
void clsPARTS_BASE::SetPartsName( const string &sPartsName )
{
	m_sPartsName = sPartsName;
}


D3DXVECTOR3 clsPARTS_BASE::GetBonePos( const char* sBoneName, const bool isLocalPos )
{
	D3DXVECTOR3 vBonePos;

	//ボーンの座標を取る.
	if( !m_pMesh->GetPosFromBone( sBoneName, &vBonePos, isLocalPos ) ){
		//ボーンが見つからなければ.
		string tmpS = m_sPartsName + "の" +  sBoneName;
#ifdef ERROR_MESSAGE_MACRO_BONE_NAME_NOT_FIND
		ERR_MSG( tmpS.c_str(), "そのモデルにその名前のボーンは存在しません" );
#endif//#ifdef ERROR_MESSAGE_MACRO_BONE_NAME_NOT_FIND
	}

	return vBonePos;
}


//アニメーション変更.//変更できるならtrue, 変更できないならfalseが返る.
bool clsPARTS_BASE::PartsAnimChange( const int iIndex )
{
	return SetAnimChange( iIndex );
}
