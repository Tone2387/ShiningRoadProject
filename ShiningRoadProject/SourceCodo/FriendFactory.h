#include"Global.h"
#include"Player.h"
#include"RoboStatusEnemy.h"

class clsFriendFactory
{
public:
	clsFriendFactory();
	~clsFriendFactory();

	const std::vector<clsCharactor*> 
		CreateFriend(
		clsPOINTER_GROUP* clsPtrGroup, 
		const std::string& strFolderName,
		clsPlayer* pPlayer);

private:
	enum enFriendType
	{
		enFriendTypeMinion = 0,
		enFriendTypeRobo,

		enFriendTypeSize
	};

	enum enFriendDataFileOrder
	{
		enFriendDataFileOrderFriendType = 0,
		enFriendDataFileOrderFriendNum,
		enFriendDataFileOrderPosX,
		enFriendDataFileOrderPosY,
		enFriendDataFileOrderPosZ,
		enFriendDataFileOrderPitch,
		enFriendDataFileOrderYaw,
		enFriendDataFileOrderRoll,

		enFriendDataFileOrderSize
	};
};