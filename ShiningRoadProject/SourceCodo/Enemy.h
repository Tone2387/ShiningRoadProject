#pragma once

class clsEnemyBase
{
public:
	clsEnemyBase();
	~clsEnemyBase();

	//sasjdflasldj

private:


public:
	//-----ç\ë¢ëÃ-----//
	enum enMovePattan
	{
		enMovePattanA = 0,
		enMovePattanB,
		enMovePattanC,

		enMovePattanMax
	};

	struct EnemyMoveState
	{
		enMovePattan MovePattan;

		float fHorDistance;
		float fHorRandDis;

		float fVerDistance;
		float fVerRandDis;

		int iAngle;
		int iRandAngle;
	};

	struct EnemyMoveData
	{
		int iMovePattan;
		EnemyMoveState** MoveState;
	};

	struct EnemyShotState
	{
		float fShotDisMax;
		float fShotDisMin;
		int iShotENMin;
	};
};

clsEnemyBase::clsEnemyBase()
{
}

clsEnemyBase::~clsEnemyBase()
{
}