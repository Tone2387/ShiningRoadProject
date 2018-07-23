#pragma once

#include "PtrGroup.h"
#include "Object.h"
#include "CharaStatic.h"

class clsStage
{
public:
	clsStage( clsResource* const pResource );
	~clsStage();


	void Render(				
		const D3DXMATRIX &mView, const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight, const D3DXVECTOR3 &vEye );

	bool WallJudge( clsObject* const pObj);

private:

	std::unique_ptr< clsCharaStatic > m_pStageGround;
	std::vector< std::unique_ptr< clsCharaStatic > > m_vpStageObject;


};
