#ifndef SPRITE_2D_H_
#define SPRITE_2D_H_

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )

#include "Common.h"//共通クラス.

#define ALIGN16	_declspec(align(16))


//======================================
//	構造体.
//======================================
//コンスタントバッファのアプリ側の定義(Sprite2D.hlsl)
//※シェーダ内のコンスタントバッファと一致している必要あり.
struct SPRITE2D_CONSTANT_BUFFER
{
	ALIGN16 D3DXMATRIX	mW;				//ワールド行列.
	ALIGN16 float		fViewPortWidth;	//ビューポート幅.
	ALIGN16 float		fViewPortHeight;//ビューポート高さ.
	ALIGN16	float		fAlpha;			//アルファ値(透過で使用する)
	ALIGN16 D3DXVECTOR2	vUV;			//UV座標.
};

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
		const SPRITE_STATE ss );


	//描画(レンダリング)
	virtual void Render();


	void SetPos( const D3DXVECTOR3 &vPos );
	void SetScale( const D3DXVECTOR3 &vScale );
	void SetScale( const float &fScale, 
		const bool withZ = false );	//XとYを等しい倍率で書ける.
									//第二引数をtrueにするとZにも影響する.		
	void SetAlpha( const float fAlpha );
	//範囲をoverするとfalseが返ってくる.
	bool AddAlpha( const float fAlpha );
	float GetAlpha();

	D3DXVECTOR3 GetPos() const;
	D3DXVECTOR3 GetScale() const;


	void AddPos( const D3DXVECTOR3 &vPos );		//引数を加算する.
	void AddScale( const D3DXVECTOR3 &vScale );	//引数倍にする.
	void AddScale( const float &fScale, const bool withZ = false );

	void SetAnim( const POINTFLOAT &anim );

protected:


	//シェーダ作成.
	HRESULT InitShader();
	//モデル作成.
	virtual HRESULT InitModel( SPRITE_STATE ss );
	//テクスチャ作成.
	HRESULT CreateTexture( const char* const fileName,
		ID3D11ShaderResourceView** pTex );

	void Release();


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

	float			m_fAlpha;	//アルファ値.
};



#endif//#ifndef SPRITE_2D_H_