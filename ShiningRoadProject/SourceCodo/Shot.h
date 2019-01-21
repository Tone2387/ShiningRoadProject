#ifndef _SHOT_H_
#define _SHOT_H_

#include"Effects.h"
#include"Sound.h"
#include"Object.h"
#include "PtrGroup.h"

struct BulletState
{
	float fRangeMax;

	float fSpeed;
	float fScale;

	int iSEShotNum;
	int iSEHitNum;

	int iShotEfcNum;
	int iMuzzeleEfcNum;
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
		enEfcMuzzle,
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

	HRESULT Init(const BulletState& BState);

	void SEInit(HWND hWnd);

	bool Hit(std::vector<clsObject::SPHERE>& v_TargetSphere);
	bool Form(const D3DXVECTOR3& vShotPos,const D3DXVECTOR3& vTarget);
	void ReStart();

private:

	virtual void UpdateProduct(clsStage* pStage);
	virtual void Action(clsStage* const pStage);

	void Explosion();

	clsSound* m_ppSE[enSESize];

	clsEffects* m_wpEffect;
	clsSOUND_MANAGER_BASE* m_wpSound;

	::Effekseer::Handle m_ShotEfcHandles[enEfcSize];

	D3DXVECTOR3 m_vStartPos;
	float m_fRangeMax;

	BulletState m_ShotState;
};

#endif //#ifndef _SHOT_H_