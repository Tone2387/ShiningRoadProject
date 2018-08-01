#pragma once


#include "AssembleModel.h"

class clsPARTS_WINDOW_MODEL
{
public:
	clsPARTS_WINDOW_MODEL( clsResource* const pResource );
	~clsPARTS_WINDOW_MODEL();

	void Update( const short Type, const short Num );

	void Render(
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye );

private:

	//選択中パーツ.
	std::unique_ptr< clsASSEMBLE_MODEL > m_upSelectParts;

	clsResource* m_wpResource;

};

