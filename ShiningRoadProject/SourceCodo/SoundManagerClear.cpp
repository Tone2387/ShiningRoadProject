#include "SoundManagerClear.h"

using namespace std;

clsSOUND_MANAGER_CLEAR::clsSOUND_MANAGER_CLEAR( const HWND hWnd )	
	: clsSOUND_MANAGER_BASE( hWnd )
{
}

clsSOUND_MANAGER_CLEAR::~clsSOUND_MANAGER_CLEAR()
{
}



void clsSOUND_MANAGER_CLEAR::CreateSceneName()
{
	//���̃V�[���̃t�@�C���ɂȂ���t�@�C����.
	const string sDATA_PASS = "Clear";
	m_sSceneName = sDATA_PASS;
}