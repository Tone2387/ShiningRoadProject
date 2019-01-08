#ifndef PARTS_WINDOW_MODEL_H_
#define PARTS_WINDOW_MODEL_H_


#include "AssembleModel.h"
#include "PartsWindowModelForArms.h"
class clsROBO_STATUS;

class clsPARTS_WINDOW_MODEL
{
public:
	clsPARTS_WINDOW_MODEL( clsResource* const pResource, clsROBO_STATUS* const pStatus );
	~clsPARTS_WINDOW_MODEL();

	void Update( const short Type, const short Num );

	void Render(
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vLight, 
		const D3DXVECTOR3& vEye,
		const bool isRender ) const;

	//アセンブルシーンのパーツウィンドウのカメラの高さの基準にする.
	D3DXVECTOR3 GetSelectPartsHeight();

	//色を受け取る.
	void SetColors( const std::vector< D3DXVECTOR4 >& vecvColors ){
		m_vecvColors = vecvColors;
	};

private:

	//選択中パーツ.
	std::unique_ptr< clsPARTS_WINDOW_MODEL_FOR_ARMS > m_upRoboModel;

	//武器.
	std::unique_ptr< clsPARTS_WEAPON >	m_upWeapon;

	//パーツ選択用変数.
	enPARTS			m_SelectType;
	SKIN_ENUM_TYPE	m_SelectNum;

	clsResource* m_wpResource;

	//色.
	std::vector< D3DXVECTOR4 > m_vecvColors;
};

#endif//#ifndef PARTS_WINDOW_MODEL_H_