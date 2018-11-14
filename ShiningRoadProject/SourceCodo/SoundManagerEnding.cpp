#include "SoundManagerEnding.h"

using namespace std;

namespace{

}

clsSOUND_MANAGER_ENDING::clsSOUND_MANAGER_ENDING( const HWND hWnd )	: clsSOUND_MANAGER_BASE( hWnd )
{
}

clsSOUND_MANAGER_ENDING::~clsSOUND_MANAGER_ENDING()
{
}


void clsSOUND_MANAGER_ENDING::CreateSceneName()
{
	//このシーンのファイルにつなげるファイル名.
	const string sDATA_PASS = "Ending";
	m_sSceneName = sDATA_PASS;
}