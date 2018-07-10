#ifndef _SHOT_H_
#define _SHOT_H_

#include"Effects.h"
#include"Sound.h"
#include"Object.h"
#include "PtrGroup.h"

struct BulletState
{
	int iAtk;
	float fMoveSpeed;
	float fRangeMax;

	float fSpeed;
	float fScale;

	int iSEShotNum;
	int iSEHitNum;

	int iShotEfcNum;
	int iLineEfcNum;
	int iHitEfcNum;
};

struct ShootingState
{
	D3DXVECTOR3* vShotStartPos;
	D3DXVECTOR3* vShotMoveDir;
};

class clsShot : public clsObject
{
private:
	enum enSEType
	{
		enSEShot = 0,
		enSEHit,

		enSESize
	};

	enum enEfcType
	{
		enEfcShot = 0,
		enEfcLine,
		enEfcHit,

		enEfcSize
	};

public:
	clsShot(clsPOINTER_GROUP* pPtrGroup);
	~clsShot();

	int m_iThisShotIndex;

	bool m_bShotExistFlg;//íeÇÃë∂ç›ämîF(falseÇ»ÇÁíeÇ™ë∂ç›ÇµÇ»Ç¢èÛë‘).
	bool m_bExistFlg;//íe,îöî≠,ãOê’ÇÃë∂ç›ämîF(falseÇ»ÇÁÇªÇÍÇÁëSÇƒÇ™ë∂ç›ÇµÇ»Ç¢èÛë‘)

	HRESULT Init(BulletState BState);

	void SEInit(HWND hWnd);

	void Move();
	bool Hit(SPHERE* ppTargetSphere,int iSphereMax);
	bool Form(D3DXVECTOR3 vShotPos,D3DXVECTOR3 vTarget);
	void ReStart();

private:

	clsSound* m_ppSE[enSESize];

	clsEffects* m_wpEffect;

	::Effekseer::Handle m_ShotEfcHandles[enEfcSize];

	D3DXVECTOR3 m_vStartPos;

	BulletState m_ShotState;
};

#endif //#ifndef _SHOT_H_