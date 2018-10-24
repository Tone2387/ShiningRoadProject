#ifndef MENU_WINDOW_GAME_OVER_DO_ASSEMBLE_H_
#define MENU_WINDOW_GAME_OVER_DO_ASSEMBLE_H_

#include "MenuWindowGameOverBase.h"


//コンティニューすると答えたときに出現し、ロボを組みなおすか聞いてくるメニュー.
class clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE : public clsMENU_WINDOW_GAME_OVER_BASE
{
public:
	clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE(
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		unsigned int* const pInformationArray );

	~clsMENU_WINDOW_GAME_OVER_DO_ASSEMBLE();

private:

	void UpdateProduct() override;
	void RenderProduct() override;
	bool CreateNextWindowProduct( 
		clsMENU_WINDOW_BASE** ppOutNextWindow, 
		clsMENU_WINDOW_BASE* const pParentWindow ) override;


};

#endif//#ifndef MENU_WINDOW_GAME_OVER_DO_ASSEMBLE_H_