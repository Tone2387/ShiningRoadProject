#include "Resource.h"
#include "File.h"

#include "OperationString.h"



using namespace std;

namespace{

	//�p�[�c�f�B���N�g���̃p�X.
	const string sPARTS_PASS = "Data\\RoboParts\\";
	//�e�p�[�c�̃f�B���N�g���̃p�X.
	const string sLEG_PASS		= sPARTS_PASS + "Leg\\Leg";
	const string sCORE_PASS		= sPARTS_PASS + "Core\\Core";
	const string sHEAD_PASS		= sPARTS_PASS + "Head\\Head";
	const string sARML_PASS		= sPARTS_PASS + "Arms\\Arms";//�r�����E�����Ȃ̂ɕ����Ă���͎̂d�l�ύX�̉\�������邽��.
	const string sARMR_PASS		= sPARTS_PASS + "Arms\\Arms";
	const string sWEAPON_PASS	= sPARTS_PASS + "Weapon\\Weapon";
	//��L�ɐ��������񂾂����ł������郂�f����.
	const string sLEG_NAME		= "\\Leg";
	const string sCORE_NAME		= "\\Core";
	const string sHEAD_NAME		= "\\Head";
	const string sARML_NAME		= "\\ArmL";
	const string sARMR_NAME		= "\\ArmR";
	const string sWEAPON_NAME	= "\\Weapon";

	//extension = �g���q.
	const string sEXTENSION_X = ".X";//��L�̃p�X�Ƃ���̊Ԃɐ���������Ŏg��.

	const UCHAR ucARM_NUM = 2;//�r�̖{��.


	//�f�[�^�t�@�C���̃p�X.
	const string sDATA_PASS_ROOT = "Data\\RoboParts\\";
	const string sDATA_PASS_MID[] =
	{
		"Leg",
		"Core",
		"Head",
		"Arms",
		"Weapon"
	};
	const string sDATA_PASS_END	 = "\\RoboPartsData.csv";



}



clsResource::clsResource()
	:m_hWnd( nullptr )
	,m_wpDevice11( nullptr )
	,m_wpCotext11( nullptr )
	,m_ppStaticModels( nullptr )
	,m_ppSkinModels( nullptr )
	,m_ucSkinModelMax( 0 )
//	,m_PartsNum()
{
	for( UCHAR i=0; i<enPARTS_READ_SIZE; i++ ){
		m_PartsNum[i] = 0;
	}
}

clsResource::~clsResource()
{
	for( UCHAR i=0; i<enStaticModel_Max; i++ ){
		ReleaseStaticModel( static_cast<enSTATIC_MODEL>( i ) );
	}
	for( SKIN_ENUM_TYPE i = 0; i<m_ucSkinModelMax; i++ ){
		ReleaseSkinModel( static_cast<enSKIN_MODEL>( i ) );
	}
	//�X�L�����b�V���̃|�C���^�̈�����.
	if( m_ppSkinModels ){
		delete[] m_ppSkinModels;
		m_ppSkinModels = nullptr;
	}
	//�X�^�e�B�b�N���b�V���̃|�C���^�̈�����.
	if( m_ppStaticModels ){
		delete[] m_ppStaticModels;
		m_ppStaticModels = nullptr;
	}

	for( UCHAR i=0; i<enPARTS_READ_SIZE; i++ ){
		m_PartsNum[i] = 0;
	}

	m_ucSkinModelMax = 0;

	m_wpCotext11 = nullptr;
	m_wpDevice11 = nullptr;
	m_hWnd = nullptr;
}

//�p�[�c�̐���f���o��.
SKIN_ENUM_TYPE clsResource::GetPartsNum( const enPARTS_READ enPartsRead )
{
	SKIN_ENUM_TYPE tmpNum = 0;

	string tmpPass =  sDATA_PASS_ROOT + sDATA_PASS_MID[enPartsRead] + sDATA_PASS_END;
	
	unique_ptr< clsFILE > upFile = make_unique< clsFILE >();
	upFile->Open( tmpPass );

	tmpNum = static_cast<SKIN_ENUM_TYPE>( upFile->GetSizeRow() );

	upFile->Close();
	upFile.reset( nullptr );
	return tmpNum;
}

//new����Ɏg��.
void clsResource::Create( const HWND hWnd, ID3D11Device* const pDevice, ID3D11DeviceContext* const pContext )
{
#ifdef RESOURCE_READ_PARTS_MODEL_LOCK
	for( UCHAR i=0; i<enPARTS_READ_SIZE; i++ ){
		m_PartsNum[i] = iTEST_ROBO_PARTS_MODEL_MAX;
	}
#else//#ifdef RESOURCE_READ_PARTS_MODEL_LOCK
	m_PartsNum[enPARTS_READ::LEG] = GetPartsNum( enPARTS_READ::LEG );
	m_PartsNum[enPARTS_READ::CORE] = GetPartsNum( enPARTS_READ::CORE );
	m_PartsNum[enPARTS_READ::HEAD] = GetPartsNum( enPARTS_READ::HEAD );
	m_PartsNum[enPARTS_READ::ARMS] = GetPartsNum( enPARTS_READ::ARMS );
	m_PartsNum[enPARTS_READ::WEAPON] = GetPartsNum( enPARTS_READ::WEAPON );
#endif//#ifdef RESOURCE_READ_PARTS_MODEL_LOCK

	m_ucSkinModelMax = 
		m_PartsNum[enPARTS_READ::LEG] + 
		m_PartsNum[enPARTS_READ::CORE] + 
		m_PartsNum[enPARTS_READ::HEAD] + 
		( ucARM_NUM * m_PartsNum[enPARTS_READ::ARMS] ) + 
		m_PartsNum[enPARTS_READ::WEAPON] + 
		enSkinModel_Max;

	InitStaticModel( hWnd, pDevice, pContext );
	InitSkinModel( hWnd, pDevice, pContext );

	CreateStaticModel( 
		"Data\\Stage\\Collision\\StageCollision.X",
		enSTATIC_MODEL::enStaticModel_StageCollision );
	CreateStaticModel( 
		"Data\\Stage\\Floor\\StageFloor.X",
		enSTATIC_MODEL::enStaticModel_StageFloor );
	CreateStaticModel( 
		"Data\\Stage\\Celling\\StageCeling.X",
		enSTATIC_MODEL::enStaticModel_StageCelling );
	CreateStaticModel(
		"Data\\Stage\\Building\\Building.X",
		enSTATIC_MODEL::enStaticModel_Building );
	CreateStaticModel(
		"Data\\Collision\\Sphere.x",
		enSTATIC_MODEL::enStaticModel_Shpere );

	CreateSkinModel(
		"Data\\Stage\\Door\\Door.X",
		enSKIN_MODEL::enSkinModel_Door );
	CreateSkinModel(
		"Data\\Stage\\Lia\\Lia.X",
		enSKIN_MODEL::enSkinModel_Lia );

	//�p�[�c�쐬.
	CreatePartsGroup();
}

//�p�[�c�쐬.
void clsResource::CreatePartsGroup()
{
	CreateParts( enPARTS::LEG );
	CreateParts( enPARTS::CORE );
	CreateParts( enPARTS::HEAD );
	CreateParts( enPARTS::ARM_L	);
	CreateParts( enPARTS::ARM_R	);
	CreateParts( enPARTS::WEAPON_L );//�����LR�����Ȃ̂�R�͖���.
}
void clsResource::CreateParts( const enPARTS enParts )
{
	//���̃p�[�c�̎n�܂�ƏI���̔ԍ����A���ꂼ��X�L�����b�V���S�̂̉��Ԗڂɂ����邩�A�̕ϐ�.
	UCHAR ucStart, ucMax;
	//X�t�@�C���̖��O.
	string sModelName;
	//Data�f�B���N�g������X�t�@�C���܂ł̃p�X.
	string sPass = SetVarToCreateParts( ucStart, ucMax, sModelName, enParts );
	//�p�X�ƃ��f������؂藣���Ă���̂́A�f�B���N�g�����ƃ��f�����ɔԍ������邽��.
	
	//�쐬.//���̃p�[�c�̃��f����ނ̐������J��Ԃ�.
	for( UCHAR i=0; i<ucMax - ucStart; i++ ){
#if 0
		//�p�[�c�t�@�C�����A��.
		ostringstream ss;
		ss << static_cast<int>( i );		//�����𕶎����( int����Ȃ��Ǝ��̂��N���邳 ).
		//�f�B���N�g�����A���f�����ɐ�����A��.
		string tmpString = sPass + ss.str();//�p�[�c�̃f�B���N�g����.
		tmpString += sModelName + ss.str();	//�p�[�c�̃��f����.
		tmpString += sEXTENSION_X;			//�g���q�A��.
		//�p�[�c�t�@�C�����A������.
#else
		//�����񑀍�N���X�쐬.
		unique_ptr<clsOPERATION_STRING> upOprtStr = make_unique<clsOPERATION_STRING>();
	
		string tmpString = upOprtStr->ConsolidatedNumber( sPass, i );//�p�[�c�̃f�B���N�g����.
		tmpString += upOprtStr->ConsolidatedNumber( sModelName, i );//�p�[�c�̃��f����.
		tmpString += sEXTENSION_X;									//�g���q�A��.

		upOprtStr.reset( nullptr );
#endif

		//���.
		CreateSkinModel(
			const_cast<LPSTR>( tmpString.c_str() ), //tmpPass.
			static_cast<enSKIN_MODEL>( ucStart + i ) );

	}
}

//CreateParts�ŕK�v�ȕϐ�����������.
string clsResource::SetVarToCreateParts(
	SKIN_ENUM_TYPE &ucStart,	//(out)���̃p�[�c�̎n�܂�ԍ�.
	SKIN_ENUM_TYPE &ucMax,	//(out)���̃p�[�c�̍ő�ԍ�.
	string &sModelName,//(out)�p�X�ɂ������郂�f����.
	const enPARTS enParts )
{
	string sPass;

	//���̃p�[�c�̃��\�[�X�ԍ��ł̊J�n�ԍ�.
	ucStart = GetPartsResourceStart( enParts );

	switch( enParts )
	{
	case enPARTS::LEG:
		ucMax = ucStart + m_PartsNum[enPARTS_READ::LEG];
		sPass = sLEG_PASS;
		sModelName = sLEG_NAME;
		break;
	case enPARTS::CORE:
		ucMax = ucStart + m_PartsNum[enPARTS_READ::CORE];
		sPass = sCORE_PASS;
		sModelName = sCORE_NAME;
		break;
	case enPARTS::HEAD:
		ucMax = ucStart + m_PartsNum[enPARTS_READ::HEAD];
		sPass = sHEAD_PASS;
		sModelName = sHEAD_NAME;
		break;
	case enPARTS::ARM_L:
		ucMax = ucStart + m_PartsNum[enPARTS_READ::ARMS];
		sPass = sARML_PASS;
		sModelName = sARML_NAME;
		break;
	case enPARTS::ARM_R:
		ucMax = ucStart + m_PartsNum[enPARTS_READ::ARMS];
		sPass = sARMR_PASS;
		sModelName = sARMR_NAME;
		break;
	case enPARTS::WEAPON_L:
//	case enPARTS::WEAPON_R:	//��񂳂�Ȃ��悤�ɕЕ������ɂ��Ă���.
		ucMax = ucStart + m_PartsNum[enPARTS_READ::WEAPON];
		sPass = sWEAPON_PASS;
		sModelName = sWEAPON_NAME;
		break;
	default:
		assert( !"�s���ȃp�[�c���w�肳��܂���" );
		break;
	}
	return sPass;
}

//GetSkinModels()�̈������ǂ̃p�[�c���Ƃ��̃p�[�c�̔ԍ���������o���֐�.
clsResource::enSKIN_MODEL clsResource::GetPartsResourceNum( 
	const enPARTS enParts, SKIN_ENUM_TYPE PartsNum ) const
{
	enSKIN_MODEL SkinResourceNo;//return.

	//�I�[�o�[���Ă�����0�ɂ���.
	switch( enParts )
	{
	case enPARTS::LEG:
		if( m_PartsNum[enPARTS_READ::LEG] <= PartsNum ) PartsNum = 0;
		break;
	case enPARTS::CORE:
		if( m_PartsNum[enPARTS_READ::CORE] <= PartsNum ) PartsNum = 0;
		break;
	case enPARTS::HEAD:
		if( m_PartsNum[enPARTS_READ::HEAD] <= PartsNum ) PartsNum = 0;
		break;
	case enPARTS::ARM_L:
	case enPARTS::ARM_R:
		if( m_PartsNum[enPARTS_READ::ARMS] <= PartsNum ) PartsNum = 0;
		break;
	case enPARTS::WEAPON_L:
	case enPARTS::WEAPON_R:
		if( m_PartsNum[enPARTS_READ::WEAPON] <= PartsNum ) PartsNum = 0;
		break;
	default:
		assert( !"�s���ȃp�[�c���w�肳��܂���" );
		break;
	}


	SkinResourceNo = 
		static_cast<enSKIN_MODEL>( GetPartsResourceStart( enParts ) + PartsNum );

	return SkinResourceNo;
}

//SetVarToCreateParts()��GetPartsResourceNum()�̕⏕.
//���̃p�[�c�̍ŏ��̃i���o�[�����\�[�X�ԍ��ɂ��ċ����Ă����.
SKIN_ENUM_TYPE clsResource::GetPartsResourceStart( const enPARTS enParts ) const
{
	switch( enParts )
	{
	case enPARTS::LEG:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max );
		break;
	case enPARTS::CORE:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_PartsNum[enPARTS_READ::LEG] );
		break;
	case enPARTS::HEAD:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_PartsNum[enPARTS_READ::LEG] + m_PartsNum[enPARTS_READ::CORE] );
		break;
	case enPARTS::ARM_L:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_PartsNum[enPARTS_READ::LEG] + m_PartsNum[enPARTS_READ::CORE] + m_PartsNum[enPARTS_READ::HEAD] );
		break;
	case enPARTS::ARM_R:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_PartsNum[enPARTS_READ::LEG] + m_PartsNum[enPARTS_READ::CORE] + m_PartsNum[enPARTS_READ::HEAD] + m_PartsNum[enPARTS_READ::ARMS] );
		break;
	case enPARTS::WEAPON_L:
	case enPARTS::WEAPON_R:
		return static_cast<SKIN_ENUM_TYPE>( enSkinModel_Max + m_PartsNum[enPARTS_READ::LEG] + m_PartsNum[enPARTS_READ::CORE] + m_PartsNum[enPARTS_READ::HEAD] + ( ucARM_NUM * m_PartsNum[enPARTS_READ::ARMS] ) );
		break;
	default:
		assert( !"�s���ȃp�[�c���w�肳��܂���" );
		break;
	}

	return 0;
}
//���{�̃p�[�c��Attach����֐�.
//������ : ���̃p�[�c?.
//������ : ���̃p�[�c�̉��Ԗ�?.
clsD3DXSKINMESH* clsResource::GetPartsModels(
	const enPARTS enParts, const SKIN_ENUM_TYPE PartsNum )
{
	return GetSkinModels( GetPartsResourceNum( enParts, PartsNum ) );
}


//==================================================.
//	������.
//==================================================.
HRESULT clsResource::InitStaticModel( 
			const HWND hWnd,
			ID3D11Device* const pDevice,
			ID3D11DeviceContext* const pContext)
{
	m_hWnd = hWnd;
	m_wpDevice11 = pDevice;
	m_wpCotext11 = pContext;
	//�X�^�e�B�b�N���b�V���̃|�C���^�̈���m��.
	m_ppStaticModels = new clsDX9Mesh*[ enStaticModel_Max ];
	for( UCHAR i=0; i<enStaticModel_Max; i++ ){
		m_ppStaticModels[i] = nullptr;
	}

	return S_OK;
}
HRESULT clsResource::InitSkinModel( 
			const HWND hWnd,
			ID3D11Device* const pDevice,
			ID3D11DeviceContext* const pContext)
{
	m_Si.hWnd = hWnd;
	m_Si.pDevice = pDevice;
	m_Si.pDeviceContext = pContext;
	//�X�L�����b�V���̃|�C���^�̈���m��.
	m_ppSkinModels = new clsD3DXSKINMESH*[m_ucSkinModelMax];
	for ( SKIN_ENUM_TYPE i = 0; i<m_ucSkinModelMax; i++ ){
		m_ppSkinModels[i] = nullptr;
	}

	return S_OK;
}



//==================================================.
//	���f���Ǎ�.
//==================================================.
// Static Model.
HRESULT clsResource::CreateStaticModel( LPSTR const fileName, const enSTATIC_MODEL enModel )
{
	//�񋓑͈͓̂̔�?.
	if( !IsRangeStaticModel( enModel ) ){
		return E_FAIL;
	}
	//���Ƀf�[�^�����邩.
	if( m_ppStaticModels[ enModel ] != nullptr ){
		return E_FAIL;
	}

	//���f���Ǎ�.
	m_ppStaticModels[ enModel ] = new clsDX9Mesh;
	m_ppStaticModels[ enModel ]->Init(
		m_hWnd, m_wpDevice11, m_wpCotext11, fileName );

	return S_OK;
}
//	Skin Model.
HRESULT clsResource::CreateSkinModel( LPSTR const fileName, const enSKIN_MODEL enModel )
{
	//�񋓑͈͓̂̔���.
	if ( !IsRangeSkinModel( enModel ) ){
		return E_FAIL;
	}

	//���Ƀf�[�^�����邩.
	if ( m_ppSkinModels[ enModel ] != nullptr ){
		return E_FAIL;
	}

	//���f���Ǎ�.
	m_ppSkinModels[ enModel ] = new clsD3DXSKINMESH(
		m_hWnd,
		m_wpDevice11,
		m_wpCotext11,
		fileName );
//	m_ppSkinModels[ enModel ]->Init( &m_Si );
//	m_ppSkinModels[ enModel ]->CreateFromX( fileName );

	return S_OK;
}

//==================================================.
//	���f���̃|�C���^�擾.
//==================================================.
//	Static Model.
clsDX9Mesh* clsResource::GetStaticModels( const enSTATIC_MODEL enModel ) const
{
	//�񋓑͈͓̂̔�?.
	if( !IsRangeStaticModel( enModel ) ){
		return nullptr;
	}
	//���Ƀf�[�^�����邩.
	if( m_ppStaticModels[ enModel ] == nullptr ){
		return nullptr;
	}
	
	//���f���̃|�C���^�Ԃ�.
	return m_ppStaticModels[ enModel ];
}
//	Skin Model.
clsD3DXSKINMESH* clsResource::GetSkinModels( const enSKIN_MODEL enModel) const
{
	//�񋓑͈͓̂̔�?.
	if ( !IsRangeSkinModel( enModel ) ){
		return nullptr;
	}

	//���Ƀf�[�^�����邩.
	if ( m_ppSkinModels[ enModel ] == nullptr ){
		return nullptr;
	}

	//���f���̃|�C���^�Ԃ�.
	return m_ppSkinModels[ enModel ];
}


//==================================================.
//	���.
//==================================================.
//	Static Model.
HRESULT clsResource::ReleaseStaticModel( const enSTATIC_MODEL enModel )
{
	//�񋓑͈͓̂̔�?.
	if( !IsRangeStaticModel( enModel ) ){
		return E_FAIL;
	}
	//���Ƀf�[�^�����邩.
	if( m_ppStaticModels[ enModel ] == nullptr ){
		return E_FAIL;
	}

	delete m_ppStaticModels[ enModel ];
	m_ppStaticModels[ enModel ] = nullptr;

	return S_OK;
}
//	Skin Model.
HRESULT clsResource::ReleaseSkinModel( const enSKIN_MODEL enModel )
{
	//�񋓑͈͓̂̔�?.
	if( !IsRangeSkinModel( enModel ) ){
		return E_FAIL;
	}
	//���Ƀf�[�^�����邩.
	if( m_ppSkinModels[ enModel ] == nullptr ){
		return E_FAIL;
	}

	delete m_ppSkinModels[ enModel ];
	m_ppSkinModels[ enModel ] = nullptr;

	return S_OK;
}


//==================================================.
//	�͈͓����`�F�b�N����֐�.
//==================================================.
bool clsResource::IsRangeStaticModel( const enSTATIC_MODEL enModel ) const
{
	if( 0 <= enModel && enModel < enStaticModel_Max ){
		return true;//�͈͓�.
	}
	return false;	//�͈͊O.
}
bool clsResource::IsRangeSkinModel( const enSKIN_MODEL enModel ) const
{
	if ( 0 <= enModel && enModel < m_ucSkinModelMax ){
		return true;
	}
	return false;
}
