#include "PartsBase.h"

using namespace std;

//ボーンが見つからないときにメッセージを出すならつける.
//#define ERROR_MESSAGE_MACRO_BONE_NAME_NOT_FIND

clsPARTS_BASE::clsPARTS_BASE()
{	
	D3DXVECTOR3 vINIT_SCALE = { 1.0f, 1.0f, 1.0f };
	m_Trans.vScale = vINIT_SCALE;
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
	const D3DXVECTOR4& vColorBase,
	const D3DXVECTOR4& vColorArmor,
	const bool isAlpha )
{
	ModelRender( mView, mProj, vLight, vEye, vColorBase, vColorArmor, isAlpha );
}

//パーツの名前を覚える.
void clsPARTS_BASE::SetPartsName( const string &sPartsName )
{
	m_sPartsName = sPartsName;
}


D3DXVECTOR3 clsPARTS_BASE::GetBonePos( const char* sBoneName, const bool isLocalPos )
{
	D3DXVECTOR3 vBonePos;

	//これが無いと左右の武器が同じ場合に右武器に持っていかれる.
	ModelUpdate( m_Trans );

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

//ボーンが存在するか.
bool clsPARTS_BASE::ExistsBone( const char* sBoneName )
{
	return m_pMesh->ExistsBone( sBoneName );
}


//アニメーション変更.//変更できるならtrue, 変更できないならfalseが返る.
bool clsPARTS_BASE::PartsAnimChange( const int iIndex )
{
	return SetAnimChange( iIndex );
}
