#ifndef SCENE_CLEAR_H_
#define SCENE_CLEAR_H_

class clsStage;
class clsROBO_TAKEOFF;

#include "SceneBase.h"
//#include "CameraTakeoff.h"

class clsSCENE_CLEAR : public clsSCENE_BASE
{
public:
	clsSCENE_CLEAR( clsPOINTER_GROUP* const ptrGroup );
	~clsSCENE_CLEAR();

private:
	void CreateProduct() final;
//	void UpdateProduct( enSCENE &enNextScene ) final;
	void RenderProduct( const D3DXVECTOR3 &vCamPos ) final;
	void RenderUi() final;//�u UI�� �vRender.

#ifdef _DEBUG
	//�f�o�b�N�J�e�L�X�g�̕\��.
	void RenderDebugText() final;
#endif//#ifdef _DEBUG

private:



};



#endif//#ifndef SCENE_CLEAR_H_