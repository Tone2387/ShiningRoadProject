#include "FactorySoundManager.h"

//#include "SoundManagerBase.h"

#include "SoundManagerTitle.h"
#include "SoundManagerAssemble.h"
#include "SoundManagerTakeoff.h"
#include "SoundManagerMission.h"
#include "SoundManagerClear.h"
#include "SoundManagerEnding.h"
#include "SoundManagerGameOver.h"

clsSOUND_MANAGER_BASE* clsFACTORY_SOUND_MANAGER::Create( const enSCENE enNextScene, HWND const hWnd ) const
{
	switch( enNextScene )
	{
	case enSCENE::TITLE:
		return new clsSOUND_MANAGER_TITLE( hWnd );

	case enSCENE::ASSEMBLE:
		return new clsSOUND_MANAGER_ASSEMBLE( hWnd );

	case enSCENE::MOVIE_TAKEOFF:
		return new clsSOUND_MANAGER_TAKEOFF( hWnd );

	case enSCENE::MISSION:
		return new clsSOUND_MANAGER_MISSION( hWnd );

	case enSCENE::MOVIE_CLEAR:
		return new clsSOUND_MANAGER_CLEAR( hWnd );

	case enSCENE::ENDING:
		return new clsSOUND_MANAGER_ENDING( hWnd );

	case enSCENE::GAMEOVER:
		return new clsSOUND_MANAGER_GAME_OVER( hWnd );


	case enSCENE::EXIT_APP:
		return nullptr;
		break;

	default:
		assert( !"不正なシーンが指定されました" );
		break;
	}

	return nullptr;
}
