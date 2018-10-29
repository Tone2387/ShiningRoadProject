#ifndef MENU_WINDOW_GAME_OVER_BASE_H_
#define MENU_WINDOW_GAME_OVER_BASE_H_


#include "MenuWindowBase.h"
#include "SceneGameOverInformation.h"

class clsMENU_WINDOW_GAME_OVER_BASE : public clsMENU_WINDOW_BASE
{
public:
	clsMENU_WINDOW_GAME_OVER_BASE(		
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		std::vector<unsigned int>* const pInformationVec );

	virtual ~clsMENU_WINDOW_GAME_OVER_BASE();



private:

};

#endif//#ifndef MENU_WINDOW_GAME_OVER_BASE_H_