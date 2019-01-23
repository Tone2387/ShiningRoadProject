#include "SoundManagerTakeoff.h"

using namespace std;

namespace{


}

clsSOUND_MANAGER_TAKEOFF::clsSOUND_MANAGER_TAKEOFF( const HWND hWnd )	
	: clsSOUND_MANAGER_BASE( hWnd )
{
}

clsSOUND_MANAGER_TAKEOFF::~clsSOUND_MANAGER_TAKEOFF()
{
}


void clsSOUND_MANAGER_TAKEOFF::CreateSceneName()
{
	//このシーンのファイルにつなげるファイル名.
	const string sDATA_PASS = "Takeoff";
	m_sSceneName = sDATA_PASS;
}