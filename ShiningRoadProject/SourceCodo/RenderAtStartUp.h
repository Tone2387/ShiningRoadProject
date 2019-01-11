#ifndef RENDER_AT_START_UP_H_
#define RENDER_AT_START_UP_H_

class clsBLEND_STATE;

//#include "Global.h"
#include "RenderSubThreadBase.h"
#include "Sprite2DCenter.h"
#include "LineBox.h"
#include "UiText.h"
#include "BlackScreen.h"
#include <vector>
#include <string>

class clsRENDER_AT_START_UP : public clsRENDER_SUB_THREAD_BASE
{
public:
	clsRENDER_AT_START_UP(
		ID3D11Device*			const pDevice,
		ID3D11DeviceContext*	const pContext,
		IDXGISwapChain*			const pSwapChain,
		ID3D11RenderTargetView*	const pBackBuffer_TexRTV,
		ID3D11DepthStencilView*	const pBackBuffer_DSTexDSV );
	~clsRENDER_AT_START_UP();

	//���[�v����.
	void Loop() override;

	//���[�h���I��������Ɏg��.
	void FinishLoad() override;

private:
	
	void UpdateProduct() override;
	void RenderProduct() const override;

	enum class enMODE
	{
		LINE_V,		//�O�g���c�ɑ傫���Ȃ�.
		LINE_H,		//�O�g�����ɑ傫���Ȃ�.

		GAGE_H,		//�Q�[�W�̘g�����ɑ傫���Ȃ�.
		GAGE_V,		//�Q�[�W�̘g���c�ɑ傫���Ȃ�.

		GAGE_MOVE,	//�Q�[�W������.

		COMPLETE,	//���[�h����.
	}	m_enMode;

	//switch���̒��g.
	void BiggerLineBoxV();//�O�g���c�ɑ傫���Ȃ�.
	void BiggerLineBoxH();//�O�g�����ɑ傫���Ȃ�.
	void BiggerGageBoxH();//�Q�[�W�̘g�����ɑ傫���Ȃ�.
	void BiggerGageBoxV();//�Q�[�W�̘g���c�ɑ傫���Ȃ�.
	void UpdateLoadMsg();//�Q�[�W������.

	void Complete();	//���[�h������.


private:

	int m_iTimer;//���[�h�́A�ҋ@����.


	std::unique_ptr< clsLINE_BOX > m_upLineBox;
	std::unique_ptr< clsLINE_BOX > m_upGageBox;

	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vecupGage;//�Q�[�W�{��.

	std::unique_ptr< clsBLACK_SCREEN > m_upBlack;

	//���S.
	std::unique_ptr< clsSPRITE2D_CENTER > m_upLogo;

	//���[�h�����b�Z�[�W.
	std::unique_ptr< clsUiText > m_upText;
	std::string m_sLodeMsg;



#ifdef _DEBUG
	std::vector< std::unique_ptr< clsSPRITE2D_CENTER > > m_vecupDebugImage;
#endif//#ifdef _DEBUG

};


#endif//#ifndef RENDER_AT_START_UP_H_