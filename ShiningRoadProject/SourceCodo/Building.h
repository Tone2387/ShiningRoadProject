#ifndef BUILDING_H_
#define BUILDING_H_

#include "Global.h"
#include "CharaStatic.h"
#include "Sprite.h"
#include <vector>

//ステージに設置するビルクラス.
//大きさを変えてもテクスチャが引き伸ばされることはない.
//そんなブロックがこちら.
class clsBUILDING
{
public:
	clsBUILDING( 
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11,
		 clsDX9Mesh* const pModel );
	~clsBUILDING();

	//毎フレーム使ってはいけない.
	//.
	void Update();

	void Render(
		const D3DXMATRIX &mView, 
		const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight, 
		const D3DXVECTOR3 &vEye );

	//レイ用.
	clsDX9Mesh* GetModelPtr();

	D3DXVECTOR3 GetPos();
	void SetPos( const D3DXVECTOR3& vPos );
	void AddPos( const D3DXVECTOR3& vPos );

	D3DXVECTOR3 GetRot();
	void SetRot( const D3DXVECTOR3& vRot );
	void AddRot( const D3DXVECTOR3& vRot );

	D3DXVECTOR3 GetScale();
	void SetScale( const D3DXVECTOR3& vScale );
	void AddScale( const D3DXVECTOR3& vScale );


private:

	struct TRANSFORM
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vRot;
		D3DXVECTOR3 vScale;
	};

	enum enWALL_DIRECTION : int
	{
		enWD_SOUTH = 0,
		enWD_EAST,
		enWD_NORTH,
		enWD_WEST,

		enWALL_DIRECTION_size
	};

	//タイルの目標数を作る.
	void SetTileNumTargetTop( unsigned int& puiRow, unsigned int& puiCol );
	//目標の数に合わせてタイルを増減する.
	void SetTileNumTop( const unsigned int uiROW, const unsigned int uiCOL );
	//タイルを並べる.
	void SetTransformTop();

	//Z=北と南.//引数は軸方向.
	void SetTileNumTargetSide( 
		unsigned int& puiRowZ, unsigned int&  puiColZ,
		unsigned int& puiRowX, unsigned int&  puiColX );
	//目標の数に合わせてタイルを増減する.
	void SetTileNumSide( 
		const unsigned int uiROW_Z, const unsigned int uiCOL_Z,
		const unsigned int uiROW_X, const unsigned int uiCOL_X );
	//タイルを並べる.
	void SetTransformSide();

	//引数に入れたタイルが中心から見て何度の位置にあるか.
	float GetTileTheta( 
		const TRANSFORM& Tile, const TRANSFORM& Center,
		float* const pfTheta, float* const pfDistance ) const;
	//回転に応じて座標を更新する.
	D3DXVECTOR3 GetTilePosForRotation( 
		D3DXVECTOR3* const vTilePos,
		const D3DXVECTOR3& vCenterPos,
		const float fTileTheta, 
		const float fTileDistance );


	//本体.
	TRANSFORM m_Trans;
	//上面.
	std::vector< std::vector< TRANSFORM > > m_vvTop;
	//側面.
	std::vector< std::vector< TRANSFORM > > m_vvSide[ enWALL_DIRECTION_size ];


	std::unique_ptr< clsCharaStatic > m_upBox;


	std::unique_ptr< clsSprite > m_upTop;
	std::unique_ptr< clsSprite > m_upSide;

	ID3D11Device*  m_wpDevice;
	ID3D11DeviceContext* m_wpContext;

};
#endif//#ifndef BUILDING_H_