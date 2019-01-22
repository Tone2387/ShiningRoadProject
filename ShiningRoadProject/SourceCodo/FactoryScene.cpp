#include "FactoryScene.h"

#include "SceneTitle.h"
#include "SceneAssemble.h"
#include "SceneMovieTakeoff.h"
#include "SceneMission.h"
#include "SceneMovieClear.h"
#include "SceneEnding.h"
#include "SceneGameOver.h"




clsSCENE_BASE* clsFACTORY_SCENE::Create( 
	const enSCENE enScene,
	clsPOINTER_GROUP* const ptrGroup ) const
{
	//指定されたシーンを作って吐き出す.
	switch( enScene )
	{
	case enSCENE::TITLE:
		return new clsSCENE_TITLE( ptrGroup );

	case enSCENE::ASSEMBLE:
		return new clsSCENE_ASSEMBLE( ptrGroup );

	case enSCENE::MOVIE_TAKEOFF:
		return new clsSCENE_MOVIE_TAKEOFF( ptrGroup );

	case enSCENE::MISSION:
		return new clsSCENE_MISSION( ptrGroup );

	case enSCENE::MOVIE_CLEAR:
		return new clsSCENE_MOVIE_CLEAR( ptrGroup );

	case enSCENE::ENDING:
		return new clsSCENE_ENDING( ptrGroup );

	case enSCENE::GAMEOVER:
		return new clsSCENE_GAME_OVER( ptrGroup );


	case enSCENE::EXIT_APP:
		return nullptr;

	default:
		assert( !"不正なシーンが指定されました" );
		break;
	}
	return nullptr;
}