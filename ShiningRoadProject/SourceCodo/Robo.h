#pragma once

#include"Charactor.h"
#include"SkinMesh.h"

//太原の書き足した分.
#ifdef Tahara
#include "PtrGroup.h"

//2018/05/16( 水 ).
	//関数「RoboInit」に「clsPOINTER_GROUP」型の引数を追加し、.
	//追加したメンバ変数( m_wpResource、m_wpEffects、m_wpSound ).
	//にアドレスを割り当てる処理を追加しました.
	
	//装飾子の都合上このクラスの「Render」関数と.
	//「clsSkinMesh」クラスの「ModelRender」関数と.
	//「clsD3DXSKINMESH」クラスの「Render」関数の.
	//引数にconstを付けました.
	
	//Zeromemoryはポインタが大暴れするので初期化を一つ一つ行いました.
	//↑「clsCharactor」と「clsObject」「clsSkinMesh」も.

#endif//#ifdef Tahara

const int g_iQuickInterbal = 1 * (int)g_fFPS;
const int g_iQuickTurnFrame = 1 * (int)g_fFPS;

class clsRobo : public clsCharactor
{
public:
	/*clsSkinMesh * m_pHead;
	clsSkinMesh * m_pCore;
	clsSkinMesh * m_pLeftArm;
	clsSkinMesh * m_pRightArm;
	clsSkinMesh * m_pLeg;*/
	clsSkinMesh* m_pMesh;

	void RoboInit(HWND hWnd,
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		clsPOINTER_GROUP* const pPtrGroup );

	void ModelUpdate()
	{
		m_pMesh->ModelUpdate(m_Trans);
	}

	void Render( 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR4 &vColor = { 1.0f, 1.0f, 1.0f ,1.0f },
		const bool alphaFlg = false )
	{
		ModelUpdate();
		m_pMesh->ModelRender(mView, mProj, vLight, vEye, vColor, alphaFlg );
	}

	virtual void tenshi()override{};

	bool m_bBoost;//true:ブースター展開/false:非展開.

	//Headパーツから数値を取得する変数と関連する変数//

	//Coreパーツから数値を取得する変数と関連する変数//
	int m_iEnelgyMax;//エネルギー最大容量.
	int m_iEnelgy;//現在エネルギー残量.
	int m_iEnelgyOutput;//エネルギー出力(回復量)

	float m_fBoostMoveSpeedMax;

	float m_fQuickMoveSpeedMax;//
	int m_iQuickEnelgyCost;
	int m_iQuickTopSpeedTime;//最高速を保つフレーム値.

	int m_iQuickDecStartTime;//残クイック噴射時間.

	int m_iQuickInterbal;//クイックブーストの再噴射までのフレーム.

	float m_fBoostRisingSpeedMax;//ブースター垂直移動の最高速.
	int m_iBoostRisingTopSpeedFrame;//↑に達するまでのフレーム値.
	float m_fBoostRisingAccele;// = m_fMoveSpeedMax / m_fTopSpeedFrame;
	float m_fBoostFollRes;//ブースター展開時に落ちる速度.

	//Armパーツから数値を取得する変数と関連する変数//

	//Legパーツから数値を取得する変数と関連する変数//
	float m_fWalktMoveSpeedMax;//Legパーツから取得する歩行速度.
	int m_iWalkTopSpeedFrame;//安定性能から計算.
	
	int m_iBoostTopSpeedFrame;//安定性能から計算.

	void Walk();
	void Boost();
	void MoveSwitch();
	void BoostRising();
	void QuickBoost();
	void SetDirQuickBoost(const float fAngle);
	void QuickTurn();
	void SetDirQuickTurn(const float fAngle);

	void Updata();

	clsRobo();
	~clsRobo();

private:


//太原の書き足した分.
#ifdef Tahara
	//消すときdeleteしないでnullしてね.
	clsResource*		m_wpResource;
	clsEffects*			m_wpEffects;
	clsSOUND_MANAGER*	m_wpSound;
#endif//#ifdef Tahara


};


