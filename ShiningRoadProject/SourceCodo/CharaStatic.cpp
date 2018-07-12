#include "CharaStatic.h"



clsCharaStatic::clsCharaStatic() :
	m_pModel( nullptr )
{
}

clsCharaStatic::~clsCharaStatic()
{
	DetatchModel();
}



//レンダリング関数.
void clsCharaStatic::Render(
	const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
	const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye,
	const D3DXVECTOR4 &vColor , const bool bAlpha )
{
	if( m_pModel == nullptr ) return;

	//座標更新する.
	UpDateModel();
	
	//メッシュのレンダリング関数を呼ぶ.
	m_pModel->Render( mView, mProj, vLight, vEye, vColor, bAlpha );
}

//モデルデータ関連付け関数clsDX9Mesh
void clsCharaStatic::AttachModel( clsDX9Mesh* const pModel )
{
	if( pModel != nullptr ){
		m_pModel = pModel;
	}
}

//モデルデータ関連付け解除関数.
void clsCharaStatic::DetatchModel()
{
	m_pModel = nullptr;
}




//座標や回転値を更新する.
void clsCharaStatic::UpDateModel()
{
	if( m_pModel == nullptr ){
		return;
	}

	m_pModel->m_Trans.vPos	= m_vPos;
	m_pModel->m_Trans.fYaw	= m_vRot.y;
	m_pModel->m_Trans.fPitch= m_vRot.x;
	m_pModel->m_Trans.fRoll	= m_vRot.z;
	m_pModel->m_Trans.vScale = m_vScale;
}


//============================================================
//	位置更新関数.
//============================================================
void clsCharaStatic::UpdatePos()
{
	//キャラクタ移動処理(ラジコン)
	D3DXMATRIX mYaw;
	D3DXMatrixRotationY( &mYaw, m_vRot.y );

	//Z軸ベクトルを用意.
	D3DXVECTOR3	vecAxisZ( 0.0f, 0.0f, 1.0f );

	//Z軸ベクトルそのものを回転状態により変換する.
	D3DXVec3TransformCoord(
		&vecAxisZ,	//(out)
		&vecAxisZ,
		&mYaw );	//Y軸回転行列.

	//方向によって進行させる値を設定.
	switch( m_enDir )
	{
	case enDIRECTION::STOP:
		break;
	case enDIRECTION::FOWARD:	//前進.
		//向かう方向*進める値(0.1f).
		m_vPos -= vecAxisZ * 0.15f * m_fSpd;
		break;
	case enDIRECTION::BACK:	//後退.
		m_vPos += vecAxisZ * 0.15f * m_fSpd;
		break;
	}

}



clsDX9Mesh* clsCharaStatic::GetModelPtr()
{
	return m_pModel;
}



