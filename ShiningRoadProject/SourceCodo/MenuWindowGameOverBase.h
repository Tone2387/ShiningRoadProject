#ifndef MENU_WINDOW_GAME_OVER_BASE_H_
#define MENU_WINDOW_GAME_OVER_BASE_H_


#include "MenuWindowBase.h"

class clsMENU_WINDOW_GAME_OVER_BASE : public clsMENU_WINDOW_BASE
{
public:
	clsMENU_WINDOW_GAME_OVER_BASE(		
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		std::vector<unsigned int>* const pInformationVec );

	virtual ~clsMENU_WINDOW_GAME_OVER_BASE();

protected:


	enum enSE : int
	{
		enSE_CURSOL_MOVE = 0,
		enSE_ENTER,
		enSE_EXIT,
		enSE_MISSION_START,
		enSE_WIN_APP,
		enSE_WIN_DISAPP,
	};

	//ìnÇ∑èÓïÒÇÃìYÇ¶éö.
	const char m_INFORMATION__INDEX_GAME_OVER;
	const char m_INFORMATION__INDEX_CONTINUE;
	const char m_INFORMATION__INDEX_ASSEMBLE;

private:

};

#endif//#ifndef MENU_WINDOW_GAME_OVER_BASE_H_