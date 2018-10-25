#ifndef CAMERA_TITLE_H_
#define CAMERA_TITLE_H_

#include "Camera.h"


class clsCAMERA_TITLE : public clsCAMERA_BASE
{
public:
	clsCAMERA_TITLE();
	~clsCAMERA_TITLE();

	void Create() final;

	void Update() final;

	//フラッシュ( ごまかし )のタイミングを教えてくれる.
	bool isFlash();

	D3DXVECTOR3 GetSpd(){
		return m_vMoveSpd;
	};

	D3DXVECTOR3 GetAcc(){
		return m_vMoveAcc;
	};


private:

	struct CAM_GHOST
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vLook;
		D3DXVECTOR3 vRot;

		CAM_GHOST(){
			vPos = vLook = vRot = { 0.0f, 0.0f, 0.0f };
		};
	};

	enum class enMODE : UCHAR
	{
		START = 0,	//タイトル画面開始時.

		SPN_L,		//左回りで回転.
		SPN_L_2,	//ズームもどきを加える.
	}m_enMode;

	//各モードの初期化.
	void Init( const enMODE enMode );

	//監視対象を中心に旋回する.
	void Spn( 
		const float fSpn, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot );

	//カメラ位置を中心にして見回す.
	void Turn( const float fTurn, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot );

	//今のカメラ向きに前進.advancing
	void Advancing( 
		const float fMove, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot );
	//横平行移動.
	void CrabWalk( 
		const float fMove, 
		D3DXVECTOR3* const vPos,
		D3DXVECTOR3* const vLook,
		D3DXVECTOR3* const vRot );

	//視点との距離を変更.
	void AddDistance( const float fAdd, const bool isCamMove );


	//くるくる.
	void CameraSpnFunction();
	//ズーム.
	void CameraFakeZoomFunction();

	float m_fDistance;//カメラと視点の距離( x&z軸だけの二次元上での ).

	D3DXVECTOR3 m_vMoveSpd;	//速度.
	D3DXVECTOR3 m_vMoveAcc;	//加速度.

	//どちらへ動くかのフラグ.
	struct BOOL3
	{
		bool x;
		bool y;
		bool z;
		BOOL3(){
			x = y = z = false;
		}
	}m_MoveFlg;

	CAM_GHOST m_CamGhost;

	bool m_isFlash;
};

#endif//#ifndef CAMERA_TITLE_H_