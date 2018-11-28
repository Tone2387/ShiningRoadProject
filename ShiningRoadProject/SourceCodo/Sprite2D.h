#ifndef SPRITE_2D_H_
#define SPRITE_2D_H_

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include "Common.h"//共通クラス.



//======================================
//	構造体.
//======================================

//スプライト構造体.
struct SPRITE_STATE
{
	WHSIZE_FLOAT	Disp;	//表示幅高さ.
	WHSIZE_FLOAT	Anim;	//何コマ?.
	SPRITE_STATE(){
		Disp = { WND_W, WND_H };
		Anim = { 1.0f, 1.0f };
	}
};


/*************************************************
*	Sprite2Dクラス.
**/
class clsSprite2D : public clsCommon
{

public:
	clsSprite2D();	//コンストラクタ.
	virtual ~clsSprite2D();	//デストラクタ.

	//初期化.
	HRESULT Create( 
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11,
		const char* fileName, 
		const SPRITE_STATE& ss );


	//描画(レンダリング)
	virtual void Render();


	D3DXVECTOR3 GetPos()					{ return m_vPos; };
	void SetPos( const D3DXVECTOR3& vPos )	{ m_vPos = vPos; };
	void AddPos( const D3DXVECTOR3& vPos )	{ m_vPos += vPos; };


	void SetScale( const D3DXVECTOR3 &vScale ) { m_vScale = vScale; };
	void SetScale( const float &fScale, 
		const bool withZ = false );	//XとYを等しい倍率で書ける.

	D3DXVECTOR3 GetScale() const { return m_vScale; }

	void AddScale( const D3DXVECTOR3 &vScale );	//引数倍にする.
	void AddScale( const float &fScale, const bool withZ = false );


	void SetColor( const D3DXVECTOR3& vColor );
	D3DXVECTOR3 GetColor();

	float GetAlpha()					{ return m_vColor.w; };
	//第二引数をtrueにするとZにも影響する.		
	void SetAlpha( const float fAlpha ) { m_vColor.w = fAlpha; }
	//範囲をoverするとfalseが返ってくる.
	bool AddAlpha( const float fAlpha );




	void SetAnim( const POINTFLOAT& anim );

protected:


	//シェーダ作成.
	HRESULT InitShader();
	//モデル作成.
	virtual HRESULT InitModel( const SPRITE_STATE& ss );
	//テクスチャ作成.
	HRESULT CreateTexture( const char* const fileName,
		ID3D11ShaderResourceView** pTex );

protected:

	//ヘッダーにいるのは継承先の為.
	struct SPRITE2D_CONSTANT_BUFFER
	{
		ALIGN16 D3DXMATRIX	mW;				//ワールド行列.
		ALIGN16	D3DXVECTOR4	vColor;			//アルファ値(透過で使用する)
		ALIGN16 float		fViewPortWidth;	//ビューポート幅.
		ALIGN16 float		fViewPortHeight;//ビューポート高さ.
		ALIGN16 D3DXVECTOR2	vUV;			//UV座標.
	};


	//↓モデルの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;//頂点シェーダ.
	ID3D11InputLayout*		m_pVertexLayout;//頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;	//ピクセルシェーダ.
	ID3D11Buffer*			m_pConstantBuffer;//コンスタントバッファ.

	//↓モデルごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;//頂点バッファ.

	ID3D11ShaderResourceView*	m_pTexture;//テクスチャ.
	ID3D11SamplerState*			m_pSampleLinear;//テクスチャのサンプラ-:テクスチャーに各種フィルタをかける.


	SPRITE_STATE	m_SState;	//スプライト情報.
	POINTFLOAT		m_fPatternNo;//マス目番号.		

	D3DXVECTOR3		m_vPos;	//位置.

	D3DXVECTOR3		m_vScale;

	D3DXVECTOR4		m_vColor;
};



#endif//#ifndef SPRITE_2D_H_