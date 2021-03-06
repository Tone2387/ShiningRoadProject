#ifndef BUILDING_H_
#define BUILDING_H_


class clsRobo;

#include "Global.h"
#include"ObjStaticMesh.h"
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
		 clsDX9Mesh* const pModel,
		std::shared_ptr< clsSprite > spTop,
		std::shared_ptr< clsSprite > spSide,
		std::shared_ptr< clsSprite > spSideInside,
		std::shared_ptr< clsSprite > spTopInside,
		std::shared_ptr< clsSprite > spBottomInside );
	~clsBUILDING();

	//ビルのテクスチャリソースを作成.
	//ビルを内包する「 clsStage 」で使う.
	static void CreateTexture(
		ID3D11Device* const pDevice11,
		ID3D11DeviceContext* const pContext11,
		std::shared_ptr< clsSprite >* const pspTop,
		std::shared_ptr< clsSprite >* const pspSide,
		std::shared_ptr< clsSprite >* const pspSideInside,
		std::shared_ptr< clsSprite >* const pspTopInside,
		std::shared_ptr< clsSprite >* const pspBottomInside );

	//レイの当たり判定に必要.
	void UpdateModel() const;
	//毎フレーム使ってはいけない.
	//TransFormを変更したときにだけ使う.
	void UpdateTile();

	void Render(
		const D3DXMATRIX &mView,
		const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight,
		const D3DXVECTOR3 &vEye,
		const D3DXVECTOR4& vColor = { 1.0f, 1.0f, 1.0f, 1.0f } ) const;

	void RenderInside(
		const D3DXMATRIX &mView,
		const D3DXMATRIX &mProj,
		const D3DXVECTOR3 &vLight,
		const D3DXVECTOR3 &vEye ) const;


	//ビルの近くにいるか( 上から見た円の判定 ).
	//ビルと判定を取りたいモノの座標.
	bool isNearBuilding( const D3DXVECTOR3& vPosObjOtherBuilding );

	//ビルの中に入ったギガポンを追い出す.
	void KickOutInside( clsRobo* const pRobo );


	//レイ用.
	clsDX9Mesh* GetModelPtr() const {
#ifdef _DEBUG
		clsDX9Mesh* pReturn = m_upBox->GetStaticMesh();
		return pReturn;
#else//#ifdef _DEBUG
		return m_upBox->GetStaticMesh();
#endif//#ifdef _DEBUG
	}

	//座標.
	D3DXVECTOR3 GetPos() const				{ return m_Trans.vPos; }
	void SetPos( const D3DXVECTOR3& vPos )	{ m_Trans.vPos = vPos;	this->UpdateTile(); }
	void AddPos( const D3DXVECTOR3& vPos )	{ m_Trans.vPos += vPos; this->UpdateTile(); }
	//回転.
	D3DXVECTOR3 clsBUILDING::GetRot() const				{ return m_Trans.vRot; }
	void clsBUILDING::SetRot( const D3DXVECTOR3& vRot )	{ m_Trans.vRot = vRot;	this->UpdateTile(); }
	void clsBUILDING::AddRot( const D3DXVECTOR3& vRot )	{ m_Trans.vRot += vRot; this->UpdateTile(); }


	D3DXVECTOR3 GetScale() const				{ return m_Trans.vScale; }
	void SetScale( const D3DXVECTOR3& vScale )	{ m_Trans.vScale = vScale;	this->UpdateTile(); }
	void AddScale( const D3DXVECTOR3& vScale )	{ m_Trans.vScale += vScale; this->UpdateTile(); }


private:

	struct TRANSFORM
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vRot;
		D3DXVECTOR3 vScale;
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
		const float fTileDistance ) const;

private:

	enum enWALL_DIRECTION : int
	{
		enWD_SOUTH = 0,
		enWD_EAST,
		enWD_NORTH,
		enWD_WEST,

		enWALL_DIRECTION_size
	};

	//本体.
	TRANSFORM m_Trans;
	//上面.
	TRANSFORM m_TopTrans;
	//側面.
	TRANSFORM m_SideTransArray[ enWALL_DIRECTION_size ];

	//分割数.
	D3DXVECTOR2 m_vSplitTop; 
	D3DXVECTOR2 m_vSplitNorthSouth; 
	D3DXVECTOR2 m_vSplitEastWest; 


	std::unique_ptr< clsObjStaticMesh > m_upBox;

	//テクスチャ.
	std::shared_ptr< clsSprite > m_spTop;
	std::shared_ptr< clsSprite > m_spSide;
	//裏面( カメラがビルの中に入った時の為 ).
	std::shared_ptr< clsSprite > m_spSideInside;
	std::shared_ptr< clsSprite > m_spTopInside;
	std::shared_ptr< clsSprite > m_spBottomInside;



	ID3D11Device*  m_wpDevice;
	ID3D11DeviceContext* m_wpContext;

};
#endif//#ifndef BUILDING_H_