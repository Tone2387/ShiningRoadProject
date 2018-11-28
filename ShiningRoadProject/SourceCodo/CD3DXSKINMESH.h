/***************************************************************************************************
*	SkinMeshCode Version 1.50
*	LastUpdate	: 2017/06/30
**/
#ifndef C_D3DXSKINMESH_H_
#define C_D3DXSKINMESH_H_
// �x���ɂ��ẴR�[�h���͂𖳌��ɂ���B4005�F�Ē�`.
#pragma warning( disable : 4005 )


#include <stdio.h>
#include <windows.h>
#include <d3dx9.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>

#include "Global.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

// �ő�{�[����.
#define MAX_BONES		255
// �ő��Ұ��ݾ�Đ�.
#define MAX_ANIM_SET	100

//// �}�N��.
//#define SAFE_RELEASE(p)		{ if(p) { (p)->Release(); (p)=NULL; } }
//#define SAFE_DELETE(p)		{ if(p) { delete (p);     (p)=NULL; } }
//#define SAFE_DELETE_ARRAY(p){ if(p) { delete[] (p);   (p)=NULL; } }

//�����񋓑�.
enum enDirection
{
	enDirection_Stop = 0,//��~.
	enDirection_Foward,//�O�i.
	enDirection_Backward,//���.
	enDirection_LeftTurn,//����].
	enDirection_RightTurn,//�E��].
	Left,
	Right
};

// �V�F�[�_�[�ɓn���l.
struct SHADER_SKIN_GLOBAL0
{	
	D3DXVECTOR4 vLightDir;	// ���C�g����.
	D3DXVECTOR4 vEye;		// �J�����ʒu.
};

struct SHADER_SKIN_GLOBAL1
{
	D3DXMATRIX	mW;			// ���[���h�s��.
	D3DXMATRIX	mWVP;		// ���[���h����ˉe�܂ł̕ϊ��s��.
	D3DXVECTOR4 vAmbient;	// �A���r�G���g��.
	D3DXVECTOR4 vDiffuse;	// �f�B�t���[�Y�F.
	D3DXVECTOR4 vSpecular;	// ���ʔ���.
	D3DXVECTOR4 vColorArmor;//�J���[.
	D3DXVECTOR4 vColorBase;	//�J���[.
};

struct SHADER_GLOBAL_BONES
{
	D3DXMATRIX mBone[MAX_BONES];
	SHADER_GLOBAL_BONES()
	{
		for( int i=0; i<MAX_BONES; i++ )
		{
			D3DXMatrixIdentity( &mBone[i] );
		}
	}
};

// �I���W�i���@�}�e���A���\����.
struct MY_SKINMATERIAL
{
	CHAR szName[110];
	D3DXVECTOR4	Ka;	// �A���r�G���g.
	D3DXVECTOR4	Kd;	// �f�B�t���[�Y.
	D3DXVECTOR4	Ks;	// �X�y�L�����[.
	CHAR szTextureName[512];	// �e�N�X�`���[�t�@�C����.
	ID3D11ShaderResourceView* pTexture;
	DWORD dwNumFace;	// ���̃}�e���A���ł���|���S����.
	MY_SKINMATERIAL()
	{
		ZeroMemory( szName, 110 );
		Ka = Kd = Ks = { 0.0f, 0.0f, 0.0f, 0.0f };
		ZeroMemory( szTextureName, 512 );
		pTexture = NULL;
		dwNumFace = 0;
	}
	~MY_SKINMATERIAL()
	{
		SAFE_RELEASE( pTexture );
	}
};

// ���_�\����.
struct MY_SKINVERTEX
{
	D3DXVECTOR3	vPos;		// ���_�ʒu.
	D3DXVECTOR3	vNorm;		// ���_�@��.
	D3DXVECTOR2	vTex;		// UV���W.
	UINT bBoneIndex[4];		// �{�[�� �ԍ�.
	float bBoneWeight[4];	// �{�[�� �d��.
	MY_SKINVERTEX()
	{
//		ZeroMemory( this, sizeof( MY_SKINVERTEX ) );
		vPos = vNorm = { 0.0f, 0.0f, 0.0f };
		vTex = { 0.0f, 0.0f };
		for( char i=0; i<4; i++ ){
			bBoneIndex[i] = 0;
			bBoneWeight[i] = 0.0f;
		}
	}
};
// �{�[���\����.
struct BONE
{
	D3DXMATRIX mBindPose;	// �����|�[�Y�i�����ƕς��Ȃ��j.
	D3DXMATRIX mNewPose;	// ���݂̃|�[�Y�i���̓s�x�ς��j.
	DWORD dwNumChild;		// �q�̐�.
	int iChildIndex[50];	// �����̎q�́g�C���f�b�N�X�h50�܂�.
	CHAR Name[256];

	BONE()
	{
		ZeroMemory( this, sizeof( BONE ) );
		D3DXMatrixIdentity( &mBindPose );
		D3DXMatrixIdentity( &mNewPose );
	}
};

// �p�[�c���b�V���\����.
struct SKIN_PARTS_MESH 
{
	DWORD	dwNumVert;
	DWORD	dwNumFace;
	DWORD	dwNumUV;
	DWORD			dwNumMaterial;
	MY_SKINMATERIAL*	pMaterial;
	char				TextureFileName[8][256];	// �e�N�X�`���[�t�@�C����(8���܂�).
	bool				bTex;

	ID3D11Buffer*	pVertexBuffer;
	ID3D11Buffer**	ppIndexBuffer;

	// �{�[��.
	int		iNumBone;
	BONE*	pBoneArray;

	bool	bEnableBones;	// �{�[���̗L���t���O.

	SKIN_PARTS_MESH()
	{
//		ZeroMemory( this, sizeof( SKIN_PARTS_MESH ) );
		dwNumVert = dwNumFace = dwNumUV = dwNumMaterial = 0;
		pMaterial = NULL;
		ZeroMemory( TextureFileName, 8 * 256 );
		bTex = bEnableBones = false;
		pVertexBuffer = NULL;
		ppIndexBuffer = NULL;
		iNumBone = 0;
		pBoneArray = NULL;
	}
};



// �f�o�C�X�֌W���p�\����.
struct CD3DXSKINMESH_INIT
{
	HWND hWnd;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

	CD3DXSKINMESH_INIT(){
		hWnd = NULL;
		pDevice = NULL;
		pDeviceContext = NULL;
	}
};

// �h���t���[���\����.
//	���ꂼ��̃��b�V���p�̍ŏI���[���h�s���ǉ�����.
struct MYFRAME: public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
	SKIN_PARTS_MESH* pPartsMesh;
	MYFRAME(){
//		ZeroMemory( this, sizeof( MYFRAME ));
		ZeroMemory( &CombinedTransformationMatrix, sizeof( D3DXMATRIX ));
		pPartsMesh = NULL;
	}
};
// �h�����b�V���R���e�i�[�\����.
//	�R���e�i�[���e�N�X�`���𕡐����Ă�悤�Ƀ|�C���^�[�̃|�C���^�[��ǉ�����
struct MYMESHCONTAINER: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;
	DWORD dwWeight;				// �d�݂̌��i�d�݂Ƃ͒��_�ւ̉e���B�j.
	DWORD dwBoneNum;			// �{�[���̐�.
	LPD3DXBUFFER pBoneBuffer;	// �{�[���E�e�[�u��.
	D3DXMATRIX** ppBoneMatrix;	// �S�Ẵ{�[���̃��[���h�s��̐擪�|�C���^.
	D3DXMATRIX* pBoneOffsetMatrices;// �t���[���Ƃ��Ẵ{�[���̃��[���h�s��̃|�C���^.
};
// X�t�@�C�����̃A�j���[�V�����K�w��ǂ݉����Ă����N���X��h��������.
//	ID3DXAllocateHierarchy�͔h�����邱�Ƒz�肵�Đ݌v����Ă���.
class MY_HIERARCHY: public ID3DXAllocateHierarchy
{
public:
	MY_HIERARCHY(){}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR , LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR ,CONST D3DXMESHDATA* ,CONST D3DXMATERIAL* , 
		CONST D3DXEFFECTINSTANCE* , DWORD , CONST DWORD *, LPD3DXSKININFO , LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME );
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER );	
};

//==================================================================================================
//
//	�p�[�T�[�N���X.
//
//==================================================================================================
class D3DXPARSER
{
public:

	D3DXPARSER();
	~D3DXPARSER();

	MY_HIERARCHY cHierarchy;
	MY_HIERARCHY* m_pHierarchy;
	LPD3DXFRAME m_pFrameRoot;

	LPD3DXANIMATIONCONTROLLER m_pAnimController;//��̫�Ăň��.
	LPD3DXANIMATIONSET m_pAnimSet[MAX_ANIM_SET];

	HRESULT LoadMeshFromX( LPDIRECT3DDEVICE9, LPSTR );
	HRESULT AllocateBoneMatrix( LPD3DXMESHCONTAINER );
	HRESULT AllocateAllBoneMatrices( LPD3DXFRAME );
	VOID UpdateFrameMatrices( LPD3DXFRAME, LPD3DXMATRIX );


	int GetNumVertices( MYMESHCONTAINER* pContainer );
	int GetNumFaces( MYMESHCONTAINER* pContainer );
	int GetNumMaterials( MYMESHCONTAINER* pContainer );
	int GetNumUVs( MYMESHCONTAINER* pContainer );
	int GetNumBones( MYMESHCONTAINER* pContainer );
	int GetNumBoneVertices( MYMESHCONTAINER* pContainer, int iBoneIndex);
	DWORD GetBoneVerticesIndices( MYMESHCONTAINER* pContainer, int iBoneIndex, int iIndexInGroup );
	double GetBoneVerticesWeights( MYMESHCONTAINER* pContainer, int iBoneIndex, int iIndexInGroup );
	D3DXVECTOR3 GetVertexCoord( MYMESHCONTAINER* pContainer, DWORD iIndex );
	D3DXVECTOR3 GetNormal( MYMESHCONTAINER* pContainer, DWORD iIndex );
	D3DXVECTOR2 GetUV( MYMESHCONTAINER* pContainer, DWORD iIndex );
	int GetIndex( MYMESHCONTAINER* pContainer, DWORD iIndex );
	D3DXVECTOR4 GetAmbient( MYMESHCONTAINER* pContainer, int iIndex );
	D3DXVECTOR4 GetDiffuse( MYMESHCONTAINER* pContainer, int iIndex );
	D3DXVECTOR4 GetSpecular( MYMESHCONTAINER* pContainer, int iIndex );
	CHAR* GetTexturePath( MYMESHCONTAINER* pContainer, int index );
	float GetSpecularPower( MYMESHCONTAINER* pContainer, int iIndex );
	int GeFaceMaterialIndex( MYMESHCONTAINER* pContainer, int iFaceIndex );
	int GetFaceVertexIndex( MYMESHCONTAINER* pContainer, int iFaceIndex, int iIndexInFace );
	D3DXMATRIX GetBindPose( MYMESHCONTAINER* pContainer, int iBoneIndex );
	D3DXMATRIX GetNewPose( MYMESHCONTAINER* pContainer, int iBoneIndex );
	CHAR* GetBoneName( MYMESHCONTAINER* pContainer, int iBoneIndex );

	// ���b�V���R���e�i���擾����.
	LPD3DXMESHCONTAINER GetMeshContainer( LPD3DXFRAME pFrame );

	// �A�j���[�V�����Z�b�g�̐؂�ւ�.
	void ChangeAnimSet( int index, LPD3DXANIMATIONCONTROLLER pAC=NULL );
	// �A�j���[�V�����Z�b�g�̐؂�ւ�(�J�n�t���[���w��\��).
	void ChangeAnimSet_StartPos( int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// �A�j���[�V������~���Ԃ��擾.
	double GetAnimPeriod( int index );
	// �A�j���[�V���������擾.
	int GetAnimMax( LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// �w�肵���{�[�����(���W�E�s��)���擾����֐�.
	bool GetMatrixFromBone( const char* sBoneName, D3DXMATRIX* const pOutMat ) const;
	bool GetPosFromBone( const char* sBoneName, D3DXVECTOR3* const pOutPos ) const;
	//�{�[�������邩�������𒲂ׂ�֐�.
	bool ExistsBone( const char* sBoneName );

	// ���b�V�����.
	HRESULT ReleaseMesh( LPD3DXFRAME pFrame );

	// �ꊇ�������.
	HRESULT Release();
};

//==================================================================================================
//
//	�X�L�����b�V���N���X.
//
//==================================================================================================
class clsD3DXSKINMESH
{
public:

	// ���\�b�h.
	clsD3DXSKINMESH( 
		const HWND hWnd, 
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11, 
		const char* sFileName );
	~clsD3DXSKINMESH();

	// �`��֐�.
	void Render( 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye, 
		const D3DXVECTOR4& vColorBase,
		const D3DXVECTOR4& vColorArmor,
		const bool isAlpha = false, 
		LPD3DXANIMATIONCONTROLLER pAC=NULL );

	//���\�[�X�̃{�[���ʒu�������΍�.
	void UpdateBonePos();
//	void UpDateBonePosProduct( LPD3DXFRAME p );
//	void UpDateBonePosProductProduct(
//		SKIN_PARTS_MESH* pMesh,
//		MYMESHCONTAINER* const pContainer );

	double GetAnimSpeed()				{ return m_dAnimSpeed;		}
	void SetAnimSpeed( double dSpeed )	{ m_dAnimSpeed = dSpeed;	}

	double GetAnimTime()				{ return m_dAnimTime; }
	void SetAnimTime(double dTime)	{ m_dAnimTime = dTime; }

	// �A�j���[�V�����Z�b�g�̐؂�ւ�.
	void ChangeAnimSet( int index, LPD3DXANIMATIONCONTROLLER pAC=NULL );
	// �A�j���[�V�����Z�b�g�̐؂�ւ�(�J�n�t���[���w��\��).
	void ChangeAnimSet_StartPos( int index, double dStartFramePos, LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// �A�j���[�V������~���Ԃ��擾.
	double GetAnimPeriod( int index );
	// �A�j���[�V���������擾.
	int GetAnimMax( LPD3DXANIMATIONCONTROLLER pAC=NULL );

	// �w�肵���{�[�����(���W�E�s��)���擾����֐�.
	bool GetMatrixFromBone( char* sBoneName, D3DXMATRIX* pOutMat );
	bool GetPosFromBone( const char* sBoneName, D3DXVECTOR3* const pOutPos, const bool isLocalPos = false ) const;
	bool GetDeviaPosFromBone(char* sBoneName, D3DXVECTOR3* pOutPos, D3DXVECTOR3 vDeviation = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//�{�[�������邩�������𒲂ׂ�֐�.
	bool ExistsBone( const char* sBoneName );

	//�p�[�T�[�N���X����A�j���[�V�����R���g���[���[���擾����.
	LPD3DXANIMATIONCONTROLLER GetAnimController()
	{
		return m_pD3dxMesh->m_pAnimController;
	}

	TRANSFORM m_Trans;


	//X���޸�ق�p��.
	D3DXVECTOR3 vecAxisX;
	//Z���޸�ق�p��.
	D3DXVECTOR3 vecAxisZ;

	//�����񋓑�.
	enDirection m_enDir;

	D3DXMATRIX m_mWorld;
	D3DXMATRIX m_mRotation;

	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;
	D3DXVECTOR3 m_vLight;
	D3DXVECTOR3 m_vEye;

	//�A�j���[�V�������x.
	double m_dAnimSpeed;
	double m_dAnimTime;

private:
	//���\�[�X�̃{�[���ʒu�������΍�.
	void UpDateBonePosProduct( LPD3DXFRAME p );
	void UpDateBonePosProductProduct(
		SKIN_PARTS_MESH* pMesh,
		MYMESHCONTAINER* const pContainer );

	//�u�����h�X�e�[�g�쐬.
	HRESULT CreateBlendState();
	// ����֐�.
	HRESULT Release();

	HRESULT CreateDeviceDx9( HWND hWnd, LPDIRECT3DDEVICE9* ppOutDevice9, const char* sErrFilePath );
	HRESULT InitShader( const char* sErrFilePath );
	// X�t�@�C������X�L�����b�V�����쐬����.
	HRESULT CreateFromX( CHAR* sFileName, LPDIRECT3DDEVICE9 pDevice9 );
	HRESULT CreateIndexBuffer( DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer );
	void RecursiveSetNewPoseMatrices( BONE* pBone,D3DXMATRIX* pmParent );

	// �S�Ẵ��b�V�����쐬����.
	void BuildAllMesh( D3DXFRAME* pFrame );
	
	// ���b�V�����쐬����.
	HRESULT CreateAppMeshFromD3DXMesh( LPD3DXFRAME pFrame );

	// X�t�@�C������X�L���֘A�̏���ǂݏo���֐�.
	HRESULT ReadSkinInfo( MYMESHCONTAINER* pContainer, MY_SKINVERTEX* pvVB, SKIN_PARTS_MESH* pParts );

	// �{�[�������̃|�[�Y�ʒu�ɃZ�b�g����֐�.
	void SetNewPoseMatrices( SKIN_PARTS_MESH* pParts, int frame, MYMESHCONTAINER* pContainer );
	// ����(���݂�)�|�[�Y�s���Ԃ��֐�.
	D3DXMATRIX GetCurrentPoseMatrix( SKIN_PARTS_MESH* pParts, int index );

	// �t���[���`��.
	void DrawFrame( 
		LPD3DXFRAME pFrame,
		const D3DXVECTOR4& vColorBase,
		const D3DXVECTOR4& vColorArmor,
		const bool isAlpha );
	// �p�[�c�`��.
	void DrawPartsMesh( 
		SKIN_PARTS_MESH* p, 
		D3DXMATRIX World, 
		MYMESHCONTAINER* const pContainer,
		const D3DXVECTOR4& vColorBase,
		const D3DXVECTOR4& vColorArmor,
		const bool isAlpha );
	void DrawPartsMeshStatic( SKIN_PARTS_MESH* pMesh, D3DXMATRIX World, MYMESHCONTAINER* pContainer );

	// �S�Ẵ��b�V�����폜.
	void DestroyAllMesh( D3DXFRAME* pFrame );
	HRESULT DestroyAppMeshFromD3DXMesh( LPD3DXFRAME p );

	//����(�A���t�@�u�����h)�ݒ�̐؂�ւ�.
	void SetBlend( const bool isAlpha );
	ID3D11BlendState*	m_pBlendState[ enBLEND_STATE_size ];		//�u�����h�X�e�[�g.


	struct MASK_TEXTURE
	{
		ID3D11ShaderResourceView*	pTex;
//		ID3D11SamplerState*			pSample;

		MASK_TEXTURE(){
			pTex = nullptr;
//			pSample = nullptr;
		}
		~MASK_TEXTURE(){
//			SAFE_RELEASE( pSample );
			SAFE_RELEASE( pTex );
		}
	};
	MASK_TEXTURE* m_pMaskBase;
	MASK_TEXTURE* m_pMaskArmor;


//	HWND m_hWnd;
	// Dx9.
//	LPDIRECT3D9			m_pD3d9;
//	LPDIRECT3DDEVICE9	m_pDevice9;
	// Dx11.
	ID3D11Device*			m_pDevice;
	ID3D11DeviceContext*	m_pDeviceContext;
	ID3D11SamplerState*		m_pSampleLinear;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pVertexLayout;
	ID3D11Buffer*			m_pConstantBuffer0;
	ID3D11Buffer*			m_pConstantBuffer1;
	ID3D11Buffer*			m_pConstantBufferBone;

	// ���b�V��.
	D3DXPARSER* m_pD3dxMesh;

	// XFile�̃p�X.
	CHAR	m_FilePath[256];

	// �A�j���[�V�����t���[��.
	int		m_iFrame;

};

#endif//#ifndef C_D3DXSKINMESH_H_