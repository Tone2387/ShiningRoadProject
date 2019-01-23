#include "SceneMovieBase.h"
#include "File.h"


clsSCENE_MOVIE_BASE::clsSCENE_MOVIE_BASE( 
	clsPOINTER_GROUP* const ptrGroup,
	const enSCENE enScene,
	const char* sCutFrameFilePath  )
	:clsSCENE_BASE( ptrGroup )
	,m_enNEXT_SCENE( enScene )
	,m_iCut( -1 )//����������++�����̂ł��̎���0�ɂȂ�悤��.
	,m_fMovieFrame( 0.0f )
	,m_isTrigger( false )
{
	clsFILE File;
	assert( File.Open( sCutFrameFilePath ) );

	//���̃V�[��( �z��̍Ō�̓Y�����ɂȂ鐔 ).
	m_iCutEnd = File.GetSizeRow() - 1;

	//�t���[�����̃Z�b�g.
	m_vecfMovieFrameNext.resize( File.GetSizeRow() );
	for( unsigned int i=0; i<m_vecfMovieFrameNext.size(); i++ ){
		m_vecfMovieFrameNext[i] = File.GetDataInt( i, 0 );
	}
}

clsSCENE_MOVIE_BASE::~clsSCENE_MOVIE_BASE()
{
}

void clsSCENE_MOVIE_BASE::CreateProduct()
{
	InitMovie();
}


void clsSCENE_MOVIE_BASE::UpdateProduct( enSCENE &enNextScene )
{
#ifndef CAMERA_FREE_MOVE_
//	if( m_enCut != enCUT_END ){ m_fMovieFrame ++; }
	m_fMovieFrame ++;
#endif//CAMERA_FREE_MOVE_

	if( isPressEnter() ){
#ifdef _DEBUG
		//�t���[�����̕ϐ������̃J�b�g�֔�Ԑ��܂ő��₷.
		NextCut();
#else//#ifdef _DEBUG
		//���[�r�[���X�L�b�v�����̃V�[����.
		enNextScene = m_enNEXT_SCENE;
#endif//#ifdef _DEBUG
	}

	//���̃J�b�g�̃t���[�����𖞂�������.
	assert( m_iCut < static_cast<int>( m_vecfMovieFrameNext.size() ) );
	if( m_fMovieFrame > m_vecfMovieFrameNext[ m_iCut ] ){
		//���̃V�[����.
		if( m_iCut == m_iCutEnd ){
			enNextScene = m_enNEXT_SCENE;
		}
		//���̃J�b�g��.
		else{
			InitMovie();
		}
	}

	UpdateMovie();
}


void clsSCENE_MOVIE_BASE::InitMovie()
{
	++ m_iCut;
	m_fMovieFrame = 0.0f;
	m_isTrigger = false;

	InitMovieProduct();
}


void clsSCENE_MOVIE_BASE::UpdateMovie()
{
	int iOtherDataIndex = 0;

	UpdateMovieProduct( iOtherDataIndex );

}

//�t���[�����������Ă��Ȃ��Ă����̃J�b�g�֔��.
void clsSCENE_MOVIE_BASE::NextCut()
{
	//�z��̃T�C�Y�`�F�b�N.
	if( m_iCut <= m_iCutEnd ){
		//�J�E���g�ϐ������ɐi�ނ܂ő��₷.
		m_fMovieFrame = m_vecfMovieFrameNext[ m_iCut ] + 1.0f;
	}
}
