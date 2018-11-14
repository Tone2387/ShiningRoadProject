#ifndef MENU_WINDOW_TITLE_BASE_H_
#define MENU_WINDOW_TITLE_BASE_H_

#include "MenuWindowBase.h"
#include "SceneTitleInformation.h"

class clsMENU_WINDOW_TITLE_BASE : public clsMENU_WINDOW_BASE
{
public:
	clsMENU_WINDOW_TITLE_BASE(		
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		std::vector<unsigned int>* const pInformationVec );

	virtual ~clsMENU_WINDOW_TITLE_BASE();

protected:

private:

};

#endif//#ifndef MENU_WINDOW_TITLE_BASE_H_