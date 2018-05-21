#include "Sprite2DCenter.h"

const float fROT_MAX = static_cast<float>( D3DX_PI * 2.0 );

clsSPRITE2D_CENTER::clsSPRITE2D_CENTER()
{
	m_vRot = { 0.0f, 0.0f, 0.0f };
}

clsSPRITE2D_CENTER::~clsSPRITE2D_CENTER()
{
}


//================================================
//	モデル作成.
//================================================
HRESULT clsSPRITE2D_CENTER::InitModel( SPRITE_STATE ss )
{
	float fW = ss.Disp.w * 0.5f;	//表示スプライト幅.
	float fH = ss.Disp.h * 0.5f;	//表示スプライト高さ.
	float fU = 1.0f/ ss.Anim.w;	//１コマ当たりの幅.
	float fV = 1.0f/ ss.Anim.h;	//１コマ当たりの高さ.

	//板ポリ(四角形)の頂点を作成.
	SpriteVertex vertices[] =
	{
		//頂点座標(x,y,z)				UV座標(u,v)
		D3DXVECTOR3(-fW,  fH, 0.0f ), D3DXVECTOR2(0.0f,   fV),//頂点1(左下).
		D3DXVECTOR3(-fW, -fH, 0.0f ), D3DXVECTOR2(0.0f, 0.0f),//頂点2(左上).
		D3DXVECTOR3( fW,  fH, 0.0f ), D3DXVECTOR2(  fU,   fV),//頂点3(右下).
		D3DXVECTOR3( fW, -fH, 0.0f ), D3DXVECTOR2(  fU, 0.0f)	//頂点4(右上).
	};
	//最大要素数を算出する.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//バッファ構造体.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;//使用方(デフォルト)
	bd.ByteWidth = sizeof(SpriteVertex) * uVerMax;//頂点サイズ(頂点×4)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//頂点バッファとして扱う.
	bd.CPUAccessFlags = 0;	//CPUからはアクセスしない.
	bd.MiscFlags = 0;	//その他のフラグ(未使用)
	bd.StructureByteStride = 0;//構造体のサイズ(未使用)

	//サブリソースデータ構造体.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;//板ポリの頂点をセット.

	//頂点バッファの作成.
	if (FAILED(
		m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer)))
	{
		MessageBox(NULL, "頂点バッファ作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}


	//頂点バッファをセット.
	UINT stride = sizeof(SpriteVertex);//データ間隔.
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer, &stride, &offset);


	//テクスチャ用のサンプラー構造体.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	//リニアフィルタ-(線形補間)
	//	POINT:高速だが粗い.
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//ラッピングモード.
	//	WRAP  :繰り返し
	//	MIRROR:反転繰り返し.
	//	CLAMP :端の模様を引き伸ばす.
	//	BORDER:別途境界色を決める.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//サンプラー作成.
	if (FAILED(
		m_pDevice11->CreateSamplerState(
		&SamDesc, &m_pSampleLinear)))//(out)サンプラー.
	{
		MessageBox(NULL, "サンプラ作成失敗", "エラー", MB_OK);
		return E_FAIL;
	}

	return S_OK;
}


//板ポリゴンを画面にレンダリング.
void clsSPRITE2D_CENTER::Render()
{
	//行列.
	D3DXMATRIX	mWorld, mTrans, mScale, mYaw, mPitch, mRoll;

	D3DXMatrixIdentity( &mWorld );	//初期化:単位行列作成.
	D3DXMatrixIdentity( &mScale );	//初期化:単位行列作成.

	//拡縮.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, m_vScale.z );

	//回転.
	D3DXMatrixRotationX( &mPitch,m_vRot.x );
	D3DXMatrixRotationY( &mYaw,	 m_vRot.y );
	D3DXMatrixRotationZ( &mRoll, m_vRot.z );

	//平行移動.
	D3DXMatrixTranslation(&mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z);

	//合成.
	mWorld = mScale * mYaw * mPitch * mRoll * mTrans;

	//使用するシェーダの登録.
	m_pDeviceContext11->VSSetShader(m_pVertexShader, NULL, 0);
	m_pDeviceContext11->PSSetShader(m_pPixelShader, NULL, 0);


	//シェーダのコンスタントバッファに各種データを渡す.
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITE2D_CONSTANT_BUFFER cd;	//コンスタントバッファ.
	//バッファ内のデータの書き方開始時にmap.
	if (SUCCEEDED(
		m_pDeviceContext11->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//ワールド行列を渡す.
		D3DXMATRIX m = mWorld;
		D3DXMatrixTranspose(&m, &m);	//行列を転置する.					
		//行列の計算方法がDirectXとGPUで異なる為.
		cd.mW = m;

		//ビューポートの幅、高さを渡す.
		cd.fViewPortWidth = WND_W;
		cd.fViewPortHeight= WND_H;

		//アルファ値を渡す.
		cd.fAlpha = m_fAlpha;

		//UV座標.
		//１コマ当たりの割合にコマ番号を掛けて位置を設定する.
		cd.vUV.x = ( 1.0f / m_SState.Anim.w ) * m_fPatternNo.x;
		cd.vUV.y = ( 1.0f / m_SState.Anim.h ) * m_fPatternNo.y;

		memcpy_s(pData.pData, pData.RowPitch,
			(void*)(&cd), sizeof(cd));

		m_pDeviceContext11->Unmap(m_pConstantBuffer, 0);
	}

	//このコンスタントバッファをどのシェーダで使うか？.
	m_pDeviceContext11->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);
	m_pDeviceContext11->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);

	//頂点バッファをセット.
	UINT stride = sizeof(SpriteVertex);	//データの間隔.
	UINT offset = 0;
	m_pDeviceContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);

	//頂点インプットレイアウトをセット.
	m_pDeviceContext11->IASetInputLayout(m_pVertexLayout);

	//プリミティブ・トポロジーをセット.
	m_pDeviceContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャをシェーダに渡す.
	m_pDeviceContext11->PSSetSamplers(
		0, 1, &m_pSampleLinear);	//サンプラ-をセット.
	m_pDeviceContext11->PSSetShaderResources(
		0, 1, &m_pTexture);		//テクスチャをシェーダに渡す.

	//アルファブレンド用ブレンドステート作成＆設定.
	SetBlend(true);

	//プリミティブをレンダリング.
	m_pDeviceContext11->Draw(4, 0);

	//アルファブレンドを無効にする.
	SetBlend(false);
}




void clsSPRITE2D_CENTER::SetRot( const D3DXVECTOR3 &vRot )
{
	m_vRot = vRot;
	GuardRotOver( m_vRot );
}

D3DXVECTOR3 clsSPRITE2D_CENTER::GetRot()
{
	return m_vRot;
}

void clsSPRITE2D_CENTER::AddRot( const D3DXVECTOR3 &vRot )
{
	m_vRot += vRot;
	GuardRotOver( m_vRot );
}
	void AddRot( const float &fRot );




//回転の数値の限界から飛び出ない.
D3DXVECTOR3 clsSPRITE2D_CENTER::GuardRotOver( D3DXVECTOR3 &vRot )
{
	GuardRotOver( vRot.x );
	GuardRotOver( vRot.y );
	GuardRotOver( vRot.z );

	return vRot;
}

float clsSPRITE2D_CENTER::GuardRotOver( float &fRot )
{
	//0より小さいなら一周大きくする.
	if( fRot < 0.0f ){
		fRot += fROT_MAX;
	}
	//一周より大きいなら一周小さくする.
	else if( fRot > fROT_MAX ){
		fRot -= fROT_MAX;
	}
	//範囲内なら何もしない.
	else{
		return fRot;
	}

	//もう一周してるかも知れない対策.
	GuardRotOver( fRot );

	return fRot;
}
