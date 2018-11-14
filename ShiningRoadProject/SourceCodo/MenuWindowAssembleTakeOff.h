#ifndef MENU_WINDOW_ASSEMBLE_TAKE_OFF_H_
#define MENU_WINDOW_ASSEMBLE_TAKE_OFF_H_


#include "MenuWindowAssembleBase.h"


class clsMENU_WINDOW_ASSEMBLE_TAKE_OFF : public clsMENU_WINDOW_ASSEMBLE_BASE
{
public:
	clsMENU_WINDOW_ASSEMBLE_TAKE_OFF(
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		std::vector<unsigned int>* const pInformationVec );

	~clsMENU_WINDOW_ASSEMBLE_TAKE_OFF();

private:

	void UpdateProduct() override;
	void RenderProduct() override;
	bool CreateNextWindowProduct( 
		clsMENU_WINDOW_BASE** ppOutNextWindow, 
		clsMENU_WINDOW_BASE* const pParentWindow ) override { return false; };

};



#endif//#ifndef MENU_WINDOW_ASSEMBLE_TAKE_OFF_H_