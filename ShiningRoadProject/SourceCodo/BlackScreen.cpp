#include "BlackScreen.h"

const float fDARK_RIMIT = 1.0f;//暗転限界.
const float fDAWN_RIMIT = 0.0f;//明転限界.



clsBLACK_SCREEN::clsBLACK_SCREEN()
	:m_enState( enSTATE::NEUTRAL )
	,m_fChangeSpd( 0.0f )
	,m_uiFlgGroup( 0 )
{
}

clsBLACK_SCREEN::~clsBLACK_SCREEN()
{
	m_uiFlgGroup = 0;
	m_fChangeSpd = 0;
	m_enState = enSTATE::BRIGHT;
}

//暗転指示.//Alpha Per Frame.
void clsBLACK_SCREEN::GetDark( const float fDarkSpd )
{
//	//作動中は受け付けない.
//	if( m_enState != enSTATE::NEUTRAL )	return;
	//暗くならない場合も受け付けない.
	if( fDarkSpd <= 0.0f )	return;

	m_fChangeSpd = fDarkSpd;
	UpdateBitFlg( DARK_ROUTE, true );
	UpdateBitFlg( BRIGHT_ROUTE, false );
	m_enState = enSTATE::DARK;
}

//明転指示.//Alpha Per Frame.
void clsBLACK_SCREEN::GetBright( const float fBrightSpd )
{
//	//作動中は受け付けない.
//	if( m_enState != enSTATE::NEUTRAL )	return;
	//明るくならない場合も受け付けない.
	if( fBrightSpd >= 0.0f )	return;

	m_fChangeSpd = fBrightSpd;
	UpdateBitFlg( BRIGHT_ROUTE, true );
	UpdateBitFlg( DARK_ROUTE, false );
	m_enState = enSTATE::BRIGHT;
}

//暗明転の実行.
void clsBLACK_SCREEN::Update()
{
	if( m_enState == enSTATE::NEUTRAL )	return;

	//透過値の更新を行い、終了したならばその旨を返す.
	if( !AddAlpha( m_fChangeSpd ) ){
		//このif文の中へは、透過値がオーバーしたら入る.
		//暗転中だったなら.
		if( isBitFlg( DARK_ROUTE ) ){
			UpdateBitFlg( DARK_ROUTE, false );
			UpdateBitFlg( DARK_END, true );
		}
		//明転中だったなら.
		else if( isBitFlg( BRIGHT_ROUTE ) ){
			UpdateBitFlg( BRIGHT_ROUTE, false );
			UpdateBitFlg( BRIGHT_END, true );
		}
		m_enState = enSTATE::NEUTRAL;
	}
}

//暗転が完了するとtrueを返す.
bool clsBLACK_SCREEN::isDarkEnd()
{
	if( isBitFlg( DARK_END ) ){
		UpdateBitFlg( DARK_END, false );
		m_enState = enSTATE::NEUTRAL;
		return true;
	}

	return false;
}


//明転が完了するとtrueを返す.
bool clsBLACK_SCREEN::isBrightEnd()
{
	if( isBitFlg( BRIGHT_END ) ){
		UpdateBitFlg( BRIGHT_END, false );
		m_enState = enSTATE::NEUTRAL;
		return true;
	}

	return false;
}



//ビットフラグ更新.
void clsBLACK_SCREEN::UpdateBitFlg( const enFLG_GROUP enFlg, const bool bStand )
{
	//ビットを立てる.
	if( bStand ){
		m_uiFlgGroup |= 1 << enFlg;
	}
	//ビットを落とす.
	else{
		m_uiFlgGroup &= ~( 1 << enFlg );
	}
}


//フラグチェック.
bool clsBLACK_SCREEN::isBitFlg( const enFLG_GROUP enFlg ) const
{
	if( ( m_uiFlgGroup >> enFlg ) & 1 ){
		return true;
	}
	return false;
}
