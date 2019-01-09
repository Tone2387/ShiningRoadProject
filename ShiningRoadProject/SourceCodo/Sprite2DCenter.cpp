#include "Sprite2DCenter.h"
#include "BlendState.h"

namespace{

	const float fROT_MAX = static_cast<float>( D3DX_PI * 2.0 );

}

clsSPRITE2D_CENTER::clsSPRITE2D_CENTER()
	:m_vRot( { 0.0f, 0.0f, 0.0f } )
{
}

clsSPRITE2D_CENTER::~clsSPRITE2D_CENTER()
{
}


//================================================
//	���f���쐬.
//================================================
HRESULT clsSPRITE2D_CENTER::InitModel( const SPRITE_STATE& ss, const char* sErrFileName )
{
	float fW = ss.Disp.w * 0.5f;	//�\���X�v���C�g��.
	float fH = ss.Disp.h * 0.5f;	//�\���X�v���C�g����.
	float fU = 1.0f/ ss.Anim.w;	//�P�R�}������̕�.
	float fV = 1.0f/ ss.Anim.h;	//�P�R�}������̍���.

	//�|��(�l�p�`)�̒��_���쐬.
	SpriteVertex vertices[] =
	{
		//���_���W(x,y,z)				UV���W(u,v)
		D3DXVECTOR3(-fW,  fH, 0.0f ), D3DXVECTOR2(0.0f,   fV),//���_1(����).
		D3DXVECTOR3(-fW, -fH, 0.0f ), D3DXVECTOR2(0.0f, 0.0f),//���_2(����).
		D3DXVECTOR3( fW,  fH, 0.0f ), D3DXVECTOR2(  fU,   fV),//���_3(�E��).
		D3DXVECTOR3( fW, -fH, 0.0f ), D3DXVECTOR2(  fU, 0.0f)	//���_4(�E��).
	};
	//�ő�v�f�����Z�o����.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//�o�b�t�@�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;//�g�p��(�f�t�H���g)
	bd.ByteWidth = sizeof( SpriteVertex ) * uVerMax;//���_�T�C�Y(���_�~4)
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���_�o�b�t�@�Ƃ��Ĉ���.
	bd.CPUAccessFlags = 0;	//CPU����̓A�N�Z�X���Ȃ�.
	bd.MiscFlags = 0;	//���̑��̃t���O(���g�p)
	bd.StructureByteStride = 0;//�\���̂̃T�C�Y(���g�p)

	//�T�u���\�[�X�f�[�^�\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;//�|���̒��_���Z�b�g.

	//���_�o�b�t�@�̍쐬.
	if( FAILED(
		m_wpDevice->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer)))
	{
		MessageBox( NULL, "���_�o�b�t�@�쐬���s", sErrFileName, MB_OK );
		return E_FAIL;
	}


	//���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof( SpriteVertex );//�f�[�^�Ԋu.
	UINT offset = 0;
	m_wpContext->IASetVertexBuffers(
		0, 1,
		&m_pVertexBuffer, &stride, &offset);


	//�e�N�X�`���p�̃T���v���[�\����.
	D3D11_SAMPLER_DESC SamDesc;
	ZeroMemory(&SamDesc, sizeof(SamDesc));
	//���j�A�t�B���^-(���`���)
	//	POINT:���������e��.
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	//���b�s���O���[�h.
	//	WRAP  :�J��Ԃ�
	//	MIRROR:���]�J��Ԃ�.
	//	CLAMP :�[�̖͗l�������L�΂�.
	//	BORDER:�ʓr���E�F�����߂�.
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	//�T���v���[�쐬.
	if (FAILED(
		m_wpDevice->CreateSamplerState(
		&SamDesc, &m_pSampleLinear)))//(out)�T���v���[.
	{
		MessageBox( NULL, "�T���v���쐬���s", sErrFileName, MB_OK );
		return E_FAIL;
	}

	return S_OK;
}


//�|���S������ʂɃ����_�����O.
void clsSPRITE2D_CENTER::Render()
{
	//�s��.
	D3DXMATRIX	mWorld, mTrans, mScale, mYaw, mPitch, mRoll;

	D3DXMatrixIdentity( &mWorld );	//������:�P�ʍs��쐬.
	D3DXMatrixIdentity( &mScale );	//������:�P�ʍs��쐬.

	//�g�k.
	D3DXMatrixScaling( &mScale, m_vScale.x, m_vScale.y, m_vScale.z );

	//��].
	D3DXMatrixRotationX( &mPitch,m_vRot.x );
	D3DXMatrixRotationY( &mYaw,	 m_vRot.y );
	D3DXMatrixRotationZ( &mRoll, m_vRot.z );

	//���s�ړ�.
	D3DXMatrixTranslation(&mTrans,
		m_vPos.x, m_vPos.y, m_vPos.z);

	//����.
	mWorld = mScale * mRoll * mPitch * mYaw * mTrans;

	//�g�p����V�F�[�_�̓o�^.
	m_wpContext->VSSetShader(m_pVertexShader, NULL, 0);
	m_wpContext->PSSetShader(m_pPixelShader, NULL, 0);


	//�V�F�[�_�̃R���X�^���g�o�b�t�@�Ɋe��f�[�^��n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	SPRITE2D_CONSTANT_BUFFER cd;	//�R���X�^���g�o�b�t�@.
	//�o�b�t�@���̃f�[�^�̏������J�n����map.
	if (SUCCEEDED(
		m_wpContext->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD, 0, &pData)))
	{
		//���[���h�s���n��.
		D3DXMATRIX m = mWorld;
		D3DXMatrixTranspose(&m, &m);	//�s���]�u����.					
		//�s��̌v�Z���@��DirectX��GPU�ňقȂ��.
		cd.mW = m;

		//�r���[�|�[�g�̕��A������n��.
		cd.fViewPortWidth = WND_W;
		cd.fViewPortHeight= WND_H;

		//�A���t�@�l��n��.
		cd.vColor = m_vColor;

		//UV���W.
		//�P�R�}������̊����ɃR�}�ԍ����|���Ĉʒu��ݒ肷��.
		cd.vUV.x = ( 1.0f / m_SState.Anim.w ) * m_fPatternNo.x;
		cd.vUV.y = ( 1.0f / m_SState.Anim.h ) * m_fPatternNo.y;

		memcpy_s(pData.pData, pData.RowPitch,
			(void*)(&cd), sizeof(cd));

		m_wpContext->Unmap(m_pConstantBuffer, 0);
	}

	//���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�����H.
	m_wpContext->VSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);
	m_wpContext->PSSetConstantBuffers(
		0, 1, &m_pConstantBuffer);

	//���_�o�b�t�@���Z�b�g.
	UINT stride = sizeof( SpriteVertex );	//�f�[�^�̊Ԋu.
	UINT offset = 0;
	m_wpContext->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset);

	//���_�C���v�b�g���C�A�E�g���Z�b�g.
	m_wpContext->IASetInputLayout(m_pVertexLayout);

	//�v���~�e�B�u�E�g�|���W�[���Z�b�g.
	m_wpContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`�����V�F�[�_�ɓn��.
	m_wpContext->PSSetSamplers(
		0, 1, &m_pSampleLinear);	//�T���v��-���Z�b�g.
	m_wpContext->PSSetShaderResources(
		0, 1, &m_pTexture);		//�e�N�X�`�����V�F�[�_�ɓn��.

	//�A���t�@�u�����h�p�u�����h�X�e�[�g�쐬���ݒ�.
	m_psinBlend->SetBlend( true );

	//�v���~�e�B�u�������_�����O.
	m_wpContext->Draw(4, 0);

//	//�A���t�@�u�����h�𖳌��ɂ���.
//	SetBlend(false);
}







//��]�̐��l�̌��E�����яo�Ȃ�.
D3DXVECTOR3 clsSPRITE2D_CENTER::GuardRotOver( D3DXVECTOR3& vRot )
{
	GuardRotOver( vRot.x );
	GuardRotOver( vRot.y );
	GuardRotOver( vRot.z );

	return vRot;
}

float clsSPRITE2D_CENTER::GuardRotOver( float& fRot )
{
	//0��菬�����Ȃ����傫������.
	if( fRot < 0.0f ){
		fRot += fROT_MAX;
	}
	//������傫���Ȃ�������������.
	else if( fRot > fROT_MAX ){
		fRot -= fROT_MAX;
	}
	//�͈͓��Ȃ牽�����Ȃ�.
	else{
		return fRot;
	}

	//����������Ă邩���m��Ȃ��΍�.
	GuardRotOver( fRot );

	return fRot;
}
