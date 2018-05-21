#ifndef _ENEMY_H_
#define _ENEMY_H_
#pragma warning(disable:4005)//警告についてのｺｰﾄﾞ分析を無効にする.4005:再定義.

#define _CRT_SECURE_NO_WARNINGS

#include"CD3DXSKINMESH.h"
#include"DX9Mesh.h"
#include"CSkinChara.h"
#include<math.h>
#include"UISprite.h"
#include "Resource.h"
#include "Shot.h"

//========================================================
//	定数.
//========================================================
#define E_MOVE_SPEED 0.075f//移動速度.
#define E_HP 10

#define E_DOWNBOM 2

#define E_SHOTCNT 60

#define HP_DISP 5

//========================================================
//	構造体.
//========================================================

/*struct sound_data//ｻｳﾝﾄﾞ構造体.
{
char sPath[256];//ﾌｧｲﾙ名.
char sAlias[256];//ｴｲﾘｱｽ.
};*/



//========================================================
//	ﾌﾟﾚｲﾔｰｸﾗｽ.
//========================================================

enum E_MOVEPATTERN
{
	enMPattaern_None = 0,
	enMPattaern_Normal,
};

struct FILELOAD
{
	D3DXVECTOR3 vOrizinPos;
	int MovePattern;
};

class clsEnemy : public clsSkinChara
{
public:
	//========================================================
	//	列挙体.
	//========================================================

	//軸ﾍﾞｸﾄﾙ.
	D3DXVECTOR3 vecAxisZ;
	D3DXVECTOR3 vecAxisX;

	D3DXVECTOR3 m_vCamPos;

	int HP;//体力.
	int m_MaxHP;//最大体力.

	//位置構造体.
	//DXSTATIC_TRANSFORM m_StaticTrans;

	clsDX9Mesh* m_pDX9Mesh;

	bool m_bDeadFlg;//死亡ﾌﾗｸﾞ.

	//音声(ここで使うかはわからない)
	clsSound** m_pSE;
	int m_iSoundMaxNo;

	int m_iShotMax;
	int m_iShotCnt;
	D3DXVECTOR3 m_vShotStartPos;
	clsShot** m_ppShot;

	//当たり判定関係.
	SPHERE m_BodySphere;

	bool ShotCollision(SPHERE* pTargrt, int iSphereMax = 1);
	bool BodyCollision(SPHERE* pTargrt, int iSphereMax = 1);
	
#if _DEBUG
	//ﾊﾞｳﾝﾃﾞｨﾝｸﾞｽﾌｨｱ用.
	clsDX9Mesh* m_pBodySphere;
#endif

	D3DXVECTOR3 m_vOriginPos;//定位置.
	D3DXVECTOR3 m_vOldPos;

	int m_iThisEnemyIndex;
	int m_iMovePattern;

	/*====/ ｴﾌｪｸﾄ /====*/

	clsEffects* m_pEffect;
	/*/ ﾊﾝﾄﾞﾙ /*/
	::Effekseer::Handle m_DownEfc;
	::Effekseer::Handle m_DeadEfc;

	int m_iDownEfcCnt;

	clsEnemy();
	~clsEnemy();

	HRESULT Init(HWND hWnd, ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);

	void HPUIRender(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vEye);

	bool LockOnFlg;
	void LockOnRender(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vEye);

	void GameMove(D3DXVECTOR3 PlayerPos, clsDX9Mesh* Ground);//ｹﾞｰﾑ中での動き.

	void SEInit(HWND hWnd);
	void SEUpdate();

	void UpdateCollisionPos();

	void TrackingMove(D3DXVECTOR3 PlayerPos, clsDX9Mesh* Ground);

	void Update();
	void Render(D3DXMATRIX& mView, D3DXMATRIX& mProj, D3DXVECTOR3& vLight, D3DXVECTOR3& vEye, LPD3DXANIMATIONCONTROLLER pAC = NULL);

	void Hit(int Damage);

	void VectorInput(FILE *fp, int No);//csvﾌｧｲﾙからfloatの数値を入れる(改良の余地あり)

	void ReStart();

private:
	clsUISprite* m_pHp;//HPｹﾞｰｼﾞ表示.
	clsUISprite* m_pHpFrame;//HPｹﾞｰｼﾞの枠.
	clsUISprite* m_pLockOn;//ﾛｯｸｵﾝｶｰｿﾙ.

	void HPUIInit(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	void HPUIControl();

	void LockOnInit(ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11);
	void LockOnControl();
};

#endif//#ifndef _ENEMY_H_