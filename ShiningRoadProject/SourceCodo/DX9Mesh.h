#ifndef DX9MESH_H_
#define DX9MESH_H_
//�x���ɂ��ẴR�[�h���͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning( disable:4005 )	

//class LPD3DXMESH;
#include <d3dx9.h>

#include "Global.h"


#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )



//���b�V���f�[�^���t�@�C��������o�����߂�����DirectX9���g�p����.
//�������_�����O(�\��)��DirectX11�ōs��.
class clsDX9Mesh
{
public:
	clsDX9Mesh();	//�R���X�g���N�^.
	~clsDX9Mesh();	//�f�X�g���N�^.

	TRANSFORM m_Trans;

	//������.
	HRESULT Init( const HWND hWnd, ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11, const LPSTR fileName );


	//�����_�����O�p(��DX9MESH����Main����2���݂���̂Œ���).
	void Render( 
		const D3DXMATRIX& mView,
		const D3DXMATRIX& mProj,
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const D3DXVECTOR4& vColor = D3DXVECTOR4( 1.0f,1.0f,1.0f,1.0f ),
		const bool isAlpha = false );

	//����(�A���t�@�u�����h)�ݒ�̐؂�ւ�.
	void SetBlend( const bool isAlpha );


public:

	LPD3DXMESH			m_pMesh;		//���b�V���I�u�W�F�N�g.

	D3DXVECTOR3	m_vRay;			//���C�̈ʒu.
	D3DXVECTOR3	m_vAxis;		//��]��.
	LPD3DXMESH	m_pMeshForRay;	//���C�ƃ��b�V���p.

private:

	//�u�����h�X�e�[�g�쐬.
	HRESULT CreateBlendState();

	//Dx9�������p.
	HRESULT InitDx9( const HWND hWnd, LPDIRECT3DDEVICE9* ppOutDevice9, const LPSTR sErrFilePath );

	//���b�V���Ǎ�.
	HRESULT LoadXMesh( const LPSTR fileName, LPDIRECT3DDEVICE9 pDevice9  );

	//�V�F�[�_�쐬.
	HRESULT InitShader( const LPSTR sErrFilePath );

private:


	D3DXVECTOR3 vecAxisX;
	//Z���޸�ق�p��.
	D3DXVECTOR3 vecAxisZ;

//	HWND				m_hWnd;	//�E�B���h�E�n���h��.

	//Dx9.
//	LPDIRECT3D9			m_pD3d;	//DX9�I�u�W�F�N�g.
//	LPDIRECT3DDEVICE9	m_pDevice9;	//Dx9�f�o�C�X�I�u�W�F�N�g.

	//D3DMATERIAL9*		m_pMaterials;	//�}�e���A�����.
	//LPDIRECT3DTEXTURE9*	m_pTextures;	//�e�N�X�`�����.
	//char				m_TexFileName[8][255];	//�e�N�X�`���t�@�C����(8���܂�).
	DWORD				m_dwNumMaterials;//�}�e���A����.

	//Dx11.
	ID3D11Device*			m_pDevice;		//�f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_pContext;	//�f�o�C�X�R���e�L�X�g.
	ID3D11VertexShader*		m_pVertexShader;	//���_�V�F�[�_.
	ID3D11InputLayout*		m_pVertexLayout;	//���_���C�A�E�g.
	ID3D11PixelShader*		m_pPixelShader;		//�s�N�Z���V�F�[�_.
	ID3D11Buffer*			m_pConstantBuffer0;	//�R���X�^���g�o�b�t�@0.
	ID3D11Buffer*			m_pConstantBuffer1;	//�R���X�^���g�o�b�t�@1.
//
//
	ID3D11Buffer*			m_pVertexBuffer;//���_(�o�[�e�b�N�X)�o�b�t�@.
	ID3D11Buffer**			m_ppIndexBuffer;//�C���f�b�N�X�o�b�t�@.
	ID3D11SamplerState*		m_pSampleLinear;//�e�N�X�`���̃T���v���[.//�e�N�X�`���Ɋe��t�B���^��������.
//
//
	//�}�e���A���\����.
	struct MY_MATERIAL
	{
		D3DXVECTOR4		Ambient;	//�A���r�G���g.
		D3DXVECTOR4		Diffuse;	//�f�B�t���[�Y.
		D3DXVECTOR4		Specular;	//�X�y�L����.
		CHAR	szTextureName[128];	//�e�N�X�`���t�@�C����.
		ID3D11ShaderResourceView*	pTexture;//�e�N�X�`��.
		DWORD	dwNumFace;			//���̃}�e���A���̃|���S����.
		//�R���X�g���N�^.
		MY_MATERIAL(){
	//		ZeroMemory( this, sizeof( MY_MATERIAL ) );
			pTexture = nullptr;
		}
		//�f�X�g���N�^.
		~MY_MATERIAL(){
			SAFE_RELEASE( this->pTexture );
		}
	}	*m_pMaterials;	//�}�e���A���\����.

	DWORD			m_NumAttr;		//������.
	DWORD			m_AttrID[300];	//����ID ��300�����܂�.

	bool			m_bTexture;		//�e�N�X�`���̗L��.



	ID3D11BlendState*	m_pBlendState[ enBLEND_STATE_size ];		//�u�����h�X�e�[�g.

};

#endif//#ifndef DX9MESH_H_
