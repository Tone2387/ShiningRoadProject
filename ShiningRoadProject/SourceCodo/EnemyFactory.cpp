#include"EnemyFactory.h"
#include"AIRobo.h"
#include"File.h"
#include"OperationString.h"
#include"AIRobo.h"
#include"RoboStatusEnemy.h"

const std::vector<clsCharactor*> clsEnemyFactory::CreateEnemy(clsPOINTER_GROUP* clsPtrGroup, std::string strFolderName)
{
	std::string strFileName = strFolderName + "\\Enemy.csv";

	clsFILE EnemyTransformFile;
	//clsFILE EnemyFile;

	clsOPERATION_STRING strOpr;

	assert(EnemyTransformFile.Open(strFileName));
	//assert(EnemyFile.Open("Data\\FileData\\Hiyoshi\\Enemy\\EnemyRoboData.csv"));

	std::vector<clsCharactor*> v_EnemysTmp;

	v_EnemysTmp.resize(EnemyTransformFile.GetSizeRow());

	for (int i = 0; i < v_EnemysTmp.size(); i++)
	{
		if (EnemyTransformFile.GetDataInt(i, enEnemyDataFileOrderEnemyType) == enEnemyTypeMinion)
		{

		}

		else if (EnemyTransformFile.GetDataInt(i, enEnemyDataFileOrderEnemyType) == enEnemyTypeRobo)
		{
			const int iNum = EnemyTransformFile.GetDataInt(i, enEnemyDataFileOrderEnemyNum);
			strFileName = "Data\\FileData\\Hiyoshi\\RoboAI\\Robo";

			strFileName = strOpr.ConsolidatedNumber(strFileName, iNum, GetNumDigit(iNum));

			clsAIRobo* clsEnemyTmp = new clsAIRobo;

			clsEnemyTmp->Init(clsPtrGroup, strFileName);

			D3DXVECTOR3 vPosTmp;
			vPosTmp.x = EnemyTransformFile.GetDataFloat(i, enEnemyDataFileOrderPosX);
			vPosTmp.y = EnemyTransformFile.GetDataFloat(i, enEnemyDataFileOrderPosY);
			vPosTmp.z = EnemyTransformFile.GetDataFloat(i, enEnemyDataFileOrderPosZ);

			clsEnemyTmp->SetPosition(vPosTmp);

			D3DXVECTOR3 vRotTmp;
			vRotTmp.x = EnemyTransformFile.GetDataFloat(i, enEnemyDataFileOrderPitch);
			vRotTmp.y = EnemyTransformFile.GetDataFloat(i, enEnemyDataFileOrderYaw);
			vRotTmp.z = EnemyTransformFile.GetDataFloat(i, enEnemyDataFileOrderRoll);

			clsEnemyTmp->SetRotation(vRotTmp);

			v_EnemysTmp[i] = clsEnemyTmp;
		}
	}

	return v_EnemysTmp;
}

clsEnemyFactory::clsEnemyFactory()
{
}

clsEnemyFactory::~clsEnemyFactory()
{
}

const int clsEnemyFactory::GetNumDigit(const int iNum)
{
	int iResult = 0;//結果.
	int iDigit = 1;//桁数.
	int iNumTmp = iNum;//数字.

	const int iDecimal = 10;//十進数.

	while (iResult == 0)
	{
		//10以上か?.
		if (iNumTmp < iDecimal)
		{
			//桁を確定.
			iResult = iDigit;
		}

		else
		{
			//桁を増やす.
			iNumTmp = iNumTmp / iDecimal;
			++iDigit;
		}
	}

	return iResult;
}