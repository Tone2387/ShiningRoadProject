#pragma once 


#include "AssembleModel.h"


#define sBONE_NAME_COL_JOINT	"Joint"//00～xx.

#define sBONE_NAME_COL_CORE "Jenerator"//コアの中心.
#define sBONE_NAME_COL_HEAD "Center"//頭の中心.



//ミッション中にロボクラスが使う、パーツの集合体.
class clsMISSION_MODEL : public clsASSEMBLE_MODEL
{
public:

	clsMISSION_MODEL();
	~clsMISSION_MODEL();

	//ボーンに必要な情報.
	struct BONE_SET
	{
		int			iBoneNum;
		int			iParts;//何のパーツか(enPARTSをキャストしてこの変数に入れる).
	};

	enum enCOL_PARTS : UCHAR
	{
		enCOL_PARTS_LEG = 0,
		enCOL_PARTS_CORE,
		enCOL_PARTS_HEAD,
		enCOL_PARTS_ARMS,

		enCOL_PARTS_size
	};

	//Similarity:相似、類似性.
	//番号が違うだけの名前のボーンの数を返す.
	int GetSimilarityNameBoneNum(const enPARTS PartsNum, const char* strBoneName);

	//各パーツの当たり判定の数を返す.
	int GetColNum( const enCOL_PARTS enColParts );

	D3DXVECTOR3 GetDirfromBone(
		const enPARTS PartsNum,
		const char* strBoneRootName, 
		const char* strBoneEndName);

	D3DXVECTOR3 GetDirfromBone(
		const enPARTS PartsNum,
		const int enBoneRootName,
		const int enBoneEndName,
		const int iVecNum = 0);

	//当たり判定の座標の配列をすべて返す.
	std::vector< clsObject::SPHERE > GetColState(clsROBO_STATUS* const pStatus);

	void UpdateColPos();

	//指定したパーツの回転値を変更する.
	void SetPartsRotate(const enPARTS PartsNum, const D3DXVECTOR3 vRot);
	//指定したパーツのアニメーションを変える.
	void SetPartsAnimNo(const enPARTS PartsNum, const int iAnimIndex, const double dAnimTime = 0.0);
	//指定したパーツのアニメーション速度を変える.
	void SetPartsAnimSpeed(const enPARTS PartsNum, const double dAnimSpeed);
	//指定したパーツのアニメーションを通常再生にする.
	void SetPartsAnimNormal(const enPARTS PartsNum, const bool bAnimTimeInit = false);
	//指定したパーツのアニメーションを逆再生にする.
	void SetPartsAnimReverce(const enPARTS PartsNum, const bool bAnimTimeInit = false);

	//指定したパーツのアニメーション番号を渡す.
	const int GetPartsAnimNo(const enPARTS PartsNum);
	//指定したパーツのアニメーション終了を渡す true:アニメーション終了.
	const bool IsPartsAnimEnd(const enPARTS PartsNum);
	//指定したパーツのアニメーション再生時間を渡す.
	const double GetPartsAnimNowTime(const enPARTS PartsNum);
	//指定したパーツのアニメーション再生状態を渡す true:逆再生.
	const bool IsPartsAnimReverce(const enPARTS PartsNum);

	//前フレームのボーン位置を受け取るための関数群.
	//脚パーツ.
	const D3DXVECTOR3 GetBonePosLegBoosterRoot(const int);
	const D3DXVECTOR3 GetBonePosLegBoosterEnd(const int);

	const D3DXVECTOR3 GetBonePosLegJoint(const int);

	const D3DXVECTOR3 GetBonePosLegSole(const int);

	//頭パーツ.
	const D3DXVECTOR3 GetBonePosHeadCenter();

	//コアパーツ.
	const D3DXVECTOR3 GetBonePosCoreBoosterRoot(const int);
	const D3DXVECTOR3 GetBonePosCoreBoosterEnd(const int);

	const D3DXVECTOR3 GetBonePosCoreJenerator();
	
	//左腕パーツ.
	const D3DXVECTOR3 GetBonePosArmLBoostFrontRoot(const int);
	const D3DXVECTOR3 GetBonePosArmLBoostFrontEnd(const int);

	const D3DXVECTOR3 GetBonePosArmLBoostSideRoot(const int);
	const D3DXVECTOR3 GetBonePosArmLBoostSideEnd(const int);

	const D3DXVECTOR3 GetBonePosArmLBoostBackRoot(const int);
	const D3DXVECTOR3 GetBonePosArmLBoostBackEnd(const int);

	const D3DXVECTOR3 GetBonePosArmLJoint(const int);

	//右腕パーツ.
	const D3DXVECTOR3 GetBonePosArmRBoostFrontRoot(const int);
	const D3DXVECTOR3 GetBonePosArmRBoostFrontEnd(const int);

	const D3DXVECTOR3 GetBonePosArmRBoostSideRoot(const int);
	const D3DXVECTOR3 GetBonePosArmRBoostSideEnd(const int);

	const D3DXVECTOR3 GetBonePosArmRBoostBackRoot(const int);
	const D3DXVECTOR3 GetBonePosArmRBoostBackEnd(const int);

	const D3DXVECTOR3 GetBonePosArmRJoint(const int);

	//左武器パーツ.
	const D3DXVECTOR3 GetBonePosWeaponLMuzzleRoot();
	const D3DXVECTOR3 GetBonePosWeaponLMuzzleEnd();

	const D3DXVECTOR3 GetBonePosWeaponLCartridgeInjection();

	//右武器パーツ.
	const D3DXVECTOR3 GetBonePosWeaponRMuzzleRoot();
	const D3DXVECTOR3 GetBonePosWeaponRMuzzleEnd();

	const D3DXVECTOR3 GetBonePosWeaponRCartridgeInjection();

private:

	//引数のパーツの当たり判定の情報作成.
	//戻り値全パーツの当たり判定の数.
	int CreateColStateBones();

	//ボーンの数を確定する( CreateColStateBones関数の後に使う ).
	void FixBoneStates();
	//.
	//戻り値そのパーツの当たり判定の数.
	int CreateColStateBone( const enCOL_PARTS enColParts );

	//Create関数の中にある.
	void CreateProduct( clsROBO_STATUS* const pStatus ) override;

	//ボーン情報.
	std::vector< BONE_SET > m_vColStates;
	int						m_iColStateIndex;
	std::vector< D3DXVECTOR3 > m_vvColPos;//関数で渡す用の変数.

	int m_iColNum[ enCOL_PARTS_size ];//各パーツの当たり判定の数.

	int m_iColMax;//当たり判定の合計数.

};

