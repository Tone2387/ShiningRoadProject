#ifndef SPRITE_H_
#define SPRITE_H_

//警告についてのコード分析を無効にする.4005:再定義.
#pragma warning( disable:4005 )	

//============================================================
//	インクルード.
//============================================================
#include "Common.h"//共通クラス.


//============================================================
//	構造体.
//============================================================
//コンスタントバッファのアプリ側の定義(Simple.hlsl).
//シェーダ内のコンスタントバッファと一致している必要あり.
struct SPRITESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX	mWVP;		//ワールド,ビュー,射影の合成変換行列.
	D3DXVECTOR4	vColor;		//カラー(RGBAの型に合わせる) : テクスチャの上から色を載せる(赤っぽくも見せるためとか).
	D3DXVECTOR4	vUV;		//UV座標.
};

////頂点の構造体.
//struct SpriteVertex
//{
//	D3DXVECTOR3 vPos;	//頂点座標(x,y,z).
//	D3DXVECTOR2 vTex;	//テクスチャ座標.
//};




//============================================================
//	スプライトクラス.
//============================================================
class clsSprite
	: public clsCommon
{
public:
	clsSprite();	//コンストラクタ.
	virtual ~clsSprite();	//デストラクタ.

	//初期化.
	HRESULT Create( ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11 );


	//描画(レンダリング)(※DX9MESH内とMain内で2つ存在するので注意).
	virtual void Render( const D3DXMATRIX& mView, const D3DXMATRIX& mProj,
				 const D3DXVECTOR3 &vEye );

	void SetScale( const float fScale ){
		m_fScale = fScale;
	}


	D3DXVECTOR3		m_vPos;	//位置.

	int		m_AnimCount;//UVスクロール.

	bool	m_bDispFlg;//表示フラグ.


protected:

	//シェーダ作成.
	HRESULT InitShader();
	////モデル作成.
	virtual HRESULT InitModel();

	//↓モデルの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;	//頂点シェーダ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;		//ピクセルシェーダ.
	ID3D11Buffer*			m_pConstantBuffer;	//コンスタントバッファ.

	//↓モデルごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;	//頂点バッファ.

	ID3D11ShaderResourceView*	m_pTexture;		//テクスチャ.
	ID3D11SamplerState*			m_pSampleLinear;//テクスチャのサンプラー:/テクスチャに各種フィルタをかける.



	float m_fScale;


};


#endif	//#ifndef SPRITE_H_
