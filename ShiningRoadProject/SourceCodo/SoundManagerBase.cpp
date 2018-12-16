#include "SoundManagerBase.h"
#include "File.h"

#include "OperationString.h"

#include <assert.h>

using namespace std;

//�T�E���h�N���X�֎w�����o���ۂ�vector�͈̔͂𒴂��Ă�����A���߂ł��Ƃ�������������}�N��.
#define SOUND_NUMBER_OVER_SHECK(no,vp) \
if( (no) >= static_cast<int>( (vp).size() ) ){\
	assert( !"�w�肳�ꂽ�ԍ��̃T�E���h�N���X�͑��݂��܂���" );\
	return false;\
}

namespace{

	//�Y����.
	const char cALIAS_NUM = 0;	//�G�C���A�X��.
	const char cPATH_NUM = 1;	//�����f�[�^�̃t�@�C���p�X.
	const char cVOLUME_NUM = 2;	//�ő剹��.
	const char cMAX_NUM = 3;	//�����Đ��\��.

	//�t�@�C���N���X�̃p�X�p.
	const string sDATA_PASS = "Data\\Sound\\Data\\";
	const string sBGM_PASS = "BGM.csv";
	const string sSE_PASS = "SE.csv";
	const string sSUB_PASS_BGM = "BGM\\";
	const string sSUB_PASS_SE = "SE\\";

	//���[�v�t���O�̏�����.
	const bool bLOOP_INIT = false;

	//�G�C���A�X�Ɣԍ��̊�,�ԍ��̌�ɑ}������.
	const char sCONSOLIDATE[] = "cons";
}


clsSOUND_MANAGER_BASE::clsSOUND_MANAGER_BASE( const HWND hWnd )
	:m_hWnd( hWnd )
	,m_BgmSet()
	,m_SeSet()
	,m_dqisLoopBgm()
	,m_dqisLoopSe()
	,m_veciBgmNum()
	,m_veciSeNum()
{
}

clsSOUND_MANAGER_BASE::~clsSOUND_MANAGER_BASE()
{
	StopAllSound();

	//�T�E���h�N���X.
	for( unsigned int i=0; i<m_SeSet.size(); i++ ){
		for( unsigned int j=0; j<m_SeSet[i].size(); j++ ){
			if( m_SeSet[i][j] ){
//				m_SeSet[i][j]->Close();
				m_SeSet[i][j].reset();
				m_SeSet[i][j] = nullptr;
			}
		}
		m_SeSet[i].clear();
		m_SeSet[i].shrink_to_fit();
	}
	m_SeSet.clear();
	m_SeSet.shrink_to_fit();

	for( unsigned int i=0; i<m_BgmSet.size(); i++ ){
		for( unsigned int j=0; j<m_BgmSet[i].size(); j++ ){
			if( m_BgmSet[i][j] ){
//				m_BgmSet[i][j]->Close();
				m_BgmSet[i][j].reset();
				m_BgmSet[i][j] = nullptr;
			}
		}
		m_BgmSet[i].clear();
		m_BgmSet[i].shrink_to_fit();
	}
	m_BgmSet.clear();
	m_BgmSet.shrink_to_fit();

	m_dqisLoopBgm.clear();
	m_dqisLoopBgm.shrink_to_fit();
	m_dqisLoopSe.clear();
	m_dqisLoopSe.shrink_to_fit();

	m_veciBgmNum.clear();
	m_veciBgmNum.shrink_to_fit();
	m_veciSeNum.clear();
	m_veciSeNum.shrink_to_fit();



	m_hWnd = nullptr;

}





void clsSOUND_MANAGER_BASE::Create()
{
	//�p���N���X�œ����֐�( m_sSceneName������ ).
	CreateSceneName();

	//CSV�f�[�^�̃p�X.
	string tmpDataPass = sDATA_PASS + m_sSceneName;
	string BgmDataPath = tmpDataPass + sBGM_PASS;
	string SeDataPath  = tmpDataPass + sSE_PASS;

	//BGM.
	CreateSound( m_BgmSet, m_dqisLoopBgm, BgmDataPath, sSUB_PASS_BGM, m_veciBgmNum );
	//SE.
	CreateSound( m_SeSet,  m_dqisLoopSe,  SeDataPath,  sSUB_PASS_SE,  m_veciSeNum );
}

//���t���[�����g��.
void clsSOUND_MANAGER_BASE::UpdateLoop()
{
	//���[�v�Đ�.
	for( unsigned int i=0; i<m_BgmSet.size(); i++ ){
		//���[�v����K�v����?.
		if( !m_dqisLoopBgm[i] ){
			continue;
		}
		//���݂���?.
		if( !m_BgmSet[i][0] ){
			continue;
		}
		//�~�܂��Ă���Ȃ�.
		if( IsStoppedBGM( i ) ){
			//������x.
			PlayBGM( i, true );//���[�v���鉹�͎������[�v�ł���悤�ɂ���.
		}		
	}

	for( unsigned int i=0; i<m_SeSet.size(); i++ ){
		if( !m_dqisLoopSe[i] ){
			continue;
		}
		if( !m_SeSet[i][0] ){
			continue;
		}
		if( IsStoppedSE( i ) ){
			PlaySE( i, true );
		}		
	}
}

//���ׂĒ�~.
void clsSOUND_MANAGER_BASE::StopAllSound()
{
	for( unsigned int i=0; i<m_BgmSet.size(); i++ ){
		StopBGM( i );
	}

	for( unsigned int i=0; i<m_SeSet.size(); i++ ){
		StopSE( i );
	}
}


//�T�E���h�N���X�쐬.
void clsSOUND_MANAGER_BASE::CreateSound( 
	SOUND_SET &vpSound,
	std::deque<bool> &dqbLoop,
	const string &sFilePath,
	const string &sSubPath,
	vector<int> &viMaxNum )
{
	//�T�E���h�f�[�^.
	vector< clsSound::SOUND_DATA > vData;
	CreateSoundData( vData, sFilePath, sSubPath, viMaxNum );

	//���̐�.
	int iSoundNum = vData.size();

	//�G�C���A�X���p.
	clsOPERATION_STRING OprtStr;

	//���s�ڂɏ�������?.
	vpSound.resize( iSoundNum );
	for( unsigned int i=0; i<vpSound.size(); i++ )
	{
		vpSound[i].resize( viMaxNum[i] );
		//���܂œ����Đ�������?.
		for( unsigned int j=0; j<vpSound[i].size(); j++ ){
			vpSound[i][j]= make_unique<clsSound>();
			
			//�G�C���A�X�Ɣԍ��̊Ԃɑ}������.
			vData[i].sAlias += sCONSOLIDATE;
			//�G�C���A�X���ɐ������Ȃ���.
			string tmpAlias = OprtStr.ConsolidatedNumber( vData[i].sAlias.c_str(), j );
			//�ԍ��̌�ɑ}������.
			tmpAlias += sCONSOLIDATE;

			//�쐬.
			assert( vpSound[i][j]->Open( vData[i].sPath.c_str(), tmpAlias.c_str(), m_hWnd ) );
//			vpSound[i][j]->Open( vData[i].sPath.c_str(), tmpAlias.c_str(), m_hWnd );
			//�ő剹�ʐݒ�.
			vpSound[i][j]->SetMaxVolume( vData[i].iMaxVolume );
			//�����ʏ�����.
			vpSound[i][j]->SetVolume( vData[i].iMaxVolume );
		}
		vpSound[i].shrink_to_fit();
	}
	//�]���ȕ�������.
	vpSound.shrink_to_fit();
	vData.clear();
	vData.shrink_to_fit();
	//���[�v�t���O�쐬.
	dqbLoop.resize( vpSound.size(), bLOOP_INIT );
	dqbLoop.shrink_to_fit();
}

//�T�E���h�f�[�^�쐬.
void clsSOUND_MANAGER_BASE::CreateSoundData(
	vector< clsSound::SOUND_DATA > &vData,
	const string &sFilePath,
	const string &sSubPath,
	std::vector<int> &viMaxNum )
{
	unique_ptr< clsFILE > upFile = make_unique< clsFILE >();
	//�J���Ȃ�������X���[.
	if( upFile->Open( sFilePath ) ){
		//�Ƃ肠��������.
		vData.clear();
		viMaxNum.clear();
		//�t�@�C�����Ƃ����s�������₷.
		vData.resize( upFile->GetSizeRow() );
		viMaxNum.resize( vData.size() );
		//�f�[�^�̒��g������.
		for( unsigned int i=0; i<upFile->GetSizeRow(); i++ ){
			vData[i].sAlias = upFile->GetDataString( i, cALIAS_NUM );
			vData[i].sPath = sSubPath + upFile->GetDataString( i, cPATH_NUM );
			vData[i].iMaxVolume = upFile->GetDataInt( i, cVOLUME_NUM );
			
			viMaxNum[i] = upFile->GetDataInt( i, cMAX_NUM );
		}
	}
	upFile->Close();
	upFile.reset( nullptr );
}




////----- BGM -----//
////�Đ��֐�.
//bool clsSOUND_MANAGER_BASE::PlayBGM( const int bgmNo, const bool bNotify )
//{
//	return Play( m_BgmSet, m_dqisLoopBgm, m_veciBgmNum, bgmNo, bNotify );
//}
////��~�֐�.
//bool clsSOUND_MANAGER_BASE::StopBGM( const int bgmNo )
//{
//	return Stop( m_BgmSet, m_dqisLoopBgm, bgmNo );
//}
////���̒�~���m�F����֐�.
//bool clsSOUND_MANAGER_BASE::IsStoppedBGM( const int bgmNo ) const
//{
//	return IsStopped( m_BgmSet, bgmNo );
//}
////���̍Đ������m�F����֐�.
//bool clsSOUND_MANAGER_BASE::IsPlayingBGM( const int bgmNo ) const
//{
//	return IsPlaying( m_BgmSet, bgmNo );
//}
////�����߂��֐�(�Đ��ʒu������).
//bool clsSOUND_MANAGER_BASE::SeekToStartBGM( const int bgmNo ) const
//{
//	return SeekToStart( m_BgmSet, bgmNo );
//}


////----- SE -----//
////�Đ��֐�.
//bool clsSOUND_MANAGER_BASE::PlaySE( const int seNo, const bool bNotify )
//{
//	return Play( m_SeSet, m_dqisLoopSe, m_veciSeNum, seNo, bNotify );
//}
////��~�֐�.
//bool clsSOUND_MANAGER_BASE::StopSE( const int seNo )
//{
//	return Stop( m_SeSet, m_dqisLoopSe, seNo );
//}
////���̒�~���m�F����֐�.
//bool clsSOUND_MANAGER_BASE::IsStoppedSE( const int seNo ) const
//{
//	return IsStopped( m_SeSet, seNo );
//}
////���̍Đ������m�F����֐�.
//bool clsSOUND_MANAGER_BASE::IsPlayingSE( const int seNo ) const
//{
//	return IsPlaying( m_SeSet, seNo );
//}
////�����߂��֐�(�Đ��ʒu������).
//bool clsSOUND_MANAGER_BASE::SeekToStartSE( const int seNo ) const
//{
//	return SeekToStart( m_SeSet, seNo );
//}


//----- �e�֐��̒��g -----//.
//�Đ��֐�.
bool clsSOUND_MANAGER_BASE::Play( 
	const SOUND_SET &vpSound, 
	std::deque<bool> &dqbLoop,
	std::vector<int> &viNum,
	const int No, 
	const bool bNotify ) const
{
	SOUND_NUMBER_OVER_SHECK( No, vpSound );
	dqbLoop[No] = bNotify;
#if 0
	vpSound[No]->SeekToStart();
	return vpSound[No]->Play( bNotify );
#else//#if 0

	//���݂��Ȃ��Ԃ������疖���̉���炷.
	unsigned int uiNo = static_cast<int>( No );
	if( uiNo >= vpSound.size() ){
		uiNo =  vpSound.size() - 1;
//		assert( !"�Ȃ�����I�΂Ȃ���" );
	}


	//����No�̉��̉��Ԗڂ�炷��?( ������m_veciBgmNum�Am_veciSeNum�̎g�������ς�� ).
	viNum[ uiNo ] ++;
	if( static_cast<unsigned int>( viNum[ uiNo ] ) >= vpSound[ uiNo ].size() ){
		viNum[ uiNo ] = 0;
	}

	vpSound[ uiNo ][ viNum[ uiNo ] ]->SeekToStart();

	return vpSound[uiNo][viNum[uiNo]]->Play(bNotify);
#endif//#if 0
}

//��~�֐�.
bool clsSOUND_MANAGER_BASE::Stop( 
	const SOUND_SET &vpSound, std::deque<bool> &dqbLoop, const int No ) const
{
	SOUND_NUMBER_OVER_SHECK( No, vpSound );
	dqbLoop[No] = bLOOP_INIT;

#if 0
	vpSound[No]->SeekToStart();

	return vpSound[No]->Stop();
#else//#if 0

	for( unsigned int i=0; i<vpSound[No].size(); i++ ){
		//���Ɏ~�܂��Ă���Ȃ牽�����Ȃ�.
		if( vpSound[No][i]->IsStopped() ){
			continue;
		}
		if( !vpSound[No][i]->Stop() ){
//			assert( !"�~�߂�܂���ł���" );
			return false;
		}
	}
	return true;

#endif//#if 0
}

//���̒�~���m�F����֐�.
bool clsSOUND_MANAGER_BASE::IsStopped( const SOUND_SET &vpSound, const int No ) const
{
	SOUND_NUMBER_OVER_SHECK( No, vpSound );
	for( unsigned int i=0; i<vpSound[No].size(); i++ ){
		if( !vpSound[No][i]->IsStopped() ){
			return false;
		}
	}
	return true;
}

//���̍Đ������m�F����֐�.
bool clsSOUND_MANAGER_BASE::IsPlaying( const SOUND_SET &vpSound, const int No ) const
{
	SOUND_NUMBER_OVER_SHECK( No, vpSound );
	for( unsigned int i=0; i<vpSound[No].size(); i++ ){
		if( !vpSound[No][i]->IsPlaying() ){
			return false;
		}
	}
	return true;
}

//�����߂��֐�(�Đ��ʒu������).
bool clsSOUND_MANAGER_BASE::SeekToStart( const SOUND_SET &vpSound, const int No ) const
{
	SOUND_NUMBER_OVER_SHECK( No, vpSound );
	for( unsigned int i=0; i<vpSound[No].size(); i++ ){
		if( !vpSound[No][i]->SeekToStart() ){
			assert( !"�߂��Ȃ�����" );
			return false;
		}
	}
	return true;
}


