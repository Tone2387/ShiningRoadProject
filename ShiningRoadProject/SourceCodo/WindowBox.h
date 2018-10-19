#pragma once

#include "LineBox.h"


class clsWINDOW_BOX : public clsLINE_BOX
{
public:
	clsWINDOW_BOX(		
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11 );

	virtual ~clsWINDOW_BOX();


	virtual void Update() override;
	virtual void Render() override;


protected:

	virtual void SetColor( const D3DXVECTOR3& vColor ) override {
		clsLINE_BOX::SetColor( vColor );
		m_upBack->SetColor( vColor );
	}


private:


	std::unique_ptr< clsSPRITE2D_CENTER >	m_upBack;//�w�i�摜.

};

