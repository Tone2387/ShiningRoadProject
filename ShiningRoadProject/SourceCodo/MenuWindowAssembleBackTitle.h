#ifndef MENU_WINDOW_ASSEMBLE_BACK_TITLE
#define MENU_WINDOW_ASSEMBLE_BACK_TITLE


#include "MenuWindowAssembleBase.h"


class clsMENU_WINDOW_ASSEMBLE_BACK_TITLE : public clsMENU_WINDOW_ASSEMBLE_BASE
{
public:
	clsMENU_WINDOW_ASSEMBLE_BACK_TITLE(
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		std::vector<unsigned int>* const pInformationVec );

	~clsMENU_WINDOW_ASSEMBLE_BACK_TITLE();

private:

};


#endif//#ifndef MENU_WINDOW_ASSEMBLE_BACK_TITLE