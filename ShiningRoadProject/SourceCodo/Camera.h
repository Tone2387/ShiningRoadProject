#pragma once

#include "Global.h"



//カメラの基底クラス.
class clsCAMERA_BASE
{
public:
	clsCAMERA_BASE();
	virtual ~clsCAMERA_BASE();

	//最初( シーンのはじめ )の初期化.
	virtual void Create() = 0;

	//第二引数がtrue->m_vLookもついてくる.
	//第二引数がfalse->m_vLookはそのままの位置.
	virtual void SetPos( const D3DXVECTOR3& vPos, bool isWithLook = true );
	virtual void AddPos( const D3DXVECTOR3& vPos, bool isWithLook = true );

	D3DXVECTOR3 GetPos();
	D3DXVECTOR3 GetLookPos();

protected:

	D3DXVECTOR3 m_vPos;	//カメラ位置.
	D3DXVECTOR3 m_vLook;//注視位置.
	D3DXVECTOR3 m_vRot;	//回転値.

	D3DXMATRIX	m_mRot;	//回転行列.

};

