#include "SceneAssemble.h"

using namespace std;

#define TEST_TEX_PASS "Data\\Load\\LoadBack.png"

//要素数は<clsSCENE_ASSEMBLE::ENUM_SIZE>.
const string sPARTS_STATUS_PASS[] =
{
	"Data\\RoboParts\\Leg\\RoboPartsData.csv",
	"Data\\RoboParts\\Core\\RoboPartsData.csv",
	"Data\\RoboParts\\Head\\RoboPartsData.csv",
	"Data\\RoboParts\\Arms\\RoboPartsData.csv",
	"Data\\RoboParts\\Weapon\\RoboPartsData.csv",
};


//================================//
//========== 組み換えクラス ==========//
//================================//
clsSCENE_ASSEMBLE::clsSCENE_ASSEMBLE( clsPOINTER_GROUP* const ptrGroup ) : clsSCENE_BASE( ptrGroup )
	,m_pFile()//配列を0で初期化.
	,m_pAsmModel( nullptr )
{
	//念のため.
	for( UCHAR i=0; i<enPARTS_TYPES::ENUM_SIZE; i++ ){
		m_pFile[i] = nullptr;
	}
}

clsSCENE_ASSEMBLE::~clsSCENE_ASSEMBLE()
{
	SAFE_DELETE( m_pAsmModel );

	for( UCHAR i=0; i<enPARTS_TYPES::ENUM_SIZE; i++ ){
		if( m_pFile[i] == nullptr ) continue;
		m_pFile[i]->Close();
		SAFE_DELETE( m_pFile[i] );
	}
	
}

void clsSCENE_ASSEMBLE::CreateProduct()
{

//	m_pTestChara = new clsCharaStatic;
//	m_pTestChara->AttachModel( 
//		m_wpResource->GetStaticModels( 
//		clsResource::enSTATIC_MODEL::enStaticModel_Ground ) );
//	m_pTestChara->Init();
//	m_pTestChara->SetPosition( D3DXVECTOR3( 1.0f, 0.0f, 50.0f ) );
//
//	m_pParts = new clsCharaStatic;
//	m_pParts->AttachModel( 
//		m_wpResource->GetStaticModels( 
//		clsResource::enSTATIC_MODEL::enStaticModel_Enemy ) );
//	m_pParts->Init();
//	m_pParts->SetPosition( D3DXVECTOR3( -2.0f, 1.0f, 0.0f ) );
//
//
//	m_pSprite = new clsSPRITE2D_CENTER;
//	SPRITE_STATE tmpSs;
//	m_pSprite->Create( 
//		m_wpDevice, m_wpContext, 
//		TEST_TEX_PASS, tmpSs );
//	m_pSprite->SetPos( { WND_W*0.5f, WND_H*0.5f, 0.0f } );

	//パーツのステータス読み込み.
	for( UCHAR i=0; i<enPARTS_TYPES::ENUM_SIZE; i++ ){
		if( m_pFile[i] != nullptr ) continue;
		m_pFile[i] = new clsFILE;
		m_pFile[i]->Open( sPARTS_STATUS_PASS[i] );
	}

	//モデルさん作成.
	m_pAsmModel = new clsASSEMBLE_MODEL;
	m_pAsmModel->Create( m_wpResource );
	m_pAsmModel->SetAnimSpd( 0.01 );

	m_wpCamera->SetPos( { 0.0f, 0.0f, -100.0f } );
	m_wpCamera->SetLookPos( { 0.0f, 0.0f, 0.0f } );

	//ミッションシーンに引き継ぐ情報の初期化.
	m_wpRoboStatus->Clear();
}

void clsSCENE_ASSEMBLE::UpdateProduct( enSCENE &nextScene )
{
	//テストモデル初期化 & パーツ切替.
	if( GetAsyncKeyState( VK_SPACE ) & 0x1 ){
		static int tmpI = 0; 
		m_pAsmModel->AttachModel( enPARTS::LEG, tmpI );
		m_pAsmModel->AttachModel( enPARTS::CORE, tmpI );
		m_pAsmModel->AttachModel( enPARTS::HEAD, tmpI );
		m_pAsmModel->AttachModel( enPARTS::ARM_L, tmpI );
		m_pAsmModel->AttachModel( enPARTS::ARM_R, tmpI );
		m_pAsmModel->AttachModel( enPARTS::WEAPON_L, tmpI );
		m_pAsmModel->AttachModel( enPARTS::WEAPON_R, tmpI );
		tmpI ++;
		if( tmpI >= iTEST_ROBO_PARTS_MODEL_MAX ) tmpI = 0;

		m_pAsmModel->SetPos( { 0.0f, -50.0f, 0.0f } );
		m_pAsmModel->SetRot( { 0.0f, -50.0f, 0.0f } );
		m_pAsmModel->SetScale( 0.5f );
	}

	//テストモデル移動.
	float fff = 1.0f;
	if( GetAsyncKeyState( 'W' ) & 0x8000 ) m_pAsmModel->AddPos( { 0.0f, fff, 0.0f } );
	if( GetAsyncKeyState( 'S' ) & 0x8000 ) m_pAsmModel->AddPos( { 0.0f, -fff, 0.0f } );
	if( GetAsyncKeyState( 'D' ) & 0x8000 ) m_pAsmModel->AddPos( { fff, 0.0f, 0.0f } );
	if( GetAsyncKeyState( 'A' ) & 0x8000 ) m_pAsmModel->AddPos( { -fff, 0.0f, 0.0f } );
	if( GetAsyncKeyState( 'E' ) & 0x8000 ) m_pAsmModel->AddPos( { 0.0f, 0.0f, fff } );
	if( GetAsyncKeyState( 'Q' ) & 0x8000 ) m_pAsmModel->AddPos( { 0.0f, 0.0f, -fff } );
	float rrr = 0.05f;
	if( GetAsyncKeyState( 'T' ) & 0x8000 ) m_pAsmModel->AddRot( { 0.0f, rrr, 0.0f } );
	if( GetAsyncKeyState( 'G' ) & 0x8000 ) m_pAsmModel->AddRot( { 0.0f, -rrr, 0.0f } );
	if( GetAsyncKeyState( 'F' ) & 0x8000 ) m_pAsmModel->AddRot( { rrr, 0.0f, 0.0f } );
	if( GetAsyncKeyState( 'H' ) & 0x8000 ) m_pAsmModel->AddRot( { -rrr, 0.0f, 0.0f } );
	if( GetAsyncKeyState( 'R' ) & 0x8000 ) m_pAsmModel->AddRot( { 0.0f, 0.0f, rrr } );
	if( GetAsyncKeyState( 'Y' ) & 0x8000 ) m_pAsmModel->AddRot( { 0.0f, 0.0f, -rrr } );


	//選択肢.
	if( GetAsyncKeyState( VK_RIGHT ) & 0x1 )MoveCursorRight();
	if( GetAsyncKeyState( VK_LEFT ) & 0x1 ) MoveCursorLeft();
	if( GetAsyncKeyState( VK_UP ) & 0x1 )	MoveCursorUp();
	if( GetAsyncKeyState( VK_DOWN ) & 0x1 ) MoveCursorDown();
	if( GetAsyncKeyState( VK_RETURN ) & 0x1 ){
		Enter();
	}
	if( GetAsyncKeyState( VK_BACK ) & 0x1 ){
		Undo();
	}




	m_pAsmModel->UpDate();


}

void clsSCENE_ASSEMBLE::RenderProduct( const D3DXVECTOR3 &vCamPos )
{
//	m_pSprite->SetPos( ConvDimPos( m_pParts->GetPosition() ) );
////	ConvDimPos( m_pSprite->GetPos(), m_pParts->GetPosition() );
//
//	m_pParts->Render( m_mView, m_mProj, m_vLight, vCamPos );
//	m_pTestChara->Render( m_mView, m_mProj, m_vLight, vCamPos, 
//		D3DXVECTOR4(0.5f,2.0f,0.5f,0.75f), true );
//
//	m_pSprite->Render();

	m_pAsmModel->Render( m_mView, m_mProj, m_vLight, vCamPos );
//	m_pAsmModel->SetPos( m_pAsmModel->GetPos() );
//	m_pAsmModel->UpDate();
}



//カーソル移動.
void clsSCENE_ASSEMBLE::MoveCursorUp()
{
	m_PartsSelect.Num --;

	m_PartsSelect.Num = 
		KeepRange( m_PartsSelect.Num, 0, m_pFile[m_PartsSelect.Type]->GetSizeRow() );
}

void clsSCENE_ASSEMBLE::MoveCursorDown()
{
	m_PartsSelect.Num ++;

	m_PartsSelect.Num = 
		KeepRange( m_PartsSelect.Num, 0, m_pFile[m_PartsSelect.Type]->GetSizeRow() );
}

void clsSCENE_ASSEMBLE::MoveCursorRight()
{
	m_PartsSelect.Type ++;

	m_PartsSelect.Type = 
		KeepRange( m_PartsSelect.Type, 0, enPARTS_TYPES::ENUM_SIZE );
	//パーツ種類を入れ替えたときにパーツ数が違うと困るので.
	m_PartsSelect.Num = 
		KeepRange( m_PartsSelect.Num, 0, m_pFile[m_PartsSelect.Type]->GetSizeRow() );
}

void clsSCENE_ASSEMBLE::MoveCursorLeft()
{
	m_PartsSelect.Type --;

	m_PartsSelect.Type = 
		KeepRange( m_PartsSelect.Type, 0, enPARTS_TYPES::ENUM_SIZE );
	//パーツ種類を入れ替えたときにパーツ数が違うと困るので.
	m_PartsSelect.Num = 
		KeepRange( m_PartsSelect.Num, 0, m_pFile[m_PartsSelect.Type]->GetSizeRow() );
}

//決定.
void clsSCENE_ASSEMBLE::Enter()
{
	//ステータスの、CSVから削る行数.
	const int iSTATUS_CUT_NUM = 2;//番号と名前.

	//ステータスが何項目あるのか.
	const int iStatusSize = m_pFile[ m_PartsSelect.Type ]->GetSizeCol() - iSTATUS_CUT_NUM;

	//引数用変数.
	vector<int> tmpStatus;
	tmpStatus.reserve( iStatusSize );
	for( unsigned int i=0; i<iStatusSize; i++ ){
		//m_pFile[]の添え字はどのパーツか、である.
		tmpStatus.push_back( 
			m_pFile[ m_PartsSelect.Type ]->
				GetDataInt( m_PartsSelect.Num, i + iSTATUS_CUT_NUM ) );
		//GetDataInt()の第一引数は、そのパーツ部位の何番目の行を参照すればよいのか.
		//第二引数でiSTATUS_CUT_NUMを足しているのは、元の表にあるパーツ番号と名前はいらないからカットするためである.
	}

	switch( m_PartsSelect.Type )
	{
	case enPARTS_TYPES::LEG:
		m_wpRoboStatus->ReceiveLeg( tmpStatus );
		m_pAsmModel->AttachModel( enPARTS::LEG, m_PartsSelect.Num );
		break;
	case enPARTS_TYPES::CORE:
		m_wpRoboStatus->ReceiveCore( tmpStatus );
		m_pAsmModel->AttachModel( enPARTS::CORE, m_PartsSelect.Num );
		break;
	case enPARTS_TYPES::HEAD:
		m_wpRoboStatus->ReceiveHead( tmpStatus );
		m_pAsmModel->AttachModel( enPARTS::HEAD, m_PartsSelect.Num );
		break;
	case enPARTS_TYPES::ARMS:
		m_wpRoboStatus->ReceiveArms( tmpStatus );
		m_pAsmModel->AttachModel( enPARTS::ARM_L, m_PartsSelect.Num );
		m_pAsmModel->AttachModel( enPARTS::ARM_R, m_PartsSelect.Num );
		break;
	case enPARTS_TYPES::WEAPON:
		m_wpRoboStatus->ReceiveWeaponL( tmpStatus );
		m_wpRoboStatus->ReceiveWeaponR( tmpStatus );
		m_pAsmModel->AttachModel( enPARTS::WEAPON_L, m_PartsSelect.Num );
		m_pAsmModel->AttachModel( enPARTS::WEAPON_R, m_PartsSelect.Num );
		break;
	default:
		break;
	}

	//引数用変数の片づけ.
	tmpStatus.clear();
	tmpStatus.shrink_to_fit();

//	nextScene = enSCENE::MISSION;
}

//戻る.
void clsSCENE_ASSEMBLE::Undo()
{

}





//範囲内に収める( パーツの選択肢がオーバーしないようにする ).
//minはその数値より小さくならない、maxはそれ以上にはならない.
// min <= t < max.
template<class T, class MIN, class MAX >
T clsSCENE_ASSEMBLE::KeepRange( T t, const MIN min, const MAX max ) const
{
	int num = static_cast<int>( t );
	int Min = static_cast<int>( min );
	int Max = static_cast<int>( max );
	
	if( Min > num ){
		num = Min;
	}
	else if( num >= Max ){
		num = Max - 1;
	}

	return static_cast<T>( num );
}


//============================ デバッグテキスト ===========================//
#if _DEBUG
void clsSCENE_ASSEMBLE::RenderDebugText()
{
	//NULLチェック.
	ASSERT_IF_NULL( m_upText );

	char strDbgTxt[256];
	int iTxtY = 0;
	const int iOFFSET = 10;//一行毎にどれだけ下にずらすか.
	
	//モデルのpos.
	sprintf_s( strDbgTxt, 
		"ModelPos : x[%f], y[%f], z[%f]",
		m_pAsmModel->GetPos().x, 
		m_pAsmModel->GetPos().y, 
		m_pAsmModel->GetPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	//各パーツのpos.
	for( UCHAR ucNo=0; ucNo<static_cast<UCHAR>( enPARTS::MAX ); ucNo++ ){
		sprintf_s( strDbgTxt, 
			"PartsPos : x[%f], y[%f], z[%f]",
			m_pAsmModel->GetPartsPos( ucNo ).x, 
			m_pAsmModel->GetPartsPos( ucNo ).y, 
			m_pAsmModel->GetPartsPos( ucNo ).z );
		m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );
	}

	//カメラ.
	sprintf_s( strDbgTxt, 
		"CameraPos : x[%f], y[%f], z[%f]",
		GetCameraPos().x, GetCameraPos().y, GetCameraPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	sprintf_s( strDbgTxt, 
		"CamLokPos : x[%f], y[%f], z[%f]",
		GetCameraLookPos().x, GetCameraLookPos().y, GetCameraLookPos().z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	//ライト.
	sprintf_s( strDbgTxt, 
		"Light : x[%f], y[%f], z[%f]",
		m_vLight.x, m_vLight.y, m_vLight.z );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	//選択肢.
	sprintf_s( strDbgTxt, 
		"PartsSelect : Type[%f], Num[%f]",
		static_cast<float>( m_PartsSelect.Type ), static_cast<float>( m_PartsSelect.Num ) );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

	//テスト用に数値を出す.
	string tmpsString;
	tmpsString = m_pFile[m_PartsSelect.Type]->GetDataString( m_PartsSelect.Num );
	const char* tmpcString = tmpsString.c_str();
	sprintf_s( strDbgTxt, 
		tmpcString );
	m_upText->Render( strDbgTxt, 0, iTxtY += iOFFSET );

}
#endif //#if _DEBUG



