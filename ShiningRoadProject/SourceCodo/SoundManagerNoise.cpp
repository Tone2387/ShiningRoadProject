#include "SoundManagerNoise.h"

using namespace std;

clsSOUND_MANAGER_NOISE::clsSOUND_MANAGER_NOISE( const HWND hWnd )	: clsSOUND_MANAGER_BASE( hWnd )
{
}

clsSOUND_MANAGER_NOISE::~clsSOUND_MANAGER_NOISE()
{
}

void clsSOUND_MANAGER_NOISE::CreateSceneName()
{
	//���̃V�[���̃t�@�C���ɂȂ���t�@�C����.
	const string sDATA_PASS = "Noise";
	m_sSceneName = sDATA_PASS;
}

