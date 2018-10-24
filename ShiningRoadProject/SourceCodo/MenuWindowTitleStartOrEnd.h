#ifndef MENU_WINDOW_TITLE_START_OR_END_H_
#define MENU_WINDOW_TITLE_START_OR_END_H_


#include "MenuWindowTitleBase.h"

class clsMENU_WINDOW_TITLE_START_OR_END : public clsMENU_WINDOW_TITLE_BASE
{
public:
	clsMENU_WINDOW_TITLE_START_OR_END(		
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		unsigned int* const pInformationArray );

	~clsMENU_WINDOW_TITLE_START_OR_END();



private:

	void UpdateProduct() override;
	void RenderProduct() override;
	bool CreateNextWindowProduct( 
		clsMENU_WINDOW_BASE** ppOutNextWindow, 
		clsMENU_WINDOW_BASE* const pParentWindow ) override;


};
#endif//#ifndef MENU_WINDOW_TITLE_START_OR_END_H_