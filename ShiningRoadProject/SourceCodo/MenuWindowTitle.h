#ifndef MENU_WINDOW_TITLE_H_
#define MENU_WINDOW_TITLE_H_


#include "MenuWindowBase.h"

class clsMENU_WINDOW_TITLE : public clsMENU_WINDOW_BASE
{
public:
	clsMENU_WINDOW_TITLE(		
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		unsigned int* const pInformationArray );

	~clsMENU_WINDOW_TITLE();



private:

	void UpdateProduct() override;
	void RenderProduct() override;
	bool CreateNextWindowProduct( 
		clsMENU_WINDOW_BASE** ppOutNextWindow, 
		clsMENU_WINDOW_BASE* const pParentWindow ) override;




};
#endif//#ifndef MENU_WINDOW_TITLE_H_