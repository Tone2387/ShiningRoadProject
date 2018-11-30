#ifndef EFFECTS_H_
#define EFFECTS_H_

//警告についてのコード分析を無効にする。4005:再定義.
#pragma warning( disable : 4005 )


//XAudioを使用する場合は有効にする.
//#define EFFECTS_USE_XAUDIO_


//==================================================
//	先にしておくべきヘッダ、ライブラリの読込.
//==================================================
#include <stdio.h>

#include <D3D11.h>
#include <XAudio2.h>
#pragma comment( lib, "d3d11.lib" )

//==================================================
//	ここからEffekseer関係 ヘッダ、ライブラリの読込.
//==================================================
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <EffekseerSoundXAudio2.h>


#ifdef _DEBUG
#pragma comment( lib, "VS2013\\Debug\\Effekseer.lib" )
#pragma comment( lib, "VS2013\\Debug\\EffekseerRendererDX11.lib" )
#pragma comment( lib, "VS2013\\Debug\\EffekseerSoundXAudio2.lib" )
#else//#ifdef _DEBUG
#pragma comment( lib, "VS2013\\Release\\Effekseer.lib" )
#pragma comment( lib, "VS2013\\Release\\EffekseerRendererDX11.lib" )
#pragma comment( lib, "VS2013\\Release\\EffekseerSoundXAudio2.lib" )
#endif//#ifdef _DEBUG


//「D3DX～」使用で必須.
//注意:Effekseer関係より後に読み込むこと.
#include <D3DX10.h>
#pragma comment( lib, "d3dx10.lib" )


//シングルトンの時はつける.
//#define EFFECTS_CLASS_SINGLETON

#include <vector>

//==================================================
//	フリーソフトEffekseerのデータを使うためのクラス.
//==================================================
class clsEffects
{
public:

//	//エフェクト種類列挙体.
//	enum enEFFECTS : UCHAR
//	{
//		//Arbia.
//		enEFFECTS_STEP = 0,	//asiato.v
//		enEFFECTS_ARBIA_ATK,//arbia_kougeki.v
//		enEFFECTS_WAVE,		//syougekiha.v
//		enEFFECTS_KICK_HIT,	//kougeki.v
//
//		//Enemy.
//		enEFFECTS_ENEMY_ATK,//teki_kougeki.
//		enEFFECTS_SLASH_HIT,//yarare.v
//		enEFFECTS_SLASH_DISC,//bikkuri.v
//
//		//Pendulum.
//		enEFFECTS_PEND_FIRE,//hibana.v
//
//		//TreasureBox.
//		enEFFECTS_TRB_OPEN,	//takara_hasira.v
//		enEFFECTS_TRB_LOOP,	//takara_nakami.v
//
//		//Door.
//		enEFFECTS_DOOR_DUST,//takara_nakami.v
//
//		//for loop.
//		enEFFECTS_MAX
//	};

#ifdef EFFECTS_CLASS_SINGLETON
	//インスタンス取得(唯一のアクセス経路).
	static clsEffects* GetInstance()
	{
		//唯一のインスタンスを作成する.
		//(staticで作成されたので2回目は無視される).
		static clsEffects s_Instance;
		return &s_Instance;
	}
#else//EFFECTS_CLASS_SINGLETON
	clsEffects();
#endif//EFFECTS_CLASS_SINGLETON
	~clsEffects();

	//構築関数.
	HRESULT Create( 
		ID3D11Device* const pDevice,
		ID3D11DeviceContext* const pContext );

	//描画.
	void Render( 
		const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, 
		const D3DXVECTOR3& vEye ) const;


	//再生関数.
	::Effekseer::Handle Play( const int EfcType, const D3DXVECTOR3 &vPos ) const 
	{
		return m_pManager->Play(
			m_vecpEffect[EfcType], vPos.x, vPos.y, vPos.z );
	};
	//一時停止.
	void Paused( const ::Effekseer::Handle handle, const bool bFlag ) const 
	{
		m_pManager->SetPaused( handle, bFlag );	//bFlag:true = 一時停止.
	}
	//停止.
	void Stop( const ::Effekseer::Handle handle ) const 
	{
		m_pManager->StopEffect( handle );
	}
	//すべて停止.
	void StopAll() const 
	{
		if( !m_pManager )return;
		m_pManager->StopAllEffects();
	}
	
	//位置指定.
	void SetPosition( const ::Effekseer::Handle handle, const D3DXVECTOR3 &vPos ) const 
	{
		m_pManager->SetLocation( handle,
			::Effekseer::Vector3D( vPos.x, vPos.y, vPos.z ) );
	}


	//回転指定.
	void SetRotation( const ::Effekseer::Handle handle, const D3DXVECTOR3 &vRot ) const
	{
		m_pManager->SetRotation( handle,
			vRot.x, vRot.y, vRot.z );
	}
	void SetRotation( 
		const ::Effekseer::Handle handle,
		const D3DXVECTOR3 &vAxis, const float fAngle ) const 
	{
			m_pManager->SetRotation( handle,
				::Effekseer::Vector3D( vAxis.x, vAxis.y, vAxis.z ),
				fAngle );
	}

	//サイズ指定.
	void SetScale( const ::Effekseer::Handle handle, const float fScale ) const 
	{
		m_pManager->SetScale( handle,
			fScale, fScale, fScale );
	}
	void SetScale( const ::Effekseer::Handle handle, const D3DXVECTOR3 &vScale ) const 
	{
		m_pManager->SetScale( handle,
			vScale.x, vScale.y, vScale.z );
	}

	//スピード指定.
	void SetSpd( const ::Effekseer::Handle handle, const float fSpd ) const 
	{
		m_pManager->SetSpeed( handle, fSpd );
	}


	//動いてる?.
	bool isPlay( const ::Effekseer::Handle handle ) const
	{
		return m_pManager->Exists( handle );
	}

private:

#ifdef EFFECTS_CLASS_SINGLETON
	//生成やコピーを禁止する.
	clsEffects();
	clsEffects( const clsEffects& rhs );
	clsEffects& operator = ( const clsEffects& rhs ) const;
#endif//#ifdef EFFECTS_CLASS_SINGLETON

	//初期化.
	HRESULT Init( 
		ID3D11Device* const pDevice,
		ID3D11DeviceContext* const pContext );

	//データ読込関数.
	HRESULT LoadData();

	//破棄関数.
	HRESULT Destroy();

	//データ解放関数.
	HRESULT ReleaseData();

	//ビュー行列設定.
	void SetViewMatrix( const D3DXMATRIX& mView ) const;
	//プロジェクション行列設定.
	void SetProjectionMatrix( const D3DXMATRIX& mProj ) const;

	//ベクター変換関数.
	::Effekseer::Vector3D Vector3DDxToEfk( 
					const D3DXVECTOR3* pSrcVec3Dx ) const;
	D3DXVECTOR3 Vector3EfkToDx( 
					const ::Effekseer::Vector3D* pSrcVec3Efk ) const;

	//行列変換関数.
	::Effekseer::Matrix44 MatrixDxToEfk(
					const D3DXMATRIX* pSrcMatDx ) const;
	D3DXMATRIX MatrixEfkToDx(
					const ::Effekseer::Matrix44* pSrcMatEfk ) const;


private:

	//エフェクトを作動させるために必要.
	::Effekseer::Manager*			m_pManager;
	::EffekseerRenderer::Renderer*	m_pRender;

#ifdef EFFECTS_USE_XAUDIO_
	::EffekseerSound::Sound*		m_pSound;
	//エフェクトデータに含まれる音再生に必要.
	IXAudio2*						m_pXA2;
	IXAudio2MasteringVoice*			m_pXA2Master;
#endif//#ifdef EFFECTS_USE_XAUDIO_

	//エフェクトの種類ごとに必要.
	std::vector< ::Effekseer::Effect* >	m_vecpEffect;


};

//=======================================================
//	使い方.
//=======================================================
//
//	//エフェクトを出したいクラスのヘッダーに.
//	//これと同じ型の変数を宣言する.
//	::Effekseer::Handle		m_Handle;
//
//
//	//----- cppでの使用例 -----//.
//
//	//出したいタイミング.
//	m_ehBomb = m_wpEffect->Play( 0, m_vPos );
//	m_wpEffect->SetRotation( m_ehBomb, m_vRot );



#endif//#ifndef EFFECTS_H_