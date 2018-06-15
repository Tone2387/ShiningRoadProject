#pragma once

//#include "Global.h"

#include "Sprite2D.h"
#include <vector>

class clsASSEMBLE_UI
{
public:
	clsASSEMBLE_UI();
	~clsASSEMBLE_UI();

	void Create( 
		ID3D11Device* const pDevice, 
		ID3D11DeviceContext* const pContext );
	void Input();
	void Update();
	void Render();

private:

	enum enPARTS_TYPE
	{
		LEG = 0,
		CORE,
		HEAD,
		ARMS,
		WEAPON,
	
		enPARTS_TYPE_SIZE
	};

	std::vector<clsSprite2D*>	m_vpPartsType;
	clsSprite2D*				m_pPartsTypeSelect;
	std::vector<clsSprite2D*>	m_vpPartsNum;
	clsSprite2D*				m_pPartsNumSelect;
	clsSprite2D*				m_pStatusWindow;
};
