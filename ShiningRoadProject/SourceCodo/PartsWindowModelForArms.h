#ifndef PARTS_WINDOW_MODEL_FOR_ARMS_H_
#define PARTS_WINDOW_MODEL_FOR_ARMS_H_

#include "AssembleModel.h"


//�r�𗼘r�\�������������߂̃N���X.
class clsPARTS_WINDOW_MODEL_FOR_ARMS : public clsASSEMBLE_MODEL
{
public:
	clsPARTS_WINDOW_MODEL_FOR_ARMS();
	~clsPARTS_WINDOW_MODEL_FOR_ARMS();


	//�p�����ƈႢ�A��܈����̃��f����\������.
	void Render(
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const enPARTS_TYPES RenderParts = enPARTS_TYPES::ENUM_SIZE,
		ID3D11DeviceContext* const pContext = nullptr ) final;

private:
	void UpdateProduct() final;

	//�`�悷�邩�ۂ���f���o��.
	bool isRender( const enPARTS_TYPES AlphaParts, const UINT uiIndex ) const;

};
#endif//#ifndef PARTS_WINDOW_MODEL_FOR_ARMS_H_