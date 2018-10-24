#ifndef MENU_WINDOW_TITLE_BASE_H_
#define MENU_WINDOW_TITLE_BASE_H_

#include "MenuWindowBase.h"

class clsMENU_WINDOW_TITLE_BASE : public clsMENU_WINDOW_BASE
{
public:
	clsMENU_WINDOW_TITLE_BASE(		
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		unsigned int* const pInformationArray );

	virtual ~clsMENU_WINDOW_TITLE_BASE();

protected:


	enum enSE : int
	{
		enSE_CURSOL_MOVE = 0,
		enSE_ENTER,
		enSE_EXIT,

		enSE_BOMBER,

		enSE_WIN_APP,
		enSE_WIN_DIS_APP
	};

	//ìnÇ∑èÓïÒÇÃìYÇ¶éö.
	const char m_INFORMATION__INDEX_GAME_END;
	const char m_INFORMATION__INDEX_NEXT_SCENE;
	const char m_INFORMATION__INDEX_CLOSE_MENU;


private:

};

#endif//#ifndef MENU_WINDOW_TITLE_BASE_H_