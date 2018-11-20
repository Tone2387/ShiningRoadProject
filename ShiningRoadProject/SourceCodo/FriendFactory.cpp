#include"FriendFactory.h"
#include"AIRobo.h"
#include"File.h"
#include"OperationString.h"
#include"AIRobo.h"
#include"RoboStatusEnemy.h"

const std::vector<clsCharactor*>
clsFriendFactory::CreateFriend(
clsPOINTER_GROUP* clsPtrGroup,
std::string strFolderName,
clsPlayer* pPlayer)
{
	clsFILE TransformFile;
	clsOPERATION_STRING strOpr;

	std::vector<clsCharactor*> v_FriendsTmp;

	//プレイヤー作成.
	std::string strFileName = strFolderName + "\\Player.csv";
	
	assert(TransformFile.Open(strFileName));
	
	v_FriendsTmp.resize(1);

	clsPlayer* pPlayerTmp = new clsPlayer;

	pPlayerTmp->Init(clsPtrGroup);

	D3DXVECTOR3 vPosTmp;
	vPosTmp.x = TransformFile.GetDataFloat(0, enFriendDataFileOrderPosX);
	vPosTmp.y = TransformFile.GetDataFloat(0, enFriendDataFileOrderPosY);
	vPosTmp.z = TransformFile.GetDataFloat(0, enFriendDataFileOrderPosZ);

	pPlayerTmp->SetPosition(vPosTmp);

	D3DXVECTOR3 vRotTmp;
	vRotTmp.x = TransformFile.GetDataFloat(0, enFriendDataFileOrderPitch);
	vRotTmp.y = TransformFile.GetDataFloat(0, enFriendDataFileOrderYaw);
	vRotTmp.z = TransformFile.GetDataFloat(0, enFriendDataFileOrderRoll);

	pPlayerTmp->SetRotation(vRotTmp);

	//プレイヤーを0番目に登録.
	v_FriendsTmp[0] = pPlayerTmp;

	pPlayer = pPlayerTmp;

	TransformFile.Close();

	//仲間がいる場合作成.
	strFileName = strFolderName + "\\Friend.csv";

	if (TransformFile.Open(strFileName))
	{
		v_FriendsTmp.resize(TransformFile.GetSizeRow());

		for (int i = 1; i < v_FriendsTmp.size(); i++)
		{
			if (TransformFile.GetDataInt(i, enFriendDataFileOrderFriendType) == enFriendTypeMinion)
			{

			}

			else if (TransformFile.GetDataInt(i, enFriendDataFileOrderFriendType) == enFriendTypeRobo)
			{
				const int iNum = TransformFile.GetDataInt(i, enFriendDataFileOrderFriendNum);

				clsAIRobo* pFriendTmp = new clsAIRobo;

				pFriendTmp->Init(clsPtrGroup, iNum);

				vPosTmp.x = TransformFile.GetDataFloat(i, enFriendDataFileOrderPosX);
				vPosTmp.y = TransformFile.GetDataFloat(i, enFriendDataFileOrderPosY);
				vPosTmp.z = TransformFile.GetDataFloat(i, enFriendDataFileOrderPosZ);

				pFriendTmp->SetPosition(vPosTmp);

				vRotTmp.x = TransformFile.GetDataFloat(i, enFriendDataFileOrderPitch);
				vRotTmp.y = TransformFile.GetDataFloat(i, enFriendDataFileOrderYaw);
				vRotTmp.z = TransformFile.GetDataFloat(i, enFriendDataFileOrderRoll);

				pFriendTmp->SetRotation(vRotTmp);

				v_FriendsTmp[i] = pFriendTmp;
			}
		}
	}

	TransformFile.Close();

	return v_FriendsTmp;
}

clsFriendFactory::clsFriendFactory()
{
}

clsFriendFactory::~clsFriendFactory()
{
}