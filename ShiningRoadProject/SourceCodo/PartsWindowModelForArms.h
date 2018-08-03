#pragma once

#include "AssembleModel.h"


//腕を両腕表示したいがためのクラス.
class clsPARTS_WINDOW_MODEL_FOR_ARMS : public clsASSEMBLE_MODEL
{
public:
	clsPARTS_WINDOW_MODEL_FOR_ARMS();
	~clsPARTS_WINDOW_MODEL_FOR_ARMS();

	void UpdateProduct() final;

	//継承元と違い、第五引数のモデルを表示する.
	void Render(
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const enPARTS_TYPES RenderParts = enPARTS_TYPES::ENUM_SIZE/*,
		const D3DXVECTOR4& vColor = { 1.0f, 1.0f, 1.0f, 1.0f },
		const bool isAlpha = false*/ ) final;

private:
	//描画するか否かを吐き出す.
	bool isRender( const enPARTS_TYPES AlphaParts, const UINT uiIndex );

};
