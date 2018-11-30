
//�e�N�X�`���̓��W�X�^t(n).
Texture2D		g_texColor	: register( t0 );
//�}�X�N.
Texture2D		g_TexMask1	: register( t1 );

//�T���v���[�̓��W�X�^s(n).
SamplerState	g_samLinear	: register( s0 );



//�R���X�^���g�o�b�t�@.
cbuffer global_0	: register(b0)
{
	matrix g_mW;		//���[���h�s��.
	matrix g_mWVP;		//���[���h,�r���[,�ˉe�̍����s��.
	float4 g_vLightDir;	//���C�g�̕����x�N�g��.
	float4 g_vEye;		//�J����(���_).
	float4 g_vColor;	//�J���[(���ߐF).
};
cbuffer global_1	: register(b1)
{
	float4 g_Ambient = float4( 0, 0, 0, 0 );	//����.
	float4 g_Diffuse = float4( 1, 0, 0, 0 );	//�g�U����(�F).
	float4 g_Specular= float4( 1, 1, 1, 1 );	//���ʔ���.
};

//���_�V�F�[�_�̏o�̓p�����[�^.
struct VS_OUT
{
	float4 Pos		:	SV_Position;
	float4 Color	:	COLOR0;
	float3 Light	:	TEXCOORD0;
	float3 Normal	:	TEXCOORD1;
	float3 EyeVector:	TEXCOORD2;
	float2 Tex		:	TEXCOORD3;

	float4 PosWorld	:	TEXCOORD4;//�O���E���h�t�H�O�p.
};

//============================================================
//	���_�V�F�[�_.
//============================================================
VS_OUT VS_Main( float4 Pos	: POSITION,
				float4 Norm	: NORMAL,
				float2 Tex	: TEXCOORD )
{
	VS_OUT Out = (VS_OUT)0;

	Out.Pos = mul( Pos, g_mWVP );
	
	//�@�������[���h��Ԃ�.
	Norm.w = 0;//w=0�ňړ������𔽉f�����Ȃ�.
	float4 tmpFloat4 = mul( Norm, g_mW );
	Out.Normal.x = tmpFloat4.x;	
	Out.Normal.y = tmpFloat4.y;	
	Out.Normal.z = tmpFloat4.z;	
	//���C�g����.
	tmpFloat4	= g_vLightDir;
	Out.Light.x	= tmpFloat4.x;
	Out.Light.y	= tmpFloat4.y;
	Out.Light.z	= tmpFloat4.z;

	float3 PosWorld = mul( Pos, g_mW );
	Out.PosWorld = mul( Pos, g_mW );

	//.�����x�N�g��.
	Out.EyeVector.x	= g_vEye.x - PosWorld.x;
	Out.EyeVector.y	= g_vEye.y - PosWorld.y;
	Out.EyeVector.z	= g_vEye.z - PosWorld.z;

	float3 Normal	= normalize( Out.Normal );
	float3 LightDir	= normalize( Out.Light );
	float3 ViewDir	= normalize( Out.EyeVector );
	float4 NL		= saturate( dot( Normal, LightDir ) );

	float3 Reflect	= normalize( 2 * NL * Normal - LightDir );
	float4 Specular	= pow( saturate( dot( Reflect, ViewDir ) ), 4 );

	//�F.
	Out.Color	= g_Diffuse * NL + Specular * g_Specular;
	//�e�N�X�`�����W.
	Out.Tex		= Tex;

	return Out;
}


//***********************************************************.
// ���X�^���C�U(�C���[�W���₷���悤�ɃR�����g����ꂽ��).
//***********************************************************.


//============================================================
//	�s�N�Z���V�F�[�_.
//============================================================
float4 PS_Main( VS_OUT In )	:	SV_Target
{
	float4 color
		= g_texColor.Sample( g_samLinear, In.Tex ) / 2
		+ In.Color / 2.0f;


	//�}�X�N.
	float4 maskColor = g_TexMask1.Sample( g_samLinear, In.Tex );

	if( 
		maskColor.r >= 0.99f
//		&& maskColor.g >= 0.99f
//		&& maskColor.b >= 0.99f
		)
	{
		color *= g_vColor;
	}


//	//----- �t�H�O���� -----//.
//	float fFogColor = 0.0f;
//	//--- ���t�H�O ---.
//	float d = In.Pos.z * In.Pos.w;	//����.
//	float density = 0.03f;		//���x//0.025f.
//	float e = 2.71828;		//���R�ΐ��̒�(�l�C�s�A��).
//
//	float f = pow( e, -d * density );	//�t�H�O�t�@�N�^�[.
//	f *= 0.5f + 0.5f + 0.5f;
//	f = saturate( f );
//	float4 Fog = f * color + ( 1 - f ) * fFogColor;
//
//	//--- ���t�H�O ---.
//	float4 vFogColor = { 0.0f, 0.0f, 0.0f, 1.0f };
//	In.PosWorld /= In.PosWorld.w;
//	float fFloorHeight = 3.0f;
//	float y = In.PosWorld.y + fFloorHeight;//��΍��W�ł̍���.
//	float h = -1.0f;	//��������Fog���o��.
//
//	//���艟��.
//	{
//		float High = 0.0f;
//		float Low = -6.0f;
//		float Mid = ( High + Low ) / 2;
//		y =  Mid - y;
//		float ffa = ( y / 1.0f ) * ( y / 1.0f ) * ( y / 1.0f ) * ( y / 1.0f ) * ( y / 1.0f ) * ( y / 1.0f );
//		vFogColor = float4( 0.0f, 0.0f, 0.0f, ffa ); 
//	}
//
//	f = y / h;	//�t�H�O�t�@�N�^�[.
//	f = saturate( f );
//
//	f *= 1.0f;	//�t�H�O�̗�.
//	f = saturate( f );
//	float4 FogFloor = f * color + ( 1.0f - f ) * vFogColor;
//
//
//	color = ( FogFloor * Fog ) / color;
//	//----- �t�H�O���� �I�� -----//.

	return color;
}



//==========�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p==========
//==========�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p==========
//==========�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p==========
//==========�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p==========
//==========�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p�e�N�X�`�������p==========


//============================================================
//	���_�V�F�[�_.
//============================================================
VS_OUT VS_NoTex( float4 Pos	: POSITION,
				 float4 Norm: NORMAL )
{
	VS_OUT Out = ( VS_OUT )0;

	Out.Pos = mul( Pos, g_mWVP );

	//�@�������[���h��Ԃ�.
	Norm.w = 0;//w=0�ňړ������𔽉f�����Ȃ�.
	Out.Normal = mul( Norm, g_mW );
	//���C�g����.
	Out.Light.x = g_vLightDir.x;
	Out.Light.y = g_vLightDir.y;
	Out.Light.z = g_vLightDir.z;
	//.�����x�N�g��.
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

	//�F.
	Out.Color = g_Diffuse * NL + Specular * g_Specular;

	return Out;
}


//============================================================
//	�s�N�Z���V�F�[�_.
//============================================================
float4 PS_NoTex( VS_OUT In ): SV_Target
{
	float4 color = In.Color;
	return color * g_vColor;
}




//==========�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h==========
//==========�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h==========
//==========�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h==========
//==========�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h�ȉ��A�|���S���p�R�[�h==========

//============================================================
//�R���X�^���g�o�b�t�@.
//============================================================
cbuffer	Ita_Global	:	register(b2)	//���W�X�^�𐔕��ǉ�.
{
	matrix g_WVP;		//���[���h����ˉe�܂ł̕ϊ��s��.
	float4 g_vcolor;	//�F.
	float4 g_UV;		//UV���W.
	float4 g_Split;		//����.
};


//============================================================
//�\����.
//============================================================
struct VS_ItaOut
{
	float4	Pos	:	SV_Position;
	float2	Tex	:	TEXCOORD;
};

//============================================================
//	���_�V�F�[�_.
//============================================================
VS_ItaOut	VS_Ita( float4 Pos	:	POSITION,
					float4 Tex	:	TEXCOORD )
{
	VS_ItaOut output = (VS_ItaOut)0;
	//mul����(A,B) A��B�̊|���Z.
	output.Pos = mul( Pos, g_WVP );
	output.Tex.x = Tex.x;
	output.Tex.y = Tex.y;

	//�e�N�X�`����UV���W�𑀍삷��.
	output.Tex.x += g_UV.x;
	output.Tex.y += g_UV.y;

	return output;
}

//============================================================
//	�s�N�Z���V�F�[�_.
//============================================================
float4 PS_Ita( VS_ItaOut input )	:	SV_Target
{
	float2 TexPos = input.Tex;
	TexPos.x *= g_Split.x;
	TexPos.y *= g_Split.y;
	float4 color = g_texColor.Sample( g_samLinear, TexPos );
	return color * g_vcolor;//�F��Ԃ�.

//	float4 color = g_texColor.Sample( g_samLinear, input.Tex );
//	return color * g_vcolor;//�F��Ԃ�.
}


