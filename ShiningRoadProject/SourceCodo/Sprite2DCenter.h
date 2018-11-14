#ifndef SPRITE_2D_CENTER_H_
#define SPRITE_2D_CENTER_H_

#include "Sprite2D.h"

//基点が中心にあるので回転ができる.
class clsSPRITE2D_CENTER : public clsSprite2D
{
public:
	clsSPRITE2D_CENTER();
	~clsSPRITE2D_CENTER();

		//描画(レンダリング)
	void Render() override;

	D3DXVECTOR3 GetRot(){ return m_vRot; };
	void SetRot( const D3DXVECTOR3 &vRot ){
		m_vRot = vRot;
		GuardRotOver( m_vRot );
	};
	void AddRot( const D3DXVECTOR3 &vRot ){
		m_vRot += vRot;
		GuardRotOver( m_vRot );
	};


protected:

	//モデル作成.
	HRESULT InitModel( SPRITE_STATE ss ) override;


	//回転の数値の限界から飛び出ない.
	D3DXVECTOR3 GuardRotOver( D3DXVECTOR3 &vRot );
	float GuardRotOver( float &fRot );

	D3DXVECTOR3 m_vRot;//回転.
};

#endif//#ifndef SPRITE_2D_CENTER_H_