#ifndef MENU_WINDOW_ASSEMBLE_COLOR_CHANGE_H_
#define MENU_WINDOW_ASSEMBLE_COLOR_CHANGE_H_


class clsASSEMBLE_MODEL;

#include "MenuWindowAssembleBase.h"


class clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE : public clsMENU_WINDOW_ASSEMBLE_BASE
{
public:
	clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE(
		clsPOINTER_GROUP* const pPtrGroup,
		clsMENU_WINDOW_BASE* const pParentWindow,
		std::vector<unsigned int>* const pInformationVec,
		clsASSEMBLE_MODEL* const pAssembleModel );

	~clsMENU_WINDOW_ASSEMBLE_COLOR_CHANGE();

private:

	void UpdateProduct() override;
	void RenderProduct() override;
	bool CreateNextWindowProduct( 
		clsMENU_WINDOW_BASE** ppOutNextWindow, 
		clsMENU_WINDOW_BASE* const pParentWindow ) override;


	std::vector< std::unique_ptr< clsSprite2D > > m_vecupColorGage;
	std::vector< std::unique_ptr< clsSprite2D > > m_vecupColorBone;

	clsASSEMBLE_MODEL* m_wpAssembleModel;

	enum enSELECT_NUM : int
	{
		enSELECT_NUM_1_R = 0,
		enSELECT_NUM_1_G,
		enSELECT_NUM_1_B,

		enSELECT_NUM_2_R,
		enSELECT_NUM_2_G,
		enSELECT_NUM_2_B,

		enSELECT_NUM_BACK,

		enSELECT_NUM_size
	};

};



#endif//#ifndef MENU_WINDOW_ASSEMBLE_COLOR_CHANGE_H_