#pragma once


#include "Sound.h"


#include <vector>
#include <memory>


////���w��p�񋓑�.
//enum enBGM : UCHAR
//{
////	enBGM_START_UP = 0,
//	enBGM_TITLE = 0,
//	enBGM_ASSEMBLE,
//	enBGM_MISSION,
//	enBGM_ENDING,
//	enBGM_OVER,
//
//	enBGM_MAX,
//};
//enum enSE : UCHAR
//{
//	enSE_ENTER = 0,
//	enSE_EXIT,
//
//	enSE_MAX,
//};


class clsSOUND_MANAGER
{
public:
	clsSOUND_MANAGER( const HWND hWnd );
	~clsSOUND_MANAGER();

	//���ׂĒ�~.
	void StopAllSound();

	//----- BGM -----//
	//�Đ��֐�.
	bool PlayBGM( const int bgmNo, const bool bNotify = true ) const;
	//��~�֐�.
	bool StopBGM( const int bgmNo ) const;
	//���̒�~���m�F����֐�.
	bool IsStoppedBGM( const int bgmNo ) const;
	//���̍Đ������m�F����֐�.
	bool IsPlayingBGM( const int bgmNo ) const;
	//�����߂��֐�(�Đ��ʒu������).
	bool SeekToStartBGM( const int bgmNo ) const;


	//----- SE -----//
	//�Đ��֐�.
	bool PlaySE( const int seNo, const bool bNotify = false ) const;
	//��~�֐�.
	bool StopSE( const int seNo ) const;
	//���̒�~���m�F����֐�.
	bool IsStoppedSE( const int seNo ) const;
	//���̍Đ������m�F����֐�.
	bool IsPlayingSE( const int seNo ) const;
	//�����߂��֐�(�Đ��ʒu������).
	bool SeekToStartSE( const int seNo ) const;

private:

	//�T�E���h�N���X�쐬.
	void CreateSound();


	//�T�E���h�N���X.
//	clsSound*	m_pBgm[enBGM_MAX];
//
//	clsSound*	m_pSe[enSE_MAX];

	std::vector< std::unique_ptr< clsSound > > m_vupBgm;
	std::vector< std::unique_ptr< clsSound > > m_vupSe;

	HWND m_hWnd;
};

