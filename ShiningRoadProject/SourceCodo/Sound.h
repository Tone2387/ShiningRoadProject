#ifndef SOUND_H_
#define SOUND_H_

#include <Windows.h>

//���C�u����.
#pragma comment( lib, "winmm.lib" )



//������o�b�t�@.
const int STR_BUFF_MAX = 256;




//============================================================
//	�T�E���h�N���X.
//		midi, mp3, wav�`���t�@�C���̍Đ��E��~�����s��.
//============================================================
class clsSound
{
public:
	clsSound();
	~clsSound();

	//�T�E���h�\����.
	struct SOUND_DATA
	{
		char sAlias[STR_BUFF_MAX];	//�G�C���A�X��.
		char sPath[STR_BUFF_MAX];	//�t�@�C���p�X (�uBGM�vor�uSE�v����ł悢 ).
		int  iMaxVolume;			//�ő剹��.
	};


	//�����t�@�C�����J��.
	bool Open( const LPCTSTR sFName, const char* const sAlias, const HWND hWnd );

	//�����t�@�C�������.
	bool Close();

	//�Đ��֐�.
	bool Play( const bool bNotify = false ) const;

	//��~�֐�.
	bool Stop() const;

//	//�ꎞ��~.
//	bool Pause();
//	//�ꎞ��~�̉���.
//	bool Resume();

	//���̒�~���m�F����֐�.
	bool IsStopped() const;

	//���̍Đ������m�F����֐�.
	bool IsPlaying() const;


	//�����߂��֐�(�Đ��ʒu������).
	bool SeekToStart() const;

	//���ʂ�ݒ肷��.
	bool SetVolume( int iVolume ) const;
	//�ő剹�ʂ�ݒ肷��.
	void SetMaxVolume( const int iVolume = 1000 ){
		m_iMaxVol = iVolume;
	}


private:
	//�����ݒ�.
	HRESULT	SetInitParam( const char *sAlias, const HWND hWnd );

	//��Ԏ擾�֐�.
	bool GetStatus( char* const sStatus ) const;


	HWND	m_hWnd;					//�E�B���h�E�n���h��.
	char	m_sAlias[STR_BUFF_MAX];	//�G�C���A�X.


	int		m_iMaxVol;//�ő剹��.

	bool	m_bOpend;//true = Open��.

};

#endif//#define SOUND_H_