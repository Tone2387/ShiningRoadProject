#include"Global.h"
#include"Charactor.h"
#include"RoboStatusEnemy.h"

class clsEnemyFactory
{
public:
	clsEnemyFactory();
	~clsEnemyFactory();

	const std::vector<clsCharactor*> CreateEnemy(std::string strFolderName);

private:
	enum enEnemyType
	{
		enEnemyTypeMinion = 0,
		enEnemyTypeRobo,

		enEnemyTypeSize
	};

	enum enEnemyDataFileOrder
	{
		enEnemyDataFileOrderEnemyType = 0,
		enEnemyDataFileOrderEnemyNum,
		enEnemyDataFileOrderPosX,
		enEnemyDataFileOrderPosY,
		enEnemyDataFileOrderPosZ,
		enEnemyDataFileOrderPitch,
		enEnemyDataFileOrderYaw,
		enEnemyDataFileOrderRoll,

		enEnemyDataFileOrderSize
	};
};

