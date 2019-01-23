//#define CAN_CHECK_STRING_BYTE_TYPE//�����̉p�����A���{�ꕶ����1�o�C�g�ځA2�o�C�g�ڂ̔��肪�ł���Ȃ����.

#include "CFont.h"

#include "BlendState.h"

#include "File.h"
//#include "OperationString.h"




using namespace std;

namespace{

//#define sFONT_NAME "�������疾�� Medium"
#define sFONT_NAME "�}�L�i�X Scrap 5"

#define FONT_SHADER		"Shader\\FontText.hlsl"

//�A���C�������g�ݒ�(�����I��16�o�C�g�ɐݒ肷��).
#define ALIGN16 _declspec( align ( 16 ) )

	//�\����.
	struct FONTSHADER_CONSTANT_BUFFER
	{
		ALIGN16 D3DXMATRIX mW;			//�ʒu�ƃJ�����ʒu�ƕ\����ʐݒ�.
		ALIGN16 float ViewPortWidth;	//�J���[(RGBA�̌^�ɍ��킹��).
		ALIGN16 float ViewPortHeight;	//UV���W.
		ALIGN16 float Alpha;	//���ߒl.
		ALIGN16 D3DXVECTOR2 Uv;	//UV���W.
		ALIGN16 D3DXVECTOR4 Color;	//UV���W.
	};


	//�t�H���g�̃p�X.
	const char* sFONT_STYLE_PATH = "Data\\Font\\FontType\\Makinas-Scrap-5.otf";//Makinas-Scrap-5.otf.//aozora.ttf.


	const int iFULL_MESSAGE_NUMBER = -1;//�f�t�H���g����.


	const int iFONT_SIZE = 32;
	const float fFONT_MARGIN_X = 2.0f;//4.0f.
	const float fFONT_MARGIN_Y = 8.0f;//4.0f.

	//Create���Ă��Ȃ��Ƃ��̍s��.
	const int iERROR_TEXT_ROW_NUM = -1;

}

clsFont::clsFont( 
	ID3D11Device* const pDevice, 
	ID3D11DeviceContext* const pContext )
	:m_pVertexShader( nullptr )
	,m_pVertexLayout( nullptr )
	,m_pPixelShader( nullptr )
	,m_pVertexBuffer( nullptr )
	,m_pSampleLinear( nullptr )
	,m_pConstantBuffer( nullptr )
	,m_vecpTex2D()
	,m_vecvecpAsciiTexture()
	,m_vecsTextData()
	,m_Design()
	,m_fIndentionPosint( static_cast<float>( WND_W ) )
	,m_iFontSize( iFONT_SIZE )
	,m_fFontMarginX( fFONT_MARGIN_X )
	,m_fFontMarginY( fFONT_MARGIN_Y )
	,m_vPos( { 0.0f, 0.0f, 0.0f } )
	,m_fScale( static_cast<float>( m_iFontSize ) )
	,m_fAlpha( 1.0f )
	,m_vColor( { 1.0f, 1.0f, 1.0f, 1.0f } )
	,m_vPosLast( m_vPos )
	,m_iTextRow( iERROR_TEXT_ROW_NUM )
{
	m_wpDevice = pDevice;
	m_wpContext = pContext;

//	for( unsigned char i=0; i<enBLEND_STATE_size; i++ ){
//		m_pBlendState[i] = nullptr;
//	}

	int iReturn = AddFontResourceEx(
		sFONT_STYLE_PATH,
		FR_PRIVATE,
		&m_Design );

	assert( iReturn );


	if( FAILED( CreateShader() ) ){
		assert( !"Can't Create Shader" );
	}
	if( FAILED( CreateVertexBuffer() ) ){
		assert( !"Can't Create VertexBuffer" );
	}
	if( FAILED( CreateConstantBuffer() ) ){
		assert( !"Can't Create ConstantBuffer" );
	}

}

clsFont::~clsFont()
{
	Release();

	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pSampleLinear );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pPixelShader ) ;
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );


	//���\�[�X�폜.
	BOOL Return = RemoveFontResourceEx(
		sFONT_STYLE_PATH,
		FR_PRIVATE,
		&m_Design );

	if( !Return ){
		ERR_MSG( "�t�H���g���\�[�X�̔j���Ɏ��s", "" );
	}

}

//�V�F�[�_�̍쐬.
HRESULT clsFont::CreateShader()
{
	ID3DBlob *pCompileShader = NULL;
	ID3DBlob *pErrors = NULL;

	char strDir[512];
	GetCurrentDirectory( sizeof( strDir ), strDir );


	//�u���u����o�[�e�b�N�X�V�F�[�_�[�쐬.
	if( FAILED( D3DX11CompileFromFile(
		FONT_SHADER, NULL, NULL, "VS", "vs_5_0", 0, 0, NULL,
		&pCompileShader, &pErrors, NULL ) ) )
	{
		assert( "CFont�u���u�쐬���s(VS)" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//Vertex Shader�̍쐬.
	if( FAILED( m_wpDevice->CreateVertexShader(
		(pCompileShader)->GetBufferPointer(),
		(pCompileShader)->GetBufferSize(),
		NULL, &m_pVertexShader ) ) )	//(out)���_�V�F�[�_�[.
	{
		assert( "CFontVertex Shader�쐬���s(VS)" );
		return E_FAIL;
	}

	//���_�C���v�b�g���C�A�E�g���`.
	D3D11_INPUT_ELEMENT_DESC layout_Ita[] =
	{
		{ 
			"POSITION",							//�ʒu.
			0, DXGI_FORMAT_R32G32B32_FLOAT,		//DXGI�̃t�H�[�}�b�g32bitfloat�^.
			0,
			0,									//�f�[�^�̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0 
		},
		{ 
			"TEXCOORD",							//�e�N�X�`���ʒu.
			0, DXGI_FORMAT_R32G32_FLOAT,
			0,
			12,									//�e�N�X�`���f�[�^�̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0 
		},
	};
	UINT numElements_Ita = sizeof( layout_Ita ) / sizeof( layout_Ita[0] );

	//���_�C���v�b�g���C�A�E�g�쐬.
	if( FAILED( m_wpDevice->CreateInputLayout( layout_Ita,
		numElements_Ita, pCompileShader->GetBufferPointer(),	//(out)���_�C���v�b�g���C�A�E�g.
		pCompileShader->GetBufferSize(), &m_pVertexLayout ) ) )
	{
		assert( "CFont���_�C���v�b�g���C�A�E�g�̍쐬���s" );
		return FALSE;
	}

	//�u���u����s�N�Z���V�F�[�_�[�쐬.
	if( FAILED( D3DX11CompileFromFile(
		FONT_SHADER, NULL, NULL, "PS", "ps_5_0", 0, 0, NULL,
		&pCompileShader, &pErrors, NULL ) ) )
	{
		assert( "CFont�u���u�쐬���s(PS)" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//Pixel Shader�̍쐬.
	if( FAILED( m_wpDevice->CreatePixelShader(
		(pCompileShader)->GetBufferPointer(),
		(pCompileShader)->GetBufferSize(), NULL, &m_pPixelShader ) ) )	//(out)���_.
	{
		assert( "CFont�s�N�Z���V�F�[�_�[�쐬���s" );
		return E_FAIL;
	}

	SAFE_RELEASE( pCompileShader );	//�u���u���.



	return S_OK;
}
//�o�[�e�b�N�X�o�b�t�@�̍쐬.
HRESULT clsFont::CreateVertexBuffer()
{
	// Create VB.
	//�o�[�e�b�N�X�o�b�t�@�[�쐬.
	FONT_VERTEX vertices[4];

	FONT_VERTEX vertices2[] ={
		//���_���W�i���A���A���j�@�@�@�@UV���W�iu,v�j.
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f ), D3DXVECTOR2( 0.0f, 1.0f ),	//���_�P.
		D3DXVECTOR3( 0.0f,-1.0f, 0.0f ), D3DXVECTOR2( 0.0f, 0.0f ),	//���_�Q.
		D3DXVECTOR3( 1.0f, 1.0f, 0.0f ), D3DXVECTOR2( 1.0f, 1.0f ),	//���_�R.
		D3DXVECTOR3( 1.0f,-1.0f, 0.0f ), D3DXVECTOR2( 1.0f, 0.0f ),	//���_�S.
	};
	vertices[0] = vertices2[0];
	vertices[1] = vertices2[1];
	vertices[2] = vertices2[2];
	vertices[3] = vertices2[3];


	D3D11_BUFFER_DESC bd;					//�o�b�t�@�̍\����.
	bd.Usage = D3D11_USAGE_DEFAULT;			//�g�p���@.
	bd.ByteWidth = sizeof( FONT_VERTEX ) * 4;//���_�̃T�C�Y�i���_*�R�j.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags = 0;					//CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags = 0;						//���̑��̐ݒ�Ȃ�.

	//�T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitDate;
	InitDate.pSysMem = vertices;		//�O�p�`�̒��_�����Z�b�g.
	//���_�o�b�t�@�̍쐬.
	if( FAILED( m_wpDevice->CreateBuffer( &bd, &InitDate, &m_pVertexBuffer ) ) ){
		ERR_MSG("���_�o�b�t�@(m_pItaVB)�̍쐬�Ɏ��s", "CFont");
		return E_FAIL;
	}

	//���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof(FONT_VERTEX);	//���_�Ԃ̃T�C�Y.
	UINT offset = 0;					//�I�t�Z�b�g�l.
	m_wpContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

	//�e�N�X�`���p�T���v���[�쐬.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory( &SamDesc, sizeof( D3D11_SAMPLER_DESC ) );
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;	//���j�A�t�B���^�[�i���`��ԁj.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;		//���b�s���O���[�h�iWRAP�F�J��Ԃ�).
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	m_wpDevice->CreateSamplerState( &SamDesc, &m_pSampleLinear );	//�iout)�T���v���[.

	return S_OK;
}
//�萔�o�b�t�@�̍쐬.
HRESULT clsFont::CreateConstantBuffer()
{
	//�R���X�^���g�o�b�t�@�쐬(�|���p).
	D3D11_BUFFER_DESC ItaBD;
	ItaBD.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�R���X�^���X�g�o�b�t�@���w��.
	ItaBD.ByteWidth = sizeof( FONTSHADER_CONSTANT_BUFFER );	//�R���X�^���X�g�o�b�t�@�̃T�C�Y.
	ItaBD.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			//�������݂ŃA�N�Z�X.
	ItaBD.MiscFlags = 0;
	ItaBD.StructureByteStride = 0;
	ItaBD.Usage = D3D11_USAGE_DYNAMIC;					//�g�p�@�F���ڏ�������.

	//�R���X�^���X�g�o�b�t�@�쐬.
	if( FAILED( m_wpDevice->CreateBuffer(
		&ItaBD, NULL, &m_pConstantBuffer ) ) )
	{
		ERR_MSG("�R���X�^���g�o�b�t�@(Ita)�̍쐬�Ɏ��s", "CFont");
		return E_FAIL;
	}


	return S_OK;
}




void clsFont::Create( const char *sTextFileName )
{
	//���ɍ쐬���ꂽ���̂�����Ȃ�j�����Ă�����.
	if( m_iTextRow != iERROR_TEXT_ROW_NUM ){
		Release();
		assert( !"���g������clsFont�����������悤�Ƃ�����?\n\
				 �܂������Ƃ��Ă���������" );
	}

	if( FAILED( LoadTextFile( sTextFileName ) ) ){
		assert( !"Can't Load Text File" );
	}

	if( FAILED( CreateTexture( sTextFileName ) ) ){
		assert( !"Can't Create Texture" );
	}

	SetPos( { 0.0f, 0.0f, 0.0f } );
	SetScale( 0.0f );
	SetAlpha( 1.0f );
	SetColor( { 1.0f, 1.0f, 1.0f, 1.0f } );
}
//�t�@�C���ǂݍ���.
HRESULT clsFont::LoadTextFile( const char *sTextFileName )
{
	//�t�@�C���|�C���^.
	clsFILE File;

	if( !File.Open( sTextFileName ) ){
		return E_FAIL;
	}

	//������.
	//�s��.
	m_iTextRow = static_cast<int>( File.GetSizeRow() );
	m_vecsTextData.resize( m_iTextRow );
	m_vecpTex2D.resize( m_iTextRow, nullptr );
	m_vecvecpAsciiTexture.resize( m_iTextRow );

	for( unsigned int i=0; i<File.GetSizeRow(); i++ )
	{
		const int iCol = File.GetDataString( i, 0 ).size();//������̒���.
		//��s���R�s�[.
		m_vecsTextData[i] = File.GetDataString( i, 0 );
#ifndef CAN_CHECK_STRING_BYTE_TYPE
		m_vecvecpAsciiTexture[i].resize( iCol, nullptr );
#else//#ifndef CAN_CHECK_STRING_BYTE_TYPE
		m_vecvecpAsciiTexture[i].reserve( m_vecsTextData[i].size() );

		const int iNullPlus = 1;//�k�������̕�.
		for( unsigned int j=0; j<m_vecsTextData[i].size()-iNullPlus; j++ )
		{
			if( IsDBCSLeadByte( m_vecsTextData[i][j] ) ){
				m_vecvecpAsciiTexture[i].push_back( nullptr );
			}
		}
		m_vecvecpAsciiTexture[i].shrink_to_fit();
#endif//#ifndef CAN_CHECK_STRING_BYTE_TYPE
	}

//	if( IsDBCSLeadByte( m_vecsTextData[iTex][i] ) )


	//�t�@�C���̃N���[�Y.
	File.Close();

	return S_OK;
}
//�e�N�X�`���쐬.
HRESULT clsFont::CreateTexture( const char* sErrFilePath )
{
	//fontCreate.
	LOGFONT lf = {
		m_iFontSize, 0, 0, 0, 
		0, 0, 0, 0,
		//DEFAULT_CHARSET,
		SHIFTJIS_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FIXED_PITCH | FF_ROMAN,
		TEXT( sFONT_NAME )
	};
	

	//�s�����J��Ԃ�.
	for( unsigned int iTex=0; iTex<m_vecsTextData.size(); iTex++ )
	{
		HFONT hFont = CreateFontIndirect( &lf );
		if( !hFont ){
			ERR_MSG( "�t�H���g�쐬�s��", sErrFilePath );
			return E_FAIL;
		}

		HDC hdc = GetDC( NULL );

		HFONT oldFont = (HFONT)SelectObject( hdc, hFont );

		int iByteMax = strlen( m_vecsTextData[ iTex ].c_str() );
		
//		for( int i=0; i<iByteMax; i++ )
//		{
//			if( IsDBCSLeadByte( m_vecsTextData[ iTex ][i] ) ){
//				i++;
//			}
//		}

		UINT code;
		int iCharCnt = 0;//������J�E���g.
		for( int i=0; i<iByteMax; i++ ){
			//�����R�[�h�擾.
			if( IsDBCSLeadByte( m_vecsTextData[ iTex ][i] ) ){
				code = (BYTE)m_vecsTextData[ iTex ][i] << 8 | (BYTE)m_vecsTextData[ iTex ][ i + 1 ];
			}
			else{
				code = m_vecsTextData[ iTex ][i];
			}
			//�t�H���g�r�b�g�}�b�v�擾.
			TEXTMETRIC TM;
			GetTextMetrics( hdc, &TM );
			GLYPHMETRICS GM;
			const int grad = GGO_GRAY4_BITMAP;

			CONST MAT2 Mat = { { 0, 1 }, { 0, 0 }, { 0, 0 }, { 0, 1 } };
			DWORD size = GetGlyphOutline( hdc, code, grad, &GM, 0, NULL, &Mat );
			BYTE* ptr = new BYTE[ size ];
			GetGlyphOutline( hdc, code, grad, &GM, size, ptr, &Mat );

			//----- �������݉\�e�N�X�`���쐬 -----//.

			//CPU�ŏ������݂��ł���e�N�X�`���쐬.
			//�e�N�X�`���쐬.
			D3D11_TEXTURE2D_DESC desc;
			memset( &desc, 0, sizeof( desc ) );
			desc.Width = GM.gmCellIncX;
	//		desc.Width = ( GM.gmBlackBoxX + 3 ) / 4 * 4;
			desc.Height = TM.tmHeight;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

//			if( m_vecsTextData[ iTex ][i] ){
//				GM.
//			}
			

			if( FAILED( m_wpDevice->CreateTexture2D( &desc, 0, &m_vecpTex2D[ iTex ] ) ) ){
				MessageBox( 0, "�e�N�X�`���쐬���s", sErrFilePath, MB_OK );
				return E_FAIL;
			}

			D3D11_MAPPED_SUBRESOURCE hMappedResource;
			if( FAILED( m_wpContext->Map(
				m_vecpTex2D[ iTex ],
				0,
				D3D11_MAP_WRITE_DISCARD,
				0,
				&hMappedResource ) ) )
			{
				MessageBox(NULL, "�e�N�X�`���쐬���s",
					sErrFilePath, MB_OK);
				return E_FAIL;
			}
			// �����ŏ�������.
			BYTE* pBits = (BYTE*)hMappedResource.pData;

			// �t�H���g���̏�������.
			// iOfs_x, iOfs_y : �����o���ʒu(����).
			// iBmp_w, iBmp_h : �t�H���g�r�b�g�}�b�v�̕���.
			// Level : ���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K).
			int iOfs_x = GM.gmptGlyphOrigin.x;
			int iOfs_y = TM.tmAscent - GM.gmptGlyphOrigin.y;
			int iBmp_w = GM.gmBlackBoxX + ( 4 - ( GM.gmBlackBoxX % 4 ) ) % 4;
			int iBmp_h = GM.gmBlackBoxY;
			int Level = 17;
			int x, y;

			DWORD Alpha, Color;
			memset( pBits, 0, hMappedResource.RowPitch * TM.tmHeight );
			for( y=iOfs_y; y<iOfs_y+iBmp_h; y++ )
			{
				for( x=iOfs_x; x<iOfs_x+iBmp_w; x++ )
				{
					Alpha = ( 255 * ptr[ x - iOfs_x + iBmp_w * ( y - iOfs_y ) ] ) / ( Level - 1 );
					Color = 0x00ffffff | ( Alpha << 24 );

					memcpy( (BYTE*)pBits + hMappedResource.RowPitch * y + 4 * x, &Color, sizeof( DWORD ) );
				}
			}

			m_wpContext->Unmap( m_vecpTex2D[ iTex ], 0 );

			//�e�N�X�`�������擾����.
			D3D11_TEXTURE2D_DESC texDesc;
			m_vecpTex2D[ iTex ]->GetDesc( &texDesc );

			//ShaderResourceView�̏����쐬����.
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory( &srvDesc, sizeof( srvDesc ) );
			srvDesc.Format = texDesc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip = 0;
			srvDesc.Texture2D.MipLevels = texDesc.MipLevels;

			if( FAILED( m_wpDevice->CreateShaderResourceView(
				m_vecpTex2D[ iTex ], &srvDesc, &m_vecvecpAsciiTexture[ iTex ][ iCharCnt ] ) ) )
			{
				ERR_MSG( "�e�N�X�`���쐬���s", sErrFilePath );
				return E_FAIL;
			}

			//�����R�[�h�擾.
			if( IsDBCSLeadByte( m_vecsTextData[ iTex ][i] ) ){
				i++;
				iCharCnt++;//����.//��s�o�C�g�̔��f�����܂������Ȃ�����.
			}
			iCharCnt++;//����.
			delete[] ptr;
			ptr = nullptr;
		}
		//�f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̉��.
		SelectObject( hdc, oldFont );
		DeleteObject( hFont );
		ReleaseDC( NULL, hdc );
	}
	return S_OK;
}


void clsFont::Release()
{
	m_vecsTextData.clear();
	m_vecsTextData.shrink_to_fit();

	for( unsigned int iTex=0; iTex<m_vecvecpAsciiTexture.size(); iTex++ )
	{
		for( unsigned int i=0; i<m_vecvecpAsciiTexture[ iTex ].size(); i++ )
		{
			if( !m_vecvecpAsciiTexture[ iTex ][i] ) continue;
			m_vecvecpAsciiTexture[ iTex ][i]->Release();
			m_vecvecpAsciiTexture[ iTex ][i] = nullptr;
		}
		m_vecvecpAsciiTexture[ iTex ].clear();
		m_vecvecpAsciiTexture[ iTex ].shrink_to_fit();
	}
	m_vecvecpAsciiTexture.clear();
	m_vecvecpAsciiTexture.shrink_to_fit();

	for( unsigned int iTex=0; iTex<m_vecpTex2D.size(); iTex++ ){
		if( !m_vecpTex2D[ iTex ] ) continue;
		m_vecpTex2D[ iTex ]->Release();
		m_vecpTex2D[ iTex ] = nullptr;
	}
	m_vecpTex2D.clear();
	m_vecpTex2D.shrink_to_fit();

	m_iTextRow = iERROR_TEXT_ROW_NUM;
}
//							���i( ���s��? )	�@		��������.
void clsFont::Render( const int iTextRow, const int iCharNum )
{
	if( iTextRow <= -1 ) return;
	if( iTextRow >= static_cast<int>( m_vecvecpAsciiTexture.size() ) ) return;


	//�g�p����V�F�[�_�[�̓o�^.
	m_wpContext->VSSetShader( m_pVertexShader,	NULL, 0 );
	m_wpContext->PSSetShader( m_pPixelShader,	NULL, 0 );
	m_wpContext->GSSetShader( NULL,				NULL, 0 );

	//������̍�������W�̈ʒu�Ɏ����Ă��邽�߂ɕK�v.
	const D3DXVECTOR3 vOFFSET_POS = { -m_fScale, m_fScale * 0.5f, 0.0f };
	D3DXVECTOR3 vPos = m_vPos + vOFFSET_POS;

	int iRow = 0;//�s.
	int iCnt = 0;//���̍s�̉������ڂ�.
	float fAddLeft = 0.0f;//�ׂ���������ꂽ�ۂ̍��Ɋ񂹂��.

	const int iNUM_MAX = static_cast<int>( m_vecvecpAsciiTexture[ iTextRow ].size() );
	for( int i=0; i<iNUM_MAX; i++ )
	{
		//�f�t�H���g����.
		if( iCharNum == iFULL_MESSAGE_NUMBER ){}
		//�ł͂Ȃ��ꍇ�̔���.
		else if( i >= iCharNum ){
			break;
		}
#if 1
		if( vPos.x + m_iFontSize + ( m_fScale + m_fFontMarginX ) * iCnt + ( m_fScale + m_fFontMarginX ) * fAddLeft <= 
			m_fIndentionPosint )
		{
			//�w��͈͂̒�.
			iCnt++;
		}
		else{
			//�͈͎w��O�A��i���炷.
			iRow++;
			iCnt = 1;
			fAddLeft = 0.0f;
		}
#else
		//����ɉ��s���Ȃ�.
		iCnt++;
#endif
		//���[���h�ϊ�.
		D3DXMATRIX mWorld, mScale, mTran;		//���[���h�s��.
		D3DXMatrixIdentity( &mWorld );
		D3DXMatrixTranslation( &mTran, 
			vPos.x + ( m_fScale + m_fFontMarginX ) * static_cast<float>( iCnt ) + ( m_fScale + m_fFontMarginX ) * fAddLeft,
			vPos.y + ( m_fScale + m_fFontMarginY ) * static_cast<float>( iRow ),
			vPos.z );


		float fScaleW = 1.0f;
			//�����̑傫����l�߂��߂�.iTextRow, const int iCharNum
		float fTmpAddLeft = 0.0f;
		fScaleW = GetFineCharactorRate( iTextRow, i, &fTmpAddLeft );
		fAddLeft -= fTmpAddLeft;


		D3DXMatrixScaling( &mScale, m_fScale * fScaleW, m_fScale, 1.0f );
		mWorld = mScale * mTran;

		//�V�F�[�_�[�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
		D3D11_MAPPED_SUBRESOURCE	pData;
		FONTSHADER_CONSTANT_BUFFER	cb;
		if( SUCCEEDED( m_wpContext->Map(
			m_pConstantBuffer, 0,
			D3D11_MAP_WRITE_DISCARD,
			0, &pData ) ) )
		{
			//���[���h�s���n��.
			cb.mW = mWorld;
			D3DXMatrixTranspose( &cb.mW, &cb.mW );
			cb.ViewPortWidth = WND_W;
			cb.ViewPortHeight = WND_H;


			cb.Uv = { 0.0f, 0.0f };

			cb.Color = m_vColor;
			//�����x��n��.
			cb.Alpha = m_fAlpha;

			memcpy_s( pData.pData, pData.RowPitch, (void*)( &cb ), sizeof( cb ) );
			m_wpContext->Unmap( m_pConstantBuffer, 0 );
		}

		//�Ō�̕����̈ʒu���擾.
		m_vPosLast = D3DXVECTOR3(
			vPos.x + ( m_fScale + m_fFontMarginX ) * static_cast<float>( iCnt )	  + static_cast<float>( m_iFontSize * 2 ),
			vPos.y + ( m_fScale + m_fFontMarginY ) * static_cast<float>( iRow * 2 ) - static_cast<float>( m_iFontSize ),
			0.0f );

		//���̃R���X�^���g�o�b�t�@���g���V�F�[�_�[�̓o�^.
		m_wpContext->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );
		m_wpContext->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );

		//�o�[�e�b�N�X�o�b�t�@���Z�b�g.
		UINT stride = sizeof( FONT_VERTEX );
		UINT offset = 0;
		m_wpContext->IASetVertexBuffers( 0, 1, &m_pVertexBuffer, &stride, &offset );

		//�g�|���W�[.
		m_wpContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
		//���_�C���v�b�g���C�A�E�g���Z�b�g.
		m_wpContext->IASetInputLayout(m_pVertexLayout);

		//�e�N�X�`�����V�F�[�_�[�ɓn��.
		m_wpContext->PSSetSamplers( 0, 1, &m_pSampleLinear );
		m_wpContext->PSSetShaderResources( 0, 1, &m_vecvecpAsciiTexture[ iTextRow ][i] );

		//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬.
		m_psinBlend->SetBlend( true );

		//�`��.
		m_wpContext->Draw( 4, 0 );
	}
}
	 
void clsFont::SetColor( const D3DXVECTOR4 &vColor )
{
	const float fNOT_ALPHA = 1.0f;
	m_vColor = vColor;
	m_vColor.w = fNOT_ALPHA;
}


//�e�L�X�g�̓��e.
std::string clsFont::GetText( const int iRow ) const
{
	assert( static_cast<unsigned int>( iRow ) < m_vecsTextData.size() );

	return m_vecsTextData[ iRow ];
}

//�������ׂ�����{����Ԃ�( ���Ȃ��Ȃ�1.0f ).
//��������( �S�p�� )�������ڂ�.
//�Ō�̈����͕������m�̊Ԋu�ɉ����|���邩.
float clsFont::GetFineCharactorRate( 
	const int iTextRow, 
	const int iCharNum, 
	float* outfAddLeft ) const
{
	const float fDEFAULT_RATE	= 1.0f;
	const float fSLENDER_RATE	= 0.5f;
	const float fSLENDER_RATE_2 = 0.75f;
	const float fSLENDER_RATE_3 = 0.875f;

	float fReturn = fDEFAULT_RATE;
	*outfAddLeft = 0.0f;

	//���̕���������( ���p�����{�ꂩ ).
	encCHARACTOR_TYPE encCharaType = GetCharactorType( iTextRow, iCharNum );

	//���{��̐擪�o�C�g.
	if( encCharaType == encCHARACTOR_TYPE::JAPANESE_HEAD ){
		fReturn = fDEFAULT_RATE;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		return fReturn;
	}
	//���{��̖����o�C�g.
	else if( encCharaType == encCHARACTOR_TYPE::JAPANESE_FOOT ){
		fReturn = 0.0f;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		return fReturn;
	}

	switch( m_vecsTextData[ iTextRow ][ iCharNum ] )
	{
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'g':
	case 'h':
	case 'k':
	case 'n':
	case 'o':
	case 's':
		fReturn = fSLENDER_RATE_3;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		break;

	case 'f':
	case 'r':
	case 't':
	case 'v':
	case 'z':
		fReturn = fSLENDER_RATE_2;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		break;

	case 'i':
	case 'j':
	case 'l':
	case 'I':
	case '.':
	case ':':
	case '/':
	case '(':
	case ')':
	case '1':
		fReturn = fSLENDER_RATE;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		break;

	default:
		fReturn = fDEFAULT_RATE;
		*outfAddLeft = fDEFAULT_RATE - fReturn;
		break;
	}

	return fReturn;
}

clsFont::encCHARACTOR_TYPE clsFont::GetCharactorType( 
	const int iTextRow, const int iCharNum ) const
{
	//�����Ȃ��Ȃ炻��͕������Ȃ킿�A�Ƃ肠�������p��Ԃ��Ă����΂悢( ���ɓ��{��̖����������Ƃ��Ă������Ȃ�����e�����Ȃ� ).
	if( iCharNum + 1 >= static_cast<int>( m_vecsTextData[ iTextRow ].size() ) ){
		return encCHARACTOR_TYPE::ALPHABET;
	}
	//�}�C�i�X�ɍs�����Ƃ���΋����Ȃ�( ����͐�Δ��p ).
	else if( iCharNum - 1 < 0 ){
		return encCHARACTOR_TYPE::ALPHABET;
	}

	//���擪�o�C�g?.
	if(  IsDBCSLeadByte( m_vecsTextData[ iTextRow ][ iCharNum ] ) ){
		//�����̃e�N�X�`����null�Ȃ�.
		if( !m_vecvecpAsciiTexture[ iTextRow ][ iCharNum ] ){
			//���{�ꖖ���o�C�g.
			return encCHARACTOR_TYPE::JAPANESE_FOOT;
		}
		//���{��擪�o�C�g.
		return encCHARACTOR_TYPE::JAPANESE_HEAD;
	}
	//1�o�C�g�O���擪�����Ȃ�.
	else if( IsDBCSLeadByte( m_vecsTextData[ iTextRow ][ iCharNum - 1 ] ) ){
		//���{�ꖖ���o�C�g.
		return encCHARACTOR_TYPE::JAPANESE_FOOT;
	}

	//���p�����ł�.
	return encCHARACTOR_TYPE::ALPHABET;
}
