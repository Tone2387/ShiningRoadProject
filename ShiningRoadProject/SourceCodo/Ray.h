#ifndef RAY_H_
#define RAY_H_

#include "Common.h"	//共通クラス.

//レイクラス(表示のみを行う).
class clsRay : public clsCommon	//共通クラス継承.
{
public:
	//============================================================
	//	構造体.
	//============================================================
	//コンスタントバッファのアプリ側の定義(Ray.hlsl).
	//シェーダ内のコンスタントバッファと一致している必要あり.
	struct SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX	mWVP;		//ワールド,ビュー,射影の合成変換行列.
		D3DXVECTOR4	vColor;		//カラー(RGBAの型に合わせる) : テクスチャの上から色を載せる(赤っぽくも見せるためとか).
	};

	//頂点構造体.
	struct MODEL_VERTEX
	{
		D3DXVECTOR3 vPos;	//位置.
	};

	//レイ構造体.
	struct RAY
	{
		D3DXVECTOR3 vPoint[2];	//始点、終点.
		D3DXVECTOR3 vDirection;	//方向.
		D3DXVECTOR3 vPos;		//位置.
		FLOAT		fYaw;		//Y回転軸.
		RAY()
		{
			const D3DXVECTOR3 vINIT_VECTOR3 = { 0.0f, 0.0f, 0.0f };
			//ZeroMemory( this, sizeof( RAY ) );
			vPoint[0] = vPoint[1] = vINIT_VECTOR3;
			vDirection = vINIT_VECTOR3;
			vPos = vINIT_VECTOR3;
			fYaw = 0.0f;
		}
	};
	RAY	m_Ray;	//レイ構造体.

	clsRay();
	~clsRay();

	//初期化.
	HRESULT Init( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

	//描画.
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj ) const;


private:
	//シェーダ初期化.
	HRESULT InitShader();
	//レイ初期化.
	HRESULT InitModel();

	//↓モデルの種類ごとに用意.
	ID3D11VertexShader*		m_pVertexShader;	//頂点シェーダ.
	ID3D11InputLayout*		m_pVertexLayout;	//頂点レイアウト.
	ID3D11PixelShader*		m_pPixelShader;		//ピクセルシェーダ.
	ID3D11Buffer*			m_pConstantBuffer;	//コンスタントバッファ.

	//↓モデルごとに用意.
	ID3D11Buffer*			m_pVertexBuffer;	//頂点バッファ.


};




#endif//#ifndef RAY_H_