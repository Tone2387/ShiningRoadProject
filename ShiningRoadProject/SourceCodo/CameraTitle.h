#ifndef CAMERA_TITLE_H_
#define CAMERA_TITLE_H_

#include "CameraMovieBase.h"


class clsCAMERA_TITLE : public clsCAMERA_MOVIE_BASE
{
public:
	clsCAMERA_TITLE();
	~clsCAMERA_TITLE();

	void Create() final {};

	void Update() final;

	//フラッシュ( ごまかし )のタイミングを教えてくれる.
	bool isFlash();

	D3DXVECTOR3 GetSpd() const { return m_vMoveSpd; };

	D3DXVECTOR3 GetAcc() const { return m_vMoveAcc; };


private:


	enum class enMODE : UCHAR
	{
		START = 0,	//タイトル画面開始時.

		SPN_L,		//左回りで回転.
		SPN_L_2,	//ズームもどきを加える.
	}	m_enMode;

	//各モードの初期化.
	void Init( const enMODE enMode );



	//くるくる.
	void CameraSpnFunction();
	//ズーム.
	void CameraFakeZoomFunction();

private:

	struct CAM_GHOST
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vLook;
		D3DXVECTOR3 vRot;

		CAM_GHOST(){
			vPos = vLook = vRot = { 0.0f, 0.0f, 0.0f };
		};
	}	m_CamGhost;



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
	}	m_MoveFlg;


	bool m_isFlash;
};

#endif//#ifndef CAMERA_TITLE_H_