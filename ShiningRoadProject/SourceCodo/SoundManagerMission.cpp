#include "SoundManagerMission.h"

using namespace std;

namespace{

	//このシーンのファイルにつなげるファイル名.
	const string sDATA_PASS = "Mission";

}

clsSOUND_MANAGER_MISSION::clsSOUND_MANAGER_MISSION( const HWND hWnd )	: clsSOUND_MANAGER_BASE( hWnd )
{
}

clsSOUND_MANAGER_MISSION::~clsSOUND_MANAGER_MISSION()
{
}


void clsSOUND_MANAGER_MISSION::CreateSceneName()
{
	m_sSceneName = sDATA_PASS;
}