#include "SoundManagerTitle.h"

using namespace std;


clsSOUND_MANAGER_TITLE::clsSOUND_MANAGER_TITLE( const HWND hWnd )	: clsSOUND_MANAGER_BASE( hWnd )
{
}

clsSOUND_MANAGER_TITLE::~clsSOUND_MANAGER_TITLE()
{
}


void clsSOUND_MANAGER_TITLE::CreateSceneName()
{
	//���̃V�[���̃t�@�C���ɂȂ���t�@�C����.
	const string sDATA_PASS = "Title";
	m_sSceneName = sDATA_PASS;
}