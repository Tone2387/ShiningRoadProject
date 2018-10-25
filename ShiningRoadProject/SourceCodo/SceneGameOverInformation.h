#ifndef SCENE_GAME_OVER_INFORMATION_H_
#define SCENE_GAME_OVER_INFORMATION_H_



//âπÇÃà¯êî.
enum enBGM : int
{
	enBGM_MAOU2 = 0,
	enBGM_MUS0,
	enBGM_POKET0,
	enBGM_POKET1
};

enum enSE : int
{
	enSE_CURSOL_MOVE = 0,
	enSE_ENTER,
	enSE_EXIT,
	enSE_MISSION_START,
	enSE_WIN_APP,
	enSE_WIN_DISAPP,
};


//ÉÅÉjÉÖÅ[Ç©ÇÁéÛÇØéÊÇ¡ÇΩèÓïÒÇè∆çáÇ∑ÇÈindex.
enum enINFORMATION_INDEX : char
{
	enINFORMATION_INDEX_GAME_OVER = 0,
	enINFORMATION_INDEX_CONTINUE,
	enINFORMATION_INDEX_ASSEMBLE,
				 
	enINFORMATION_INDEX_size
};



#endif//#ifndef SCENE_GAME_OVER_INFORMATION_H_