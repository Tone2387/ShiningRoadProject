//グローバル変数.
//テクスチャはレジスター t(n).
Texture2D		g_Texture	: register( t0 );
//サンプラーはレジスター s(n).
SamplerState	g_Sampler	: register( s0 );

//コンスタントバッファ.
cbuffer	global	: register( b0 )
{
	matrix	g_mW			: packoffset( c0 );
	float4	g_vColor		: packoffset( c4 );
	float	g_fViewPortW	: packoffset( c5 );
	float	g_fViewPortH	: packoffset( c6 );
	float2	g_vUV			: packoffset( c7 );
	float2	g_vNoiseStart	: packoffset( c9 );
	float2	g_vNoiseEnd		: packoffset( c11 );
};


//バーテックスシェーダの出力パラメータ.
struct VS_OUT
{
	float4	Pos	: SV_Position;
	float2	UV	: TEXCOORD;
};

//========== 関数 ==========//.
float random( float2 TexCoord )
{
	return frac( sin( dot( TexCoord.xy, float2( 12.9898, 78.233 ) ) ) * 43758.5453 );
}

float noise( float2 st )
{
	float2 p = floor( st );
	return random( p );
}
//========== 関数 ==========//.




//バーテックスシェーダ.
VS_OUT VS_Main( float4 Pos	: POSITION,
				float2 UV	: TEXCOORD )
{
	//初期化.
	VS_OUT	output = ( VS_OUT )0;

	output.Pos = mul( Pos, g_mW );

	//スクリーン座標に合わせる計算.
	output.Pos.x = ( output.Pos.x / g_fViewPortW ) * 2 - 1;
	output.Pos.y = 1 - ( output.Pos.y / g_fViewPortH ) * 2;

	//UV設定.
	output.UV = UV;

	//UV座標をずらす.
	output.UV.x += g_vUV.x;
	output.UV.y += g_vUV.y;

	return output;
}

//ピクセルシェーダ.

//通常.
float4 PS_Main( VS_OUT input )	: SV_Target
{
	float4 color =
		g_Texture.Sample( g_Sampler, input.UV );//色を返す.

	color *= g_vColor;

	return color;
}

//ノイズ.
float4 PS_Noise( VS_OUT input ) : SV_Target
{
	float4 color =
		g_Texture.Sample( g_Sampler, input.UV );//色を返す.

	//----- ノイズ -----//.
	//ブロック.
//	float c = noise( input.UV * 8 );
//	color *= float4( c, c, c, 1.0f );
	//パルス.
	float2 uv = input.UV;
	float x = 2*uv.y;
	uv.x += 5*sin( 10*x )*(-(x-1)*(x-1)+1);
//	float4 col = tex2D( g_Sampler, uv );
//	color = col;
	//----- ノイズ 終了 -----//.

	color *= g_vColor;

	return color;
}


