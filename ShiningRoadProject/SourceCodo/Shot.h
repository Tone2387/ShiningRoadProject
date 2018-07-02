#ifndef _SHOT_H_
#define _SHOT_H_

#include"Effects.h"
#include"Sound.h"
#include"Object.h"
#include "PtrGroup.h"

struct ShotInitState
{
	float fShotScale;

	clsSound* pSEShot;
	clsSound* pSEHit;

	
};

class clsShot : public clsObject
{
public:
	clsShot(clsPOINTER_GROUP* pPtrGroup);
	~clsShot();

	int m_iThisShotIndex;

	clsSound** m_ppSE;
	int m_iSoundMaxNo;

	bool m_bShotExistFlg;//弾の存在確認(falseなら弾が存在しない状態).
	bool m_bExistFlg;//弾,爆発,軌跡の存在確認(falseならそれら全てが存在しない状態)

	HRESULT Init(LPSTR strWeaponFolderName);

	clsEffects* m_pEffect;

	::Effekseer::Handle m_ShotEfcH;
	::Effekseer::Handle m_LineEfcH;
	::Effekseer::Handle m_HitEfcH;

	void SEInit(HWND hWnd);

	void Move();
	bool Hit(SPHERE* ppTargetSphere,int iSphereMax);
	bool Form(D3DXVECTOR3 vShotPos,D3DXVECTOR3 vTarget);
	void ReStart();

private:
	D3DXVECTOR3 m_vStartPos;
	D3DXVECTOR3 m_vMoveAxis;
};

#endif //#ifndef _SHOT_H_