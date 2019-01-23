#include"EnemyFactory.h"
#include"AIRobo.h"
#include"File.h"
#include"OperationString.h"
#include"AIRobo.h"
#include"RoboStatusEnemy.h"

const std::vector<clsCharactor*> clsEnemyFactory::CreateEnemy(
	clsPOINTER_GROUP* clsPtrGroup, 
	const std::string& strFolderName)
{
	std::string strFileName = strFolderName + "\\Enemy.csv";
	clsFILE EnemyTransformFile;

	assert(EnemyTransformFile.Open(strFileName));

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

			clsAIRobo* clsEnemyTmp = new clsAIRobo;
			clsEnemyTmp->Init(clsPtrGroup, iNum);

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