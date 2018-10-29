#ifndef MENU_WINDOW_ASSEMBLE_COLOR_CHANGE_H_
#define MENU_WINDOW_ASSEMBLE_COLOR_CHANGE_H_


#include "MenuWindowAssembleBase.h"


class clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE : public clsMENU_WINDOW_ASSEMBLE_BASE
{
public:
	clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE(
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		std::vector<unsigned int>* const pInformationVec );

	~clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE();

private:

	void UpdateProduct() override;
	void RenderProduct() override;
	bool CreateNextWindowProduct( 
		clsMENU_WINDOW_BASE** ppOutNextWindow, 
		clsMENU_WINDOW_BASE* const pParentWindow ) override;

};



#endif//#ifndef MENU_WINDOW_ASSEMBLE_COLOR_CHANGE_H_