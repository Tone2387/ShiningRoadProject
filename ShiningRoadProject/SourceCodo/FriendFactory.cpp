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
	std::string strFileName = strFolderName + "\\Friend.csv";
	clsFILE TransformFile;
	clsOPERATION_STRING strOpr;
	assert(TransformFile.Open(strFileName));
	std::vector<clsCharactor*> v_FriendsTmp;

	v_FriendsTmp.resize(TransformFile.GetSizeRow());

	for (int i = 1; i < v_FriendsTmp.size(); i++)
	{
		if (TransformFile.GetDataInt(i, enFriendDataFileOrderFriendType) == enFriendTypeMinion)
		{

		}

		else if (TransformFile.GetDataInt(i, enFriendDataFileOrderFriendType) == enFriendTypeRobo)
		{
			const int iNum = TransformFile.GetDataInt(i, enFriendDataFileOrderFriendNum);
			strFileName = "Data\\FileData\\Hiyoshi\\RoboAI\\Robo";

			strFileName = strOpr.ConsolidatedNumber(strFileName, iNum, GetNumDigit(iNum));

			clsAIRobo* clsFriendTmp = new clsAIRobo;

			clsFriendTmp->Init(clsPtrGroup, strFileName);

			D3DXVECTOR3 vPosTmp;
			vPosTmp.x = TransformFile.GetDataFloat(i, enFriendDataFileOrderPosX);
			vPosTmp.y = TransformFile.GetDataFloat(i, enFriendDataFileOrderPosY);
			vPosTmp.z = TransformFile.GetDataFloat(i, enFriendDataFileOrderPosZ);

			clsFriendTmp->SetPosition(vPosTmp);

			D3DXVECTOR3 vRotTmp;
			vRotTmp.x = TransformFile.GetDataFloat(i, enFriendDataFileOrderPitch);
			vRotTmp.y = TransformFile.GetDataFloat(i, enFriendDataFileOrderYaw);
			vRotTmp.z = TransformFile.GetDataFloat(i, enFriendDataFileOrderRoll);

			clsFriendTmp->SetRotation(vRotTmp);

			v_FriendsTmp[i] = clsFriendTmp;
		}
	}

	return v_FriendsTmp;
}

const std::vector<clsCharactor*> clsFriendFactory::CreateFriend(clsPOINTER_GROUP* clsPtrGroup, std::string strFolderName)
{
	std::string strFileName = strFolderName + "\\Friend.csv";

	clsFILE TransformFile;
	//clsFILE FriendFile;

	clsOPERATION_STRING strOpr;

	assert(TransformFile.Open(strFileName));
	//assert(FriendFile.Open("Data\\FileData\\Hiyoshi\\Friend\\FriendRoboData.csv"));

	std::vector<clsCharactor*> v_FriendsTmp;

	v_FriendsTmp.resize(TransformFile.GetSizeRow());

	for (int i = 1; i < v_FriendsTmp.size(); i++)
	{
		if (TransformFile.GetDataInt(i, enFriendDataFileOrderFriendType) == enFriendTypeMinion)
		{

		}

		else if (TransformFile.GetDataInt(i, enFriendDataFileOrderFriendType) == enFriendTypeRobo)
		{
			const int iNum = TransformFile.GetDataInt(i, enFriendDataFileOrderFriendNum);
			strFileName = "Data\\FileData\\Hiyoshi\\RoboAI\\Robo";

			strFileName = strOpr.ConsolidatedNumber(strFileName, iNum, GetNumDigit(iNum));

			clsAIRobo* clsFriendTmp = new clsAIRobo;

			clsFriendTmp->Init(clsPtrGroup, strFileName);

			D3DXVECTOR3 vPosTmp;
			vPosTmp.x = TransformFile.GetDataFloat(i, enFriendDataFileOrderPosX);
			vPosTmp.y = TransformFile.GetDataFloat(i, enFriendDataFileOrderPosY);
			vPosTmp.z = TransformFile.GetDataFloat(i, enFriendDataFileOrderPosZ);

			clsFriendTmp->SetPosition(vPosTmp);

			D3DXVECTOR3 vRotTmp;
			vRotTmp.x = TransformFile.GetDataFloat(i, enFriendDataFileOrderPitch);
			vRotTmp.y = TransformFile.GetDataFloat(i, enFriendDataFileOrderYaw);
			vRotTmp.z = TransformFile.GetDataFloat(i, enFriendDataFileOrderRoll);

			clsFriendTmp->SetRotation(vRotTmp);

			v_FriendsTmp[i] = clsFriendTmp;
		}
	}

	return v_FriendsTmp;
}

clsFriendFactory::clsFriendFactory()
{
}

clsFriendFactory::~clsFriendFactory()
{
}

const int clsFriendFactory::GetNumDigit(const int iNum)
{
	int iResult = 0;//åãâ .
	int iDigit = 1;//åÖêî.
	int iNumTmp = iNum;//êîéö.

	const int iDecimal = 10;//è\êiêî.

	while (iResult == 0)
	{
		//10à»è„Ç©?.
		if (iNumTmp < iDecimal)
		{
			//åÖÇämíË.
			iResult = iDigit;
		}

		else
		{
			//åÖÇëùÇ‚Ç∑.
			iNumTmp = iNumTmp / iDecimal;
			++iDigit;
		}
	}

	return iResult;
}