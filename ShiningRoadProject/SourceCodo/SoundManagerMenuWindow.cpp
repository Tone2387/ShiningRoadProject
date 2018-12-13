#include "SoundManagerMenuWindow.h"

using namespace std;

clsSOUND_MANAGER_MENUWINDOW::clsSOUND_MANAGER_MENUWINDOW( const HWND hWnd )	: clsSOUND_MANAGER_BASE( hWnd )
{
}

clsSOUND_MANAGER_MENUWINDOW::~clsSOUND_MANAGER_MENUWINDOW()
{
}

void clsSOUND_MANAGER_MENUWINDOW::CreateSceneName()
{
	//このシーンのファイルにつなげるファイル名.
	const string sDATA_PASS = "MenuWindow";
	m_sSceneName = sDATA_PASS;
}

