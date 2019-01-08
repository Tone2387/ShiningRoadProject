#ifndef CAMERA_BASE_H_
#define CAMERA_BASE_H_

#include "Global.h"



//カメラの基底クラス.
class clsCAMERA_BASE
{
public:
	clsCAMERA_BASE();
	virtual ~clsCAMERA_BASE();

	//最初( シーンのはじめ )の初期化.
	virtual void Create() = 0;
	//毎フレームの処理.
	virtual void Update() = 0;

	//第二引数がtrue->m_vLookもついてくる.
	//第二引数がfalse->m_vLookはそのままの位置.
	virtual void SetPos( const D3DXVECTOR3& vPos, bool isWithLook = true ){
		if( isWithLook ){
			D3DXVECTOR3 tmpVec;//カメラの移動量.
			tmpVec = vPos - m_vPos;
			m_vLook = vPos + tmpVec;
		}
		m_vPos = vPos;
	};
	virtual void AddPos( const D3DXVECTOR3& vVec, bool isWithLook = true ){
		if( isWithLook ){ m_vLook += vVec; }
		m_vPos += vVec;
	};
	D3DXVECTOR3 GetPos() const { return m_vPos; };


	virtual void SetLookPos( const D3DXVECTOR3& vPos )	{ m_vLook = vPos; };
	virtual void AddLookPos( const D3DXVECTOR3& vVec )	{ m_vLook += vVec; };
	D3DXVECTOR3 GetLookPos() const				{ return m_vLook; };

	D3DXVECTOR3 GetRot() const				{ return m_vRot; };
	void SetRot( const D3DXVECTOR3 &vRot )	{ m_vRot = vRot; };

protected:

	D3DXVECTOR3 m_vPos;	//カメラ位置.
	D3DXVECTOR3 m_vLook;//注視位置.
	D3DXVECTOR3 m_vRot;	//回転値.

	D3DXMATRIX	m_mRot;	//回転行列.

};

#endif//#ifndef CAMERA_BASE_H_