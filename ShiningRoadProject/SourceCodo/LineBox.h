#ifndef LINE_BOX_H_
#define LINE_BOX_H_


#include "Sprite2DCenter.h"

//線の四角.
class clsLINE_BOX
{
public:

	//先に変化する.
	enum class encBEFOR_CHANGE : UCHAR
	{
		NOTHING = 0,//終了したら自動でこれになる。自らが選んではいけない.

		WIDTH,
		HEIGHT,
		BOTH,	//両方同時に変わる.
	};

	clsLINE_BOX(		
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11 );

	virtual ~clsLINE_BOX();


	virtual void Update();

	virtual void Render();

	D3DXVECTOR3 GetPos() const				{ return m_vPos; };
	void SetPos( const D3DXVECTOR3 &vPos )	{ m_vPos = vPos; };
	void AddPos( const D3DXVECTOR3 &vPos )	{ m_vPos += vPos; };

	D3DXVECTOR3 GetSize() const				{ return m_vSize; };
	void SetSize( const D3DXVECTOR3 &vSize ){ m_vSize = vSize; };
	void SetSize( const float &fSize )		{ m_vSize = { fSize, fSize, 1.0f }; };
	void AddSize( const D3DXVECTOR3 &vSize ){ m_vSize += vSize; };

	void AddScale( const D3DXVECTOR3 &vScale ){
		m_vSize.x *= vScale.x;
		m_vSize.y *= vScale.y;
		m_vSize.z *= vScale.z;
	};
	void AddScale( const float &fScale ){
		m_vSize.x *= fScale;
		m_vSize.y *= fScale;
		m_vSize.z = 1.0f;
	};

	//じわじわ大きさを変えさせたいときに使う.
	void SetSizeTarget( const D3DXVECTOR3 &vSize );
	void AddChangeData( 
		const float fWidthSpd,
		const float fHeightSpd,
		const encBEFOR_CHANGE encBefor );

	//変化中はfalse.
	bool isStopChange(){ return m_isStopChange; };


	virtual void SetAlpha( const float fAlpha );
	//範囲をoverするとfalseが返ってくる.
	bool AddAlpha( const float fAlpha );

protected:

	virtual void SetColor( const D3DXVECTOR3& vColor ){
		for( char i=0; i<2; i++ ){
			m_upLineH[i]->SetColor( vColor );
			m_upLineV[i]->SetColor( vColor );
			m_upLineJoint[i]->SetColor( vColor );
			m_upLineJoint[ i + 2 ]->SetColor( vColor );
		}
	}

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vSize;//今の直径.

	float		m_fAlpha;	//アルファ値.

private:
	//箱を形作る.
	void SetUpBox();

	//角のscale.
	void SetUpJointSize();

	//Targetに向けてサイズを変える.
	void AddSizeForTarget();

	std::unique_ptr< clsSPRITE2D_CENTER > m_upLineH[2];//水平( 横線 )( 0が上 ).
	std::unique_ptr< clsSPRITE2D_CENTER > m_upLineV[2];//垂直( 縦線 )( 0が左 ).
	std::unique_ptr< clsSPRITE2D_CENTER > m_upLineJoint[4];//角( 左上から時計回り ).


	D3DXVECTOR3 m_vSizeTarget;
	D3DXVECTOR3 m_vChangeSpd;
	bool m_isChangeBigW;//trueなら大きく変わる.
	bool m_isChangeBigH;
	encBEFOR_CHANGE m_encBeforChange;

private:


	//変化が止まっているか.
	bool m_isStopChange;

};
#endif//#ifndef LINE_BOX_H_