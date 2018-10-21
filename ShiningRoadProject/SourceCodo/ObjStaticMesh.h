#pragma once

//ステージのオブジェクト等に使う.

#include"Object.h"

class clsObjStaticMesh : public clsObject
{
public:
	void AttachModel(clsDX9Mesh* const pMesh);
	void DttachModel();
	
	clsDX9Mesh* GetStaticMesh() const;

	void ModelTransUpdate();

	virtual void Render(D3DXMATRIX& const mView, D3DXMATRIX& const mProj, D3DXVECTOR3& const vLight, D3DXVECTOR3& const vEye);

	clsObjStaticMesh();
	~clsObjStaticMesh();

private:
	clsDX9Mesh* m_pModel;
};

