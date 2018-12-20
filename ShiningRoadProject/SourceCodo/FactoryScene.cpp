#include "FactoryScene.h"

#include "SceneTitle.h"
#include "SceneAssemble.h"
#include "SceneTakeoff.h"
#include "SceneMission.h"
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
		break;
	case enSCENE::ASSEMBLE:
		return new clsSCENE_ASSEMBLE( ptrGroup );
		break;
	case enSCENE::TAKEOFF:
		return new clsSCENE_TAKEOFF( ptrGroup );
		break;
	case enSCENE::MISSION:
		return new clsSCENE_MISSION( ptrGroup );
		break;
	case enSCENE::ENDING:
		return new clsSCENE_ENDING( ptrGroup );
		break;
	case enSCENE::GAMEOVER:
		return new clsSCENE_GAME_OVER( ptrGroup );
		break;

	case enSCENE::EXIT_APP:
		return nullptr;
		break;
	default:
		assert( !"不正なシーンが指定されました" );
		break;
	}
	return nullptr;
}