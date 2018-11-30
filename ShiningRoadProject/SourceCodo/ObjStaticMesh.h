#pragma once

//ステージのオブジェクト等に使う.

#include"Object.h"

class clsObjStaticMesh : public clsObject
{
public:
	void AttachModel(clsDX9Mesh* const pMesh);
	void DetatchModel();
	
	clsDX9Mesh* GetStaticMesh() const;

	void ModelTransUpdate();

	virtual void Render(const D3DXMATRIX& mView, const  D3DXMATRIX& mProj, const D3DXVECTOR3& vLight, const  D3DXVECTOR3& vEye, 
		const D3DXVECTOR4& vColor = { 1.0f, 1.0f, 1.0f, 1.0f } );

	clsObjStaticMesh();
	~clsObjStaticMesh();

private:
	clsDX9Mesh* m_pModel;
};

