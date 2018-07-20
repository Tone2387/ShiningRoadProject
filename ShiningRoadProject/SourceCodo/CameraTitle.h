#pragma once

#include "Camera.h"


class clsCAMERA_TITLE : public clsCAMERA_BASE
{
public:
	clsCAMERA_TITLE();
	~clsCAMERA_TITLE();

	void Create() final;

	void Update() final;


private:


	enum class enMODE : UCHAR
	{
		START = 0,	//タイトル画面開始時.
		IDLE,		//待機.

		UP_1,

		SPN_L,
		SPN_R,
	}m_enMode;

	//各モードの初期化.
	void Init( const enMODE enMode );

	//監視対象を中心に旋回する.
	void Spn( const float fSpn );

	//カメラ位置を中心にして見回す.
	void Turn( const float fTurn );

	//今のカメラ向きに前進.advancing
	void Advancing( const float fMove );

	float m_fDistance;//カメラと視点の距離( x&z軸だけの二次元上での ).

	D3DXVECTOR3 m_vMoveSpd;	//速度.
	D3DXVECTOR3 m_vMoveAcc;	//加速度.

};

