#include "WindowBox.h"


using namespace std;


namespace{

	const char LINE_MAX = 2;
	const char JOINT_MAX = 4;

	//�p�̉�]�p.
	const float fROT_ROLL = static_cast< float >( 3.14 * 0.5 );

	const float fSCALE = 16.0f;

}


clsLINE_BOX::clsLINE_BOX(		
	ID3D11Device* const pDevice11,
	ID3D11DeviceContext* const pContext11 )
	:m_vPos()
	,m_vSize()
	,m_fAlpha( 1.0f )
	,m_isChangeBigW( false )
	,m_isChangeBigH( false )
	,m_encBeforChange( encBEFOR_CHANGE::NOTHING )
	,m_isStopChange( false )
{
	const char* sLINE_PATH = "Data\\Image\\MenuWindow\\UiLineBody.png";
	const char* sJOINT_PATH = "Data\\Image\\MenuWindow\\UiLineJoint.png";

	const WHSIZE_FLOAT SS_DISP = { 1.0f, 1.0f };
	const WHSIZE_FLOAT SS_ANIM = { 1.0f, 1.0f };

	m_vPos = { WND_W/2, WND_H/2, 0.0f };

	SPRITE_STATE ss;
	ss.Disp = SS_DISP;
	ss.Anim = SS_ANIM;

	for( char i=0; i<LINE_MAX; i++ ){
		if( m_upLineH[i] ) continue;
		m_upLineH[i] = make_unique< clsSPRITE2D_CENTER >();
		m_upLineH[i]->Create( pDevice11, pContext11, sLINE_PATH, ss );
		m_upLineH[i]->SetScale( fSCALE );
	}

	for( char i=0; i<LINE_MAX; i++ ){
		if( m_upLineV[i] ) continue;
		m_upLineV[i] = make_unique< clsSPRITE2D_CENTER >();
		m_upLineV[i]->Create( pDevice11, pContext11, sLINE_PATH, ss );
		m_upLineV[i]->SetScale( fSCALE );
		m_upLineV[i]->SetRot( { 0.0f, 0.0f, fROT_ROLL } );
	}


	for( char i=0; i<JOINT_MAX; i++ ){
		if( m_upLineJoint[i] ) continue;
		m_upLineJoint[i] = make_unique< clsSPRITE2D_CENTER >();
		m_upLineJoint[i]->Create( pDevice11, pContext11, sJOINT_PATH, ss );
		m_upLineJoint[i]->SetScale( fSCALE );
		m_upLineJoint[i]->SetRot( { 0.0f, 0.0f, ( static_cast<float>( i ) * fROT_ROLL ) } );
	}

}

clsLINE_BOX::~clsLINE_BOX()
{
	for( char i=0; i<JOINT_MAX; i++ ){
		if( !m_upLineJoint[i] ) continue;
		m_upLineJoint[i].reset( nullptr );
	}

	for( char i=0; i<LINE_MAX; i++ ){
		if( !m_upLineV[i] ) continue;
		m_upLineV[i].reset( nullptr );
	}

	for( char i=0; i<LINE_MAX; i++ ){
		if( !m_upLineH[i] ) continue;
		m_upLineH[i].reset( nullptr );
	}
}


void clsLINE_BOX::Update()
{
	AddSizeForTarget();
	SetUpBox();
	SetUpJointSize();

}

//�����`���.
void clsLINE_BOX::SetUpBox()
{
	D3DXVECTOR3 vHarfSize = m_vSize * 0.5f;

	for( char i=0; i<LINE_MAX; i++ ){
		m_upLineH[i]->SetPos( m_vPos );
		m_upLineV[i]->SetPos( m_vPos );
	}
	for( char i=0; i<JOINT_MAX; i++ ){
		m_upLineJoint[i]->SetPos( m_vPos );
	}

	m_upLineH[0]->AddPos( { 0.0f, -vHarfSize.y, 0.0f } );
	m_upLineH[1]->AddPos( { 0.0f,  vHarfSize.y, 0.0f } );
	for( char i=0; i<LINE_MAX; i++ ){
		m_upLineH[i]->SetScale( { m_vSize.x, fSCALE, 1.0f } );
	}

	m_upLineV[0]->AddPos( { -vHarfSize.x, 0.0f, 0.0f } );
	m_upLineV[1]->AddPos( {  vHarfSize.x, 0.0f, 0.0f } );
	for( char i=0; i<LINE_MAX; i++ ){
		m_upLineV[i]->SetScale( { m_vSize.y, fSCALE, 1.0f } );
	}

	//���ォ�玞�v���.
	m_upLineJoint[0]->SetPos( { m_upLineV[0]->GetPos().x, m_upLineH[0]->GetPos().y, 1.0f } );
	m_upLineJoint[1]->SetPos( { m_upLineV[1]->GetPos().x, m_upLineH[0]->GetPos().y, 1.0f } );
	m_upLineJoint[2]->SetPos( { m_upLineV[1]->GetPos().x, m_upLineH[1]->GetPos().y, 1.0f } );
	m_upLineJoint[3]->SetPos( { m_upLineV[0]->GetPos().x, m_upLineH[1]->GetPos().y, 1.0f } );

}

//�p��scale.
void clsLINE_BOX::SetUpJointSize()
{
	D3DXVECTOR3 tmpScale;
	tmpScale.z = 1.0f;

	//X.
	if( m_vSize.x <= 0.0f ){
		tmpScale.x = 0.0f;
	}
	else if( m_vSize.x <= fSCALE ){
		tmpScale.x = m_vSize.x;
	}
	else{
		tmpScale.x = fSCALE;
	}

	//Y.
	if( m_vSize.y <= 0.0f ){
		tmpScale.y = 0.0f;
	}
	else if( m_vSize.y <= fSCALE ){
		tmpScale.y = m_vSize.y;
	}
	else{
		tmpScale.y = fSCALE;
	}

	//�Z�b�g.
	for( char i=0; i<JOINT_MAX; i++ ){
		m_upLineJoint[i]->SetScale( tmpScale );
	}
}

void clsLINE_BOX::Render()
{

	for( char i=0; i<LINE_MAX; i++ ){
		m_upLineH[i]->Render();
		m_upLineV[i]->Render();
	}


	for( char i=0; i<JOINT_MAX; i++ ){
		m_upLineJoint[i]->Render();
	}

}

void clsLINE_BOX::SetSizeTarget( const D3DXVECTOR3 &vSize )
{
	m_vSizeTarget = vSize;

	D3DXVECTOR3 tmp = m_vSizeTarget - m_vSize;

	if( tmp.x > 0.0f )	m_isChangeBigW = true;
	else				m_isChangeBigW = false;

	if( tmp.y > 0.0f )	m_isChangeBigH = true;
	else				m_isChangeBigH = false;
}

void clsLINE_BOX::AddChangeData( 
	const float fWidthSpd,
	const float fHeightSpd,
	const encBEFOR_CHANGE encBefor )
{
	m_vChangeSpd = { fWidthSpd, fHeightSpd, 0.0f };
	m_encBeforChange = encBefor;

	switch( m_encBeforChange )
	{
	case encBEFOR_CHANGE::WIDTH:
		break;
	case encBEFOR_CHANGE::HEIGHT:
		break;
	case encBEFOR_CHANGE::BOTH:
		break;
	default:
		assert( !"�s���Ȓl���^�����܂���" );
		break;
	}
	m_isStopChange = false;
}
//Target�Ɍ����ăT�C�Y��ς���.
void clsLINE_BOX::AddSizeForTarget()
{
	if( m_encBeforChange == encBEFOR_CHANGE::NOTHING ){
		return;
	}

	bool isEndW, isEndH;
	isEndW = isEndH = false;

	switch( m_encBeforChange )
	{
	case encBEFOR_CHANGE::WIDTH:
		if( m_isChangeBigW ){
			AddSize( { m_vChangeSpd.x, 0.0f, 0.0f } );
			if( m_vSize.x > m_vSizeTarget.x ){
				SetSize( { m_vSizeTarget.x, m_vSize.y, 0.0f } );
				isEndW = true;
			}
		}
		else{
			AddSize( { -m_vChangeSpd.x, 0.0f, 0.0f } );
			if( m_vSize.x < m_vSizeTarget.x ){
				SetSize( { m_vSizeTarget.x, m_vSize.y, 0.0f } );
				isEndW = true;
			}
		}
		//�ω����I����ĂȂ��Ȃ�I���.
		if( !isEndW ){
			break;
		}

		if( m_isChangeBigH ){
			AddSize( { 0.0f, m_vChangeSpd.y, 0.0f } );
			if( m_vSize.y > m_vSizeTarget.y ){
				SetSize( { m_vSize.x, m_vSizeTarget.y, 0.0f } );
				isEndH = true;
			}
		}
		else{
			AddSize( { 0.0f, -m_vChangeSpd.y, 0.0f } );
			if( m_vSize.y < m_vSizeTarget.y ){
				SetSize( { m_vSize.x, m_vSizeTarget.y, 0.0f } );
				isEndH = true;
			}
		}
		break;
	case encBEFOR_CHANGE::HEIGHT:
		if( m_isChangeBigH ){
			AddSize( { 0.0f, m_vChangeSpd.y, 0.0f } );
			if( m_vSize.y > m_vSizeTarget.y ){
				SetSize( { m_vSize.x, m_vSizeTarget.y, 0.0f } );
				isEndH = true;
			}
		}
		else{
			AddSize( { 0.0f, -m_vChangeSpd.y, 0.0f } );
			if( m_vSize.y < m_vSizeTarget.y ){
				SetSize( { m_vSize.x, m_vSizeTarget.y, 0.0f } );
				isEndH = true;
			}
		}

		//�ω����I����ĂȂ��Ȃ�I���.
		if( !isEndH ){
			break;
		}

		if( m_isChangeBigW ){
			AddSize( { m_vChangeSpd.x, 0.0f, 0.0f } );
			if( m_vSize.x > m_vSizeTarget.x ){
				SetSize( { m_vSizeTarget.x, m_vSize.y, 0.0f } );
				isEndW = true;
			}
		}
		else{
			AddSize( { -m_vChangeSpd.x, 0.0f, 0.0f } );
			if( m_vSize.x < m_vSizeTarget.x ){
				SetSize( { m_vSizeTarget.x, m_vSize.y, 0.0f } );
				isEndW = true;
			}
		}
		break;
	case encBEFOR_CHANGE::BOTH:
		if( m_isChangeBigW ){
			AddSize( { m_vChangeSpd.x, 0.0f, 0.0f } );
			if( m_vSize.x > m_vSizeTarget.x ){
				SetSize( { m_vSizeTarget.x, m_vSize.y, 0.0f } );
				isEndW = true;
			}
		}
		else{
			AddSize( { -m_vChangeSpd.x, 0.0f, 0.0f } );
			if( m_vSize.x < m_vSizeTarget.x ){
				SetSize( { m_vSizeTarget.x, m_vSize.y, 0.0f } );
				isEndW = true;
			}
		}
		if( m_isChangeBigH ){
			AddSize( { 0.0f, m_vChangeSpd.y, 0.0f } );
			if( m_vSize.y > m_vSizeTarget.y ){
				SetSize( { m_vSize.x, m_vSizeTarget.y, 0.0f } );
				isEndH = true;
			}
		}
		else{
			AddSize( { 0.0f, -m_vChangeSpd.y, 0.0f } );
			if( m_vSize.y < m_vSizeTarget.y ){
				SetSize( { m_vSize.x, m_vSizeTarget.y, 0.0f } );
				isEndH = true;
			}
		}
		break;
	default:
		assert( !"�s���Ȓl�������Ă��܂�" );
		break;
	}

	//�ω��I��.
	if( isEndW && isEndH ){
		m_encBeforChange = encBEFOR_CHANGE::NOTHING;
		m_isStopChange = true;
	}
}

void clsLINE_BOX::SetAlpha( const float fAlpha )
{
	m_fAlpha = fAlpha;

	for( char i=0; i<LINE_MAX; i++ ){
		m_upLineH[i]->SetAlpha( m_fAlpha );
		m_upLineV[i]->SetAlpha( m_fAlpha );
	}


	for( char i=0; i<JOINT_MAX; i++ ){
		m_upLineJoint[i]->SetAlpha( m_fAlpha );
	}
}

//�͈͂�over�����false���Ԃ��Ă���.
bool clsLINE_BOX::AddAlpha( const float fAlpha )
{
	float tmp = m_fAlpha + fAlpha;

	if( tmp > 1.0f ){
		m_fAlpha = 1.0f;
		return false;
	}
	else if( tmp < 0.0f ){
		m_fAlpha = 0.0f;
		return false;
	}

	m_fAlpha = tmp;
	return false;
}


