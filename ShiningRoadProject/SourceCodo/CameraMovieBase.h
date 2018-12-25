#ifndef CAMERA_MOVIE_BASE_
#define CAMERA_MOVIE_BASE_

#include "Camera.h"


//カメラの動きを入れた基底クラス.
class clsCAMERA_MOVIE_BASE : public clsCAMERA_BASE
{
public:
	clsCAMERA_MOVIE_BASE();
	virtual ~clsCAMERA_MOVIE_BASE();

	//最初( シーンのはじめ )の初期化.
	virtual void Create() override = 0;
	//毎フレームの処理.
	virtual void Update() override = 0;


	//監視対象を中心に旋回する.
	void Spn( const float fSpn ){
		SpnProduct( fSpn, &m_vPos, &m_vLook, &m_vRot );
	}

	//カメラ位置を中心にして見回す.
	void Turn( const float fTurn ){
		TurnProduct( fTurn, &m_vPos, &m_vLook, &m_vRot );
	}

	//今のカメラ向きに前進.advancing
	void Advancing( const float fMove ){
		AdvancingProduct( fMove, &m_vPos, &m_vLook, &m_vRot );
	}
	//横平行移動( +で右側、-で左側に移動 ).
	void CrabWalk( const float fMove ){
		CrabWalkProduct( fMove, &m_vPos, &m_vLook, &m_vRot );
	}


	//視点との距離を変更.
	void SetDistance( const float fDistance, const bool isCamMove );
	void AddDistance( const float fDistance, const bool isCamMove );

protected:

	//監視対象を中心に旋回する.
	void SpnProduct( 
		const float fSpn, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;

	//カメラ位置を中心にして見回す.
	void TurnProduct( const float fTurn, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;

	//今のカメラ向きに前進.advancing
	void AdvancingProduct( 
		const float fMove, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;
	//横平行移動.
	void CrabWalkProduct( 
		const float fMove, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot ) const;


protected:

	float m_fDistance;//カメラと視点の距離( x&z軸だけの二次元上での ).


private:

};


#endif//#ifndef CAMERA_MOVIE_BASE_