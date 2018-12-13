#ifndef MENU_WINDOW_MISSION_PAUSE_H_
#define MENU_WINDOW_MISSION_PAUSE_H_

#include "MenuWindowMissionBase.h"

class clsMENU_WINDOW_MISSION_PAUSE : public clsMENU_WINDOW_MISSION_BASE
{
public:
	clsMENU_WINDOW_MISSION_PAUSE(
		const HWND hWnd,
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		std::vector<unsigned int>* const pInformationVec );

	~clsMENU_WINDOW_MISSION_PAUSE();

private:

	void UpdateProduct() override;
	void RenderProduct() override;
	bool CreateNextWindowProduct( 
		clsMENU_WINDOW_BASE** ppOutNextWindow, 
		clsMENU_WINDOW_BASE* const pParentWindow ) override { return false; };

};



#endif//#ifndef MENU_WINDOW_MISSION_PAUSE_H_