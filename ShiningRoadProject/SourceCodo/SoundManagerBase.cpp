#include "SoundManagerBase.h"

using namespace std;

const unsigned int uiRESERVE_SIZE_BGM = 16;
const unsigned int uiRESERVE_SIZE_SE = 256;

//BGM.
const clsSound::SOUND_DATA BGM_DATA[] =
{
	//�^�C�g��.
	{ "Title", "BGM\\100Title\\100Title.mp3", 350 },
	//�A�Z���u��.
	{ "Assemble", "BGM\\200Main\\010Stage.mp3", 250 },
	//�~�b�V����.
	{ "Mission", "BGM\\300Result\\200ResultJingle.mp3", 350 },
	//�N���A.
	{ "Clear", "BGM\\400Clear\\100EndRoll.mp3", 200 },
	//�Q�[���I�[�o�[.
	{ "GameOver", "BGM\\500Over\\100Bye.mp3", 300 },
};

//SE.
const clsSound::SOUND_DATA SE_DATA[] =
{
	//���邢���.
	{ "Enter", "SE\\700Button\\100Enter.wav", 300 },
	//�Â����.
	{ "Exit", "SE\\700Button\\200Exit.wav", 300 },
};





clsSOUND_MANAGER::clsSOUND_MANAGER( const HWND hWnd )
	:m_hWnd( hWnd )
	,m_vupBgm()
	,m_vupSe()
{

	//BGM�쐬.
	CreateSound();

}

clsSOUND_MANAGER::~clsSOUND_MANAGER()
{
	StopAllSound();

	for( unsigned int i=0; i<m_vupSe.size(); i++ ){
		if( m_vupSe[i] ){
			m_vupSe[i]->Close();
			m_vupSe[i].reset();
			m_vupSe[i] = nullptr;
		}
	}
	m_vupSe.clear();
	m_vupSe.shrink_to_fit();

	for( unsigned int i=0; i<m_vupBgm.size(); i++ ){
		if( m_vupBgm[i] ){
			m_vupBgm[i]->Close();
			m_vupBgm[i].reset();
			m_vupBgm[i] = nullptr;
		}
	}
	m_vupBgm.clear();
	m_vupBgm.shrink_to_fit();

	m_hWnd = nullptr;
}

//���ׂĒ�~.
void clsSOUND_MANAGER::StopAllSound()
{
	for( unsigned int i=0; i<m_vupBgm.size(); i++ ){
		if( m_vupBgm[i] ){
			m_vupBgm[i]->Stop();
		}
	}

	for( unsigned int i=0; i<m_vupSe.size(); i++ ){
		if( m_vupSe[i] ){
			m_vupSe[i]->Stop();
		}
	}
}




void clsSOUND_MANAGER::CreateSound()
{
	//BGM.
	{
		//���̐�.
		int iBgmNum = sizeof( BGM_DATA ) / sizeof( BGM_DATA[0] );

		//�e�ʂ𑽂߂ɂƂ�.
		m_vupBgm.reserve( uiRESERVE_SIZE_BGM );

		for( int i=0; i<iBgmNum; i++ ){
			//�z��𑝂₵��new( make_unique )����.
			m_vupBgm.push_back( nullptr );
			m_vupBgm[i] = make_unique<clsSound>();

			//���O.
			char cAliasName[STR_BUFF_MAX] = "";
			strcat_s( cAliasName, sizeof( cAliasName ), BGM_DATA[i].sAlias );
			//�쐬.
			m_vupBgm[i]->Open( BGM_DATA[i].sPath, cAliasName, m_hWnd );
			//�ő剹�ʐݒ�.
			m_vupBgm[i]->SetMaxVolume( BGM_DATA[i].iMaxVolume );
			//�����ʏ�����.
			m_vupBgm[i]->SetVolume( BGM_DATA[i].iMaxVolume );
		}
		//�]���ȕ�������.
		m_vupBgm.shrink_to_fit();
	}


	//SE.
	{
		//���̐�.
		int iSeNum = sizeof( SE_DATA ) / sizeof( SE_DATA[0] );

		//�e�ʂ𑽂߂ɂƂ�.
		m_vupSe.reserve( uiRESERVE_SIZE_SE );

		for( int i=0; i<iSeNum; i++ ){
			m_vupSe.push_back( nullptr );
			m_vupSe[i] = make_unique<clsSound>();
			//���O.
			char cAliasName[STR_BUFF_MAX] = "";
			strcat_s( cAliasName, sizeof( cAliasName ), SE_DATA[i].sAlias );
			//�쐬.
			m_vupSe[i]->Open( SE_DATA[i].sPath, cAliasName, m_hWnd );
			//�ő剹�ʐݒ�.
			m_vupSe[i]->SetMaxVolume( SE_DATA[i].iMaxVolume );
			//�����ʏ�����.
			m_vupSe[i]->SetVolume( SE_DATA[i].iMaxVolume );
		}
		//�]���ȕ�������.
		m_vupSe.shrink_to_fit();
	}
}







//----- BGM -----//
//�Đ��֐�.
bool clsSOUND_MANAGER::PlayBGM( const int bgmNo, const bool bNotify ) const
{
	return m_vupBgm[bgmNo]->Play( bNotify );
}
//��~�֐�.
bool clsSOUND_MANAGER::StopBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->Stop();
}
//���̒�~���m�F����֐�.
bool clsSOUND_MANAGER::IsStoppedBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->IsStopped();
}
//���̍Đ������m�F����֐�.
bool clsSOUND_MANAGER::IsPlayingBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->IsPlaying();
}
//�����߂��֐�(�Đ��ʒu������).
bool clsSOUND_MANAGER::SeekToStartBGM( const int bgmNo ) const
{
	return m_vupBgm[bgmNo]->SeekToStart();
}


//----- SE -----//
//�Đ��֐�.
bool clsSOUND_MANAGER::PlaySE( const int seNo, const bool bNotify ) const
{
	return m_vupSe[seNo]->Play( bNotify );
}
//��~�֐�.
bool clsSOUND_MANAGER::StopSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}
//���̒�~���m�F����֐�.
bool clsSOUND_MANAGER::IsStoppedSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}
//���̍Đ������m�F����֐�.
bool clsSOUND_MANAGER::IsPlayingSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}
//�����߂��֐�(�Đ��ʒu������).
bool clsSOUND_MANAGER::SeekToStartSE( const int seNo ) const
{
	return m_vupSe[seNo]->Stop();
}




