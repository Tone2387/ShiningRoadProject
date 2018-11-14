#include "SoundManagerAssemble.h"

using namespace std;

namespace{


}

clsSOUND_MANAGER_ASSEMBLE::clsSOUND_MANAGER_ASSEMBLE( const HWND hWnd )	: clsSOUND_MANAGER_BASE( hWnd )
{
}

clsSOUND_MANAGER_ASSEMBLE::~clsSOUND_MANAGER_ASSEMBLE()
{
}


void clsSOUND_MANAGER_ASSEMBLE::CreateSceneName()
{
	//このシーンのファイルにつなげるファイル名.
	const string sDATA_PASS = "Assemble";
	m_sSceneName = sDATA_PASS;
}