#ifndef WINDOW_BOX_H_
#define WINDOW_BOX_H_


#include "LineBox.h"
#include "Sprite2DCenter.h"


class clsWINDOW_BOX : public clsLINE_BOX
{
public:
	clsWINDOW_BOX(		
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11 );

	virtual ~clsWINDOW_BOX(){};


	virtual void Update() override {
		clsLINE_BOX::Update();
		m_upBack->SetPos( m_vPos );
		m_upBack->SetScale( m_vSize );
	};
	virtual void Render() override {
		m_upBack->Render();
		clsLINE_BOX::Render();
	}

	void SetAlpha( const float fAlpha ) override {
		clsLINE_BOX::SetAlpha( fAlpha );
		SetAlphaBack( fAlpha );
	};

	void SetAlphaBack( const float fAlpha ){
		m_fAlpha = fAlpha;
		m_upBack->SetAlpha( m_fAlpha );
	};

protected:

	virtual void SetColor( const D3DXVECTOR3& vColor ) override {
		clsLINE_BOX::SetColor( vColor );
		m_upBack->SetColor( vColor );
	}

private:


	std::unique_ptr< clsSPRITE2D_CENTER >	m_upBack;//”wŒi‰æ‘œ.

};

#endif//#ifndef WINDOW_BOX_H_