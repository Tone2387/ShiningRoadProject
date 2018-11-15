#ifndef SOUND_MANAGER_BASE_H_
#define SOUND_MANAGER_BASE_H_


#include "Sound.h"

#include <vector>
#include <deque>
#include <memory>



//�T�E���h�N���X�̊��N���X.
class clsSOUND_MANAGER_BASE
{
public:
	clsSOUND_MANAGER_BASE( const HWND hWnd );
	virtual ~clsSOUND_MANAGER_BASE();

	//new����Ɉ�x�����g�p.
	void Create();

	//���t���[�����g��.
	void UpdateLoop();

	//���ׂĒ�~.
	void StopAllSound();

	//----- BGM -----//
	//�Đ��֐�.
	bool PlayBGM( const int bgmNo, const bool bNotify = true )	{ return Play( m_BgmSet, m_dqisLoopBgm, m_veciBgmNum, bgmNo, bNotify ); };
	//��~�֐�.
	bool StopBGM( const int bgmNo )								{ return Stop( m_BgmSet, m_dqisLoopBgm, bgmNo ); };
	//���̒�~���m�F����֐�.
	bool IsStoppedBGM( const int bgmNo ) const					{ return IsStopped( m_BgmSet, bgmNo ); };
	//���̍Đ������m�F����֐�.
	bool IsPlayingBGM( const int bgmNo ) const					{ return IsPlaying( m_BgmSet, bgmNo ); };
	//�����߂��֐�(�Đ��ʒu������).
	bool SeekToStartBGM( const int bgmNo ) const				{ return SeekToStart( m_BgmSet, bgmNo ); };


	//----- SE -----//
	//�Đ��֐�.
	bool PlaySE( const int seNo, const bool bNotify = false )	{ return Play( m_SeSet, m_dqisLoopSe, m_veciSeNum, seNo, bNotify ); };
	//��~�֐�.
	bool StopSE( const int seNo )								{ return Stop( m_SeSet, m_dqisLoopSe, seNo ); };
	//���̒�~���m�F����֐�.
	bool IsStoppedSE( const int seNo ) const					{ return IsStopped( m_SeSet, seNo ); };
	//���̍Đ������m�F����֐�.
	bool IsPlayingSE( const int seNo ) const					{ return IsPlaying( m_SeSet, seNo ); };
	//�����߂��֐�(�Đ��ʒu������).
	bool SeekToStartSE( const int seNo ) const					{ return SeekToStart( m_SeSet, seNo ); };

protected:

	//�e�p���N���X�̃N���X����m_sSceneName�ɓ����.
	virtual void CreateSceneName() = 0;

protected:

	//�p���N���X�Œ��g������.
	std::string m_sSceneName;

private:

	//�����^�͌����Ȃ̂�.
	using SOUND_SET = 
		std::vector< std::vector< std::unique_ptr< clsSound > > >;

	//�T�E���h�N���X�쐬.
	void CreateSound( 
		SOUND_SET &vpSound,
		std::deque<bool> &dqbLoop,			//���[�v�t���O�̐�����������.
		const std::string &sFilePath,		
		const std::string &sSubPath,
		std::vector<int> &viMaxNum );

	//�T�E���h�f�[�^�쐬.
	void CreateSoundData(
		std::vector< clsSound::SOUND_DATA > &vData,
		const std::string &sFilePath,
		const std::string &sSubPath,
		std::vector<int> &viMaxNum );


	//----- �e�֐��̒��g -----//.
	//�Đ��֐�.
	bool Play( 
		const SOUND_SET &vpSound, 
		std::deque<bool> &dqbLoop,
		std::vector<int> &viNum,
		const int No, const bool bNotify ) const;
	//��~�֐�.
	bool Stop( 
		const SOUND_SET &vpSound, std::deque<bool> &dqbLoop, const int No ) const;
	//���̒�~���m�F����֐�.
	bool IsStopped( const SOUND_SET &vpSound, const int No ) const;
	//���̍Đ������m�F����֐�.
	bool IsPlaying( const SOUND_SET &vpSound, const int No ) const;
	//�����߂��֐�(�Đ��ʒu������).
	bool SeekToStart( const SOUND_SET &vpSound, const int No ) const;


private:

	//�T�E���h�N���X.
	SOUND_SET m_BgmSet;
	SOUND_SET m_SeSet;
	//���[�v�Đ����邩�̃t���O.
	std::deque<bool> m_dqisLoopBgm;
	std::deque<bool> m_dqisLoopSe;
	//���𓯎��Đ��p�ɉ����?�̃t���O( �N�����̂� ).
	//���A���ɍĐ�����͉̂��Ԗڂ����w���t���O.
	std::vector<int> m_veciBgmNum;
	std::vector<int> m_veciSeNum;



	HWND m_hWnd;

};

#endif//#ifndef SOUND_MANAGER_BASE_H_