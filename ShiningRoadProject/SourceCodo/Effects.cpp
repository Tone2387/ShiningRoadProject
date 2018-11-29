#include "Effects.h"
#include "Global.h"
#include "OperationString.h"
#include "File.h"

using namespace std;

namespace{

	//�G�t�F�N�g�Ǘ��p�C���X�^���X�ő吔.
	//�ő�`��X�v���C�g��.(��΂��Ȃ�int�^�ɂ���)
	const /*int32_t*/ int g_RenderSpriteMax = 1024 * 1;
	const int g_EffectInstanceMax = g_RenderSpriteMax * 1;

	const string sDATA_PATH = "Data\\Effekseer\\Effects.csv";

	const string sFILE_PATH = "Data\\Effekseer\\Effects\\";

	const int uiEFFECT_FILE_NAME_INDEX = 0;//�t�@�C�������i�[����Ă���C���f�b�N�X.
	//const int uiMAX_PLAY_INDEX = 1;//�ő�Đ���.

	////�G�t�F�N�g�t�@�C���̃��X�g.
	//const wchar_t g_strFileNameList[ clsEffects::enEFFECTS_MAX ][128] =
	//{
	//	L"Data\\Effekseer\\tex\\asiato.efk",
	//	L"Data\\Effekseer\\tex\\arbia_kougeki.efk",
	//	L"Data\\Effekseer\\tex\\syougekiha.efk",
	//	L"Data\\Effekseer\\tex\\kougeki.efk",
	//
	//	L"Data\\Effekseer\\tex\\Teki_kougeki.efk",
	//	L"Data\\Effekseer\\tex\\yarare.efk",
	//	L"Data\\Effekseer\\tex\\bikkuri.efk",
	//
	//	L"Data\\Effekseer\\tex\\hibana.efk",
	//
	//	L"Data\\Effekseer\\tex\\takara_hasira.efk",
	//	L"Data\\Effekseer\\tex\\takara_nakami.efk",
	//
	//	L"Data\\Effekseer\\tex\\tobira_sunabokori.efk",
	//};

}



clsEffects::clsEffects()
{
//	ZeroMemory( this, sizeof( clsEffects ) );
	m_pManager = nullptr;
	m_pRender = nullptr;

#ifdef EFFECTS_USE_XAUDIO_
	m_pSound = nullptr;
	m_pXA2 = nullptr;
	m_pXA2Master = nullptr;
#endif//#ifdef EFFECTS_USE_XAUDIO_

	for( unsigned int i=0; i<m_vecpEffect.size(); i++ ){
		m_vecpEffect[i] = nullptr;
	}
}

clsEffects::~clsEffects()
{
	StopAll();
	Destroy();
}

//==================================================
//	�j���֐�.
//==================================================
HRESULT clsEffects::Destroy()
{
	//�G�t�F�N�g�f�[�^���.
	ReleaseData();

	//��ɃG�t�F�N�g�Ǘ��p�C���X�^���X��j��.
	assert( m_pManager );
	m_pManager->Destroy();

#ifdef EFFECTS_USE_XAUDIO_
	//���ɉ��Đ��p�C���X�^���X��j��.
	assert( m_pSound );
	m_pSound->Destory();
#endif//#ifdef EFFECTS_USE_XAUDIO_

	//�`��p�C���X�^���X��j��.
	assert( m_pRender );
	m_pRender->Destory();

#ifdef EFFECTS_USE_XAUDIO_
	//XAudio2�̉��.
	if( m_pXA2Master != nullptr ){
		m_pXA2Master->DestroyVoice();
		m_pXA2Master = nullptr;
	}
	ES_SAFE_RELEASE( m_pXA2 );
#endif//#ifdef EFFECTS_USE_XAUDIO_

	return S_OK;
}

//==================================================
//	�f�[�^����֐�.
//==================================================
HRESULT clsEffects::ReleaseData()
{
	//�G�t�F�N�g�̔j��.
	for( unsigned int i=0; i<m_vecpEffect.size(); i++ ){
		ES_SAFE_RELEASE( m_vecpEffect[i] );
	}
	m_vecpEffect.clear();
	m_vecpEffect.shrink_to_fit();

	return S_OK;
}


//==================================================
//	�\�z�֐�.
//==================================================
HRESULT clsEffects::Create( ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext )
{
	//������.
	if( FAILED( Init( pDevice, pContext ) ) ){
		return E_FAIL;
	}

	//�f�[�^�Ǎ�.
	if( FAILED( LoadData() ) ){
		return E_FAIL;
	}

	return S_OK;
}

//������.
HRESULT clsEffects::Init( ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext )
{
#ifdef EFFECTS_USE_XAUDIO_
	//XAudio2�̏��������s��.
	if( FAILED( XAudio2Create( &m_pXA2 ) ) ){
		ERR_MSG( "XAudio2�쐬���s", "clsEffects::Create()" );
		return E_FAIL;
	}
	if( FAILED( m_pXA2->CreateMasteringVoice( &m_pXA2Master ) ) ){
		ERR_MSG( "MasteringVoice�쐬���s", "clsEffects::Create()" );
		return E_FAIL;
	}
#endif//#ifdef EFFECTS_USE_XAUDIO_

	//�`��p�C���X�^���X�̐���.
	m_pRender = ::EffekseerRendererDX11::Renderer::Create(
		pDevice, pContext, g_RenderSpriteMax );

	//�G�t�F�N�g�Ǘ��p�C���X�^���X.
	m_pManager = ::Effekseer::Manager::Create( g_EffectInstanceMax );

	//�`��p�C���X�^���X����`��@�\��ݒ�.
	m_pManager->SetSpriteRenderer( m_pRender->CreateSpriteRenderer() );
	m_pManager->SetRibbonRenderer( m_pRender->CreateRibbonRenderer() );
	m_pManager->SetRingRenderer( m_pRender->CreateRingRenderer() );
	m_pManager->SetModelRenderer( m_pRender->CreateModelRenderer() );

	//�`��C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�.
	//�Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���.
	m_pManager->SetTextureLoader( m_pRender->CreateTextureLoader() );
	m_pManager->SetModelLoader( m_pRender->CreateModelLoader() );

#ifdef EFFECTS_USE_XAUDIO_
	//���Đ��p�C���X�^���X�̐���.
	m_pSound = ::EffekseerSound::Sound::Create( m_pXA2, 16, 16 );

	//���Đ��p�C���X�^���X����Đ��@�\��ݒ�.
	m_pManager->SetSoundPlayer( m_pSound->CreateSoundPlayer() );

	//���Đ��p�C���X�^���X����T�E���h�f�[�^�̓Ǎ��@�\��ݒ�.
	//�Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���.
	m_pManager->SetSoundLoader( m_pSound->CreateSoundLoader() );
#endif//#ifdef EFFECTS_USE_XAUDIO_

	return S_OK;
}

//==================================================
//	�f�[�^�Ǎ��֐�.
//==================================================
HRESULT clsEffects::LoadData()
{
	unique_ptr< clsFILE > upFile = make_unique<clsFILE>();
	upFile->Open( sDATA_PATH );

	string tmpString;

	//�����񑀍�p.
	clsOPERATION_STRING OprtStr;

	//�G�t�F�N�g�̓Ǎ�.
	m_vecpEffect.resize( upFile->GetSizeRow() );
	for( unsigned int i=0; i<m_vecpEffect.size(); i++ ){
		//�p�X�����.
		tmpString = sFILE_PATH + upFile->GetDataString( i, uiEFFECT_FILE_NAME_INDEX );

		//�}���`�o�C�g������( char* )���烏�C�h�o�C�g������( wchat_t* )���쐬.
		const wchar_t *tmpPath = OprtStr.CreateWcharPtrFromCharPtr( tmpString.c_str() );

		//�쐬.
		m_vecpEffect[i] =
			::Effekseer::Effect::Create( 
				m_pManager, 
				(const EFK_CHAR*)( tmpPath ) );

		//��������̂͏���.
		delete[] tmpPath;

		assert( m_vecpEffect[i] );
//		//�G���[.
//		if( m_vecpEffect[i] == nullptr ){
//			char strMsg[128];
//			wsprintf( strMsg, "clsEffects::LoadData()\n%ls",
//				tmpPath );
//
//			ERR_MSG( strMsg, "�G�t�F�N�g�t�@�C���Ǎ����s" );
//			return E_FAIL;
//		}
	}

	upFile->Close();
	upFile.reset( nullptr );

	return S_OK;
}

//==================================================
//	�`��.
//==================================================
void clsEffects::Render( 
	const D3DXMATRIX& mView, const D3DXMATRIX& mProj, const D3DXVECTOR3& vEye ) const
{
	//�r���[�s��ݒ�.
	SetViewMatrix( mView );

	//�v���W�F�N�V�����s��ݒ�.
	SetProjectionMatrix( mProj );

	//�G�t�F�N�g�̍X�V����.
	m_pManager->Update();

	//-----------------------------
	//	Effekseer�����_�����O.
	//-----------------------------
	//�G�t�F�N�g�̕`��J�n�������s��.
	m_pRender->BeginRendering();

	//�G�t�F�N�g�̕`����s��.
	m_pManager->Draw();

	//�G�t�F�N�g�̕`��I���������s��.
	m_pRender->EndRendering();

}

//==================================================
//	�r���[�s��ݒ�.
//==================================================
void clsEffects::SetViewMatrix( const D3DXMATRIX& mView ) const
{
	//�r���[(�J����)�s��.
	::Effekseer::Matrix44 tmpEsCamMat;

	//DirectX Matrix �� Effekseer Matrix.
	tmpEsCamMat = MatrixDxToEfk( &mView );

	//�r���[�s���ݒ�.
	m_pRender->SetCameraMatrix( tmpEsCamMat );

}

//==================================================
//	�v���W�F�N�V�����s��ݒ�.
//==================================================
void clsEffects::SetProjectionMatrix( const D3DXMATRIX& mProj ) const
{
	//�v���W�F�N�V�����s��.
	::Effekseer::Matrix44 tmpEsPrjMat;

	//DirectX Matrix �� Effekseer Matrix.
	tmpEsPrjMat = MatrixDxToEfk( &mProj );

	//�v���W�F�N�V�����s���ݒ�.
	m_pRender->SetProjectionMatrix( tmpEsPrjMat );

}

//==================================================
//	�x�N�^�[�ϊ��֐�(1)	DirectX Vector3 �� Effekseer Vector3.
//==================================================
::Effekseer::Vector3D clsEffects::Vector3DDxToEfk( const D3DXVECTOR3* pSrcVec3Dx ) const
{
	return ::Effekseer::Vector3D(
		pSrcVec3Dx->x,
		pSrcVec3Dx->y,
		pSrcVec3Dx->z );
}

//==================================================
//	�x�N�^�[�ϊ��֐�(2)	Effekseer Matrix �� DirectX Matrix.
//==================================================
D3DXVECTOR3 clsEffects::Vector3EfkToDx( const ::Effekseer::Vector3D* pSrcVec3Efk ) const
{
	return D3DXVECTOR3(
		pSrcVec3Efk->X,
		pSrcVec3Efk->Y,
		pSrcVec3Efk->Z );
}

//==================================================
//	�s��ϊ��֐�(1)	DirectX Matrix �� Effekseer Matrix.
//==================================================
::Effekseer::Matrix44 clsEffects::MatrixDxToEfk(
				const D3DXMATRIX* pSrcMatDx ) const
{
	::Effekseer::Matrix44 OutMatEfk;

#if 0

	for( int i=0; i<4; i++ ){
		for( int j=0; j<4; j++ ){
			OutMatEfk.Values[i][j] = pSrcMatDx->m[i][j];
		}
	}

#else

	OutMatEfk.Values[0][0] = pSrcMatDx->_11;
	OutMatEfk.Values[0][1] = pSrcMatDx->_12;
	OutMatEfk.Values[0][2] = pSrcMatDx->_13;
	OutMatEfk.Values[0][3] = pSrcMatDx->_14;

	OutMatEfk.Values[1][0] = pSrcMatDx->_21;
	OutMatEfk.Values[1][1] = pSrcMatDx->_22;
	OutMatEfk.Values[1][2] = pSrcMatDx->_23;
	OutMatEfk.Values[1][3] = pSrcMatDx->_24;

	OutMatEfk.Values[2][0] = pSrcMatDx->_31;
	OutMatEfk.Values[2][1] = pSrcMatDx->_32;
	OutMatEfk.Values[2][2] = pSrcMatDx->_33;
	OutMatEfk.Values[2][3] = pSrcMatDx->_34;

	OutMatEfk.Values[3][0] = pSrcMatDx->_41;
	OutMatEfk.Values[3][1] = pSrcMatDx->_42;
	OutMatEfk.Values[3][2] = pSrcMatDx->_43;
	OutMatEfk.Values[3][3] = pSrcMatDx->_44;

#endif

	return OutMatEfk;
}

//==================================================
//	�s��ϊ��֐�(2)	Effekseer Matrix �� DirectX Matrix.
//==================================================
D3DXMATRIX clsEffects::MatrixEfkToDx(
				const ::Effekseer::Matrix44* pSrcMatEfk ) const
{
	D3DXMATRIX OutMatDx;

#if 0

	for( int i=0; i<4; i++ ){
		for( int j=0; j<4; j++ ){
			OutMatDx.m[i][j] = pSrcMatEfk->Values[i][j];
		}
	}

#else

	OutMatDx._11 = pSrcMatEfk->Values[0][0];
	OutMatDx._12 = pSrcMatEfk->Values[0][1];
	OutMatDx._13 = pSrcMatEfk->Values[0][2];
	OutMatDx._14 = pSrcMatEfk->Values[0][3];
			
	OutMatDx._21 = pSrcMatEfk->Values[1][0];
	OutMatDx._22 = pSrcMatEfk->Values[1][1];
	OutMatDx._23 = pSrcMatEfk->Values[1][2];
	OutMatDx._24 = pSrcMatEfk->Values[1][3];
			
	OutMatDx._31 = pSrcMatEfk->Values[2][0];
	OutMatDx._32 = pSrcMatEfk->Values[2][1];
	OutMatDx._33 = pSrcMatEfk->Values[2][2];
	OutMatDx._34 = pSrcMatEfk->Values[2][3];
			
	OutMatDx._41 = pSrcMatEfk->Values[3][0];
	OutMatDx._42 = pSrcMatEfk->Values[3][1];
	OutMatDx._43 = pSrcMatEfk->Values[3][2];
	OutMatDx._44 = pSrcMatEfk->Values[3][3];

#endif

	return OutMatDx;
}

