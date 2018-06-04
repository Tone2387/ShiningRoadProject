#include "PartsBase.h"

const int iERROR_NUM = -1;


clsPARTS_BASE::clsPARTS_BASE() :
	m_StatusMax( 0 )
{
	
}

clsPARTS_BASE::~clsPARTS_BASE()
{
	m_viStatus.clear();
	m_viStatus.shrink_to_fit();

	m_StatusMax = 0;	
}



//作成時.
void clsPARTS_BASE::Create()
{
	CreateProduct();
}

//毎フレーム.
void clsPARTS_BASE::Update()
{
	UpdateProduct();
	ModelUpdate( m_Trans );
}

//描画.
void clsPARTS_BASE::Render(
	const D3DXMATRIX& mView, 
	const D3DXMATRIX& mProj, 
	const D3DXVECTOR3& vLight, 
	const D3DXVECTOR3& vEye,
	const D3DXVECTOR4 &vColor,
	const bool isAlpha )
{
	ModelRender( mView, mProj, vLight, vEye, vColor, isAlpha );
}


//パーツのステータスを吐き出す( 添え字は各クラスの列挙体を参照されたし ).
int clsPARTS_BASE::GetStatus( PARTS_STATUS_TYPE Num )
{
	//ステータスが範囲オーバーしていないかのチェック.
	isWithInRange( Num );

	return m_viStatus[Num];
}

//ステータスセット( 第一引数:ステータスに入れる数  第二引数:for文のカウント ).
template<class T>
void clsPARTS_BASE::SetStatus( const int iStatus, const T tNum )
{
	PARTS_STATUS_TYPE Num = static_cast<PARTS_STATUS_TYPE>( tNum );

	//ステータスが範囲オーバーしていないかのチェック.
	isWithInRange( Num );

	m_viStatus[Num] = iStatus;
}



//ステータス.
void clsPARTS_BASE::SetStatusMax( PARTS_STATUS_TYPE Num )
{
	m_StatusMax = Num;
}
PARTS_STATUS_TYPE clsPARTS_BASE::GetStatusMax()
{
	return m_StatusMax;
}


//ステータスが範囲オーバーしていないかのチェック.
bool clsPARTS_BASE::isWithInRange( PARTS_STATUS_TYPE Num )
{
	//範囲をオーバーしたら.
	if( Num >= m_StatusMax ){
		//エラーを送り付ける.
		assert( !"指定されたパーツステータスの番号は範囲外です" );
		return false;
	}
	return true;
}
