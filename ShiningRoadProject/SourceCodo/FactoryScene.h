#pragma once

#include "SceneClass.h"
#include "SceneTitle.h"
#include "SceneAssemble.h"
#include "SceneMission.h"
#include "SceneEnding.h"


class clsCAMERA_BASE;

//基底クラス.
class clsFACTORY
{
public:
	//引数はシーンの初期化に必要なもの.
	clsFACTORY( clsPOINTER_GROUP* const ptrGroup );
	virtual ~clsFACTORY();
	//外から使うガワ.
	//シーンのポインタに「new」の代わりに使う.
	clsSCENE_BASE* Create( 
		const enSCENE enScene );


private:
	//中身.
	virtual clsSCENE_BASE* CreateProduct( 
		const enSCENE enScene ) = 0;



};



//「gameMgr」でシーンを作るクラス.
class clsSCENE_FACTORY : public clsFACTORY
{
public:
	clsSCENE_FACTORY( 
		clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_FACTORY() override;
private:
	//中身.
	clsSCENE_BASE* CreateProduct( 
		const enSCENE flg ) final;
	//消すときdeleteしないでnullしてね.
	clsPOINTER_GROUP* m_wpPtrGroup;

};

