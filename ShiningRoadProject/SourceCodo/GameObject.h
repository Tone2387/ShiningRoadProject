#ifndef GAME_OBJ_
#define GAME_OBJ_


#define _USE_MATH_DEFINES
#include <math.h>


#include "Global.h"


class clsGameObject
{
public:
	clsGameObject();
	virtual ~clsGameObject();

	virtual void Init() = 0;

	//モデルデータ関連付け.
	virtual void AttachModel(){};
	//モデルデータ関連付け解除.
	virtual void DetatchModel(){};

	//描画.
//	virtual void Render(){};


	//位置設定.
	virtual void SetPosition( const D3DXVECTOR3 &vPos );
	void SetPositionX( const float fPosX );
	void SetPositionY( const float fPosY );
	void SetPositionZ( const float fPosZ );
	//取得.
	D3DXVECTOR3 GetPosition() const;
	float GetPositionX() const;
	float GetPositionY() const;
	float GetPositionZ() const;
	//足しこみ.
	void AddPosition( const D3DXVECTOR3 &vMove );
	void AddPositionX( const float fMoveX );
	void AddPositionY( const float fMoveY );
	void AddPositionZ( const float fMoveZ );


	//回転設定.
	//設定.
	void SetRotation( const D3DXVECTOR3 &vRot );
	void SetRotationY( const float fRotY );
	void SetRotationZ( const float fRotZ );
	//取得.
	D3DXVECTOR3 GetRotation() const;
	float GetRotationX() const;
	float GetRotationY() const;
	float GetRotationZ() const;
	//足しこみ.
	void AddRotation( const D3DXVECTOR3 &vMove );
	void AddRotationY( const float fMoveY );
	void AddRotationZ( const float fMoveZ );


	//拡縮設定.
	void SetScale( const float fScale );
	void SetScale( const D3DXVECTOR3 &vScale );
	D3DXVECTOR3 GetScale() const;


	//距離で音量を変える.
	int ChangeVolumeDistance( const float fEarPosZ, const float fThisPosZ );


protected:

	//座標や回転値を更新する.
	virtual void UpDateModel() = 0;
	//360～0に納める.
	void ThetaOverGuard( float& theta );
	void ThetaOverGuard( double& theta );

	D3DXVECTOR3		m_vPos;		//位置.
	D3DXVECTOR3		m_vRot;		//回転.
	D3DXVECTOR3		m_vScale;	//拡縮.



};



#endif//#ifndef GAME_OBJ_