

Texture2D		g_Texture	: register( t0 );
SamplerState	g_Sampler	: register( s0 );


cbuffer global
{
	matrix	g_W	: packoffset( c0 );	//ワールドから射影までの変換行列.
	float	g_ViewPortWidth	: packoffset( c4 );	//ビューポート( スクリーンサイズ ).
	float	g_ViewPortHeight: packoffset( c5 );	//ビューポート( スクリーンサイズ ).
	float	g_Alpha	: packoffset( c6 );
	float2	g_UV	: packoffset( c7 );
	float4	g_Color	: packoffset( c8 );
};


struct PS_IN
{
	float4	Pos	: SV_POSITION;
	float2	UV	: TEXCOORD;
};

PS_IN VS( float4 Pos : POSITION, float2 UV : TEXCOORD )
{
	PS_IN Out;

	Out.Pos = mul( Pos, g_W );

	Out.Pos.x = ( Out.Pos.x / g_ViewPortWidth ) * 2 - 1;
	Out.Pos.y = 1 - ( Out.Pos.y / g_ViewPortHeight ) * 2;

	Out.UV = UV;

	Out.UV.x += g_UV.x;
	Out.UV.y += g_UV.y;

	return Out;
}


float4 PS( PS_IN In ) : SV_Target
{
	float4 color = ( g_Color * g_Texture.Sample( g_Sampler, In.UV ) ) * g_Alpha;

	return color;
}


