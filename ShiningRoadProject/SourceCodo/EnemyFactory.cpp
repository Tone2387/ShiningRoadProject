#include"EnemyFactory.h"
#include"File.h"

const std::vector<clsCharactor*> clsEnemyFactory::CreateEnemy(std::string strFolderName)
{
	std::string strFileName = strFolderName + "\\Enemy.csv";

	clsFILE File;

	assert(File.Open(strFileName));

	std::vector<clsCharactor*> v_EnemysTmp;

	v_EnemysTmp.resize(File.GetSizeRow());

	for (int i = 0; i < v_EnemysTmp.size(); i++)
	{
		switch (File.GetDataInt(i,enEnemyDataFileOrderEnemyType))
		{
		case clsEnemyFactory::enEnemyTypeMinion:

			break;
		case clsEnemyFactory::enEnemyTypeRobo:

			break;

		default:

			

			break;
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