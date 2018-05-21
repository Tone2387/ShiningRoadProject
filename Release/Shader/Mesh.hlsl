//グローバル変数.

//テクスチャはレジスタt(n).
Texture2D		g_texColor: register( t0 );
//サンプラーはレジスタs(n).
SamplerState	g_samLinear:register( s0 );



//コンスタントバッファ.
cbuffer global_0	: register(b0)
{
	matrix g_mW;		//ワールド行列.
	matrix g_mWVP;		//ワールド,ビュー,射影の合成行列.
	float4 g_vLightDir;	//ライトの方向ベクトル.
	float4 g_vEye;		//カメラ(視点).
	float4 g_vColor;	//カラー(透過色).
};
cbuffer global_1	: register(b1)
{
	float4 g_Ambient = float4( 0, 0, 0, 0 );	//環境光.
	float4 g_Diffuse = float4( 1, 0, 0, 0 );	//拡散反射(色).
	float4 g_Specular= float4( 1, 1, 1, 1 );	//鏡面反射.
};

//頂点シェーダの出力パラメータ.
struct VS_OUT
{
	float4 Pos		:	SV_Position;
	float4 Color	:	COLOR0;
	float3 Light	:	TEXCOORD0;
	float3 Normal	:	TEXCOORD1;
	float3 EyeVector:	TEXCOORD2;
	float2 Tex		:	TEXCOORD3;

	float4 PosWorld	:	TEXCOORD4;//グラウンドフォグ用.
};

//============================================================
//	頂点シェーダ.
//============================================================
VS_OUT VS_Main( float4 Pos	: POSITION,
				float4 Norm	: NORMAL,
				float2 Tex	: TEXCOORD )
{
	VS_OUT Out = (VS_OUT)0;

	Out.Pos = mul( Pos, g_mWVP );
	
	//法線をワールド空間に.
	Norm.w = 0;//w=0で移動成分を反映させない.
	float4 tmpFloat4 = mul( Norm, g_mW );
	Out.Normal.x = tmpFloat4.x;	
	Out.Normal.y = tmpFloat4.y;	
	Out.Normal.z = tmpFloat4.z;	
	//ライト方向.
	tmpFloat4	= g_vLightDir;
	Out.Light.x	= tmpFloat4.x;
	Out.Light.y	= tmpFloat4.y;
	Out.Light.z	= tmpFloat4.z;

	float3 PosWorld = mul( Pos, g_mW );
	Out.PosWorld = mul( Pos, g_mW );

	//.視線ベクトル.
	Out.EyeVector.x	= g_vEye.x - PosWorld.x;
	Out.EyeVector.y	= g_vEye.y - PosWorld.y;
	Out.EyeVector.z	= g_vEye.z - PosWorld.z;

	float3 Normal	= normalize( Out.Normal );
	float3 LightDir	= normalize( Out.Light );
	float3 ViewDir	= normalize( Out.EyeVector );
	float4 NL		= saturate( dot( Normal, LightDir ) );

	float3 Reflect	= normalize( 2 * NL * Normal - LightDir );
	float4 Specular	= pow( saturate( dot( Reflect, ViewDir ) ), 4 );

	//色.
	Out.Color	= g_Diffuse * NL + Specular * g_Specular;
	//テクスチャ座標.
	Out.Tex		= Tex;

	return Out;
}


//***********************************************************.
// ラスタライザ(イメージしやすいようにコメントを入れたよ).
//***********************************************************.


//============================================================
//	ピクセルシェーダ.
//============================================================
float4 PS_Main( VS_OUT In )	:	SV_Target
{
	float4 color
		= g_texColor.Sample( g_samLinear, In.Tex ) /2
		+ In.Color / 2.0f;
	color *= g_vColor;



	//----- フォグ処理 -----//.
	float fFogColor = 0.0f;
	//--- 奥フォグ ---.
	float d = In.Pos.z * In.Pos.w;	//距離.
	float density = 0.03f;		//密度//0.025f.
	float e = 2.71828;		//自然対数の底(ネイピア数).

	float f = pow( e, -d * density );	//フォグファクター.
	f *= 0.5f + 0.5f + 0.5f;
	f = saturate( f );
	float4 Fog = f * color + ( 1 - f ) * fFogColor;

	//--- 床フォグ ---.
	float4 vFogColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	In.PosWorld /= In.PosWorld.w;
	float fFloorHeight = 3.0f;
	float y = In.PosWorld.y + fFloorHeight;//絶対座標での高さ.
	float h = -1.0f;	//ここからFogを出す.

	//ごり押し.
	{
		float High = 0.0f;
		float Low = -6.0f;
		float Mid = ( High + Low ) / 2;
		y =  Mid - y;
		float ffa = ( y / 1.0f ) * ( y / 1.0f ) * ( y / 1.0f ) * ( y / 1.0f ) * ( y / 1.0f ) * ( y / 1.0f );
		vFogColor = float4( 0.0f, 0.0f, 0.0f, ffa ); 
	}

	f = y / h;	//フォグファクター.
	f = saturate( f );

	f *= 1.0f;	//フォグの量.
	f = saturate( f );
	float4 FogFloor = f * color + ( 1.0f - f ) * vFogColor;

	//----- フォグ処理 終了 -----//.

	color = ( FogFloor * Fog ) / color;

	return color;
}



//==========テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用==========
//==========テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用==========
//==========テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用==========
//==========テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用==========
//==========テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用テクスチャ無し用==========


//============================================================
//	頂点シェーダ.
//============================================================
VS_OUT VS_NoTex( float4 Pos	: POSITION,
				 float4 Norm: NORMAL )
{
	VS_OUT Out = ( VS_OUT )0;

	Out.Pos = mul( Pos, g_mWVP );

	//法線をワールド空間に.
	Norm.w = 0;//w=0で移動成分を反映させない.
	Out.Normal = mul( Norm, g_mW );
	//ライト方向.
	Out.Light.x = g_vLightDir.x;
	Out.Light.y = g_vLightDir.y;
	Out.Light.z = g_vLightDir.z;
	//.視線ベクトル.
	float3 PosWorld = mul( Pos, g_mW );
	Out.EyeVector.x = g_vEye.x - PosWorld.x;
	Out.EyeVector.y = g_vEye.y - PosWorld.y;
	Out.EyeVector.z = g_vEye.z - PosWorld.z;

	float3 Normal = normalize( Out.Normal );
	float3 LightDir = normalize( Out.Light );
	float3 ViewDir = normalize( Out.EyeVector );
	float4 NL = saturate( dot( Normal, LightDir ) );

	float3 Reflect = normalize( 2 * NL * Normal - LightDir );
	float4 Specular = pow( saturate( dot( Reflect, ViewDir ) ), 4 );

	//色.
	Out.Color = g_Diffuse * NL + Specular * g_Specular;

	return Out;
}


//============================================================
//	ピクセルシェーダ.
//============================================================
float4 PS_NoTex( VS_OUT In ): SV_Target
{
	float4 color = In.Color;
	return color * g_vColor;
}




//==========以下、板ポリゴン用コード以下、板ポリゴン用コード以下、板ポリゴン用コード以下、板ポリゴン用コード==========
//==========以下、板ポリゴン用コード以下、板ポリゴン用コード以下、板ポリゴン用コード以下、板ポリゴン用コード==========
//==========以下、板ポリゴン用コード以下、板ポリゴン用コード以下、板ポリゴン用コード以下、板ポリゴン用コード==========
//==========以下、板ポリゴン用コード以下、板ポリゴン用コード以下、板ポリゴン用コード以下、板ポリゴン用コード==========

//============================================================
//コンスタントバッファ.
//============================================================
cbuffer	Ita_Global	:	register(b2)	//レジスタを数分追加.
{
	matrix	g_WVP;		//ワールドから射影までの変換行列.
	float4	g_vcolor;	//色.
	float4	g_UV;		//UV座標.
};


//============================================================
//構造体.
//============================================================
struct VS_ItaOut
{
	float4	Pos	:	SV_Position;
	float2	Tex	:	TEXCOORD;
};

//============================================================
//	頂点シェーダ.
//============================================================
VS_ItaOut	VS_Ita( float4 Pos	:	POSITION,
					float4 Tex	:	TEXCOORD )
{
	VS_ItaOut output = (VS_ItaOut)0;
	//mul内積(A,B) AとBの掛け算.
	output.Pos = mul( Pos, g_WVP );
	output.Tex.x = Tex.x;
	output.Tex.y = Tex.y;

	//テクスチャのUV座標を操作する.
	output.Tex.x += g_UV.x;
	output.Tex.y += g_UV.y;

	return output;
}

//============================================================
//	ピクセルシェーダ.
//============================================================
float4 PS_Ita( VS_ItaOut input )	:	SV_Target
{
	float4 color = g_texColor.Sample( g_samLinear, input.Tex );
	return color * g_vcolor;//色を返す.
}


