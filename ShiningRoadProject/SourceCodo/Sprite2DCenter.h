#pragma once

#include "Sprite2D.h"

//基点が中心にあるので回転ができる.
class clsSPRITE2D_CENTER : public clsSprite2D
{
public:
	clsSPRITE2D_CENTER();
	~clsSPRITE2D_CENTER();

		//描画(レンダリング)
	void Render() override;

	void SetRot( const D3DXVECTOR3 &vRot );
	D3DXVECTOR3 GetRot();
	void AddRot( const D3DXVECTOR3 &vRot );


protected:

	//モデル作成.
	HRESULT InitModel( SPRITE_STATE ss ) override;


	//回転の数値の限界から飛び出ない.
	D3DXVECTOR3 GuardRotOver( D3DXVECTOR3 &vRot );
	float GuardRotOver( float &fRot );

	D3DXVECTOR3 m_vRot;//回転.
};

